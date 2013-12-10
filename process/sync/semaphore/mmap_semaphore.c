/*
 * =====================================================================================
 *
 *       Filename:  mmap_semaphore.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013/12/10 20时33分30秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "mmap_semaphore.h"

#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

sem_t* sem_open(const char* pathname, int oflag, ...) {
	int fd, i, created, save_errno;
	mode_t mode;
	va_list ap;
	sem_t* sem, seminit;
	struct stat statbuff;
	unsigned int value;
	pthread_mutexattr_t mattr;
	pthread_condattr_t cattr;

	created = 0;
	sem = MAP_FAILED;
again:
	if (oflag & O_CREAT) {
		va_start(ap, oflag);
		mode = va_arg(ap, va_mode_t) & ~S_IXUSR;
		value = va_arg(ap, unsigned int);
		va_end(ap);
		
		fd = open(pathname, oflag | O_EXCL | O_RDWR, mode | S_IXUSR);
		if (fd < 0) {
			if (errno == EEXIST && (oflag & O_EXCL) == 0) {
				goto exists;
			} else {
				return SEM_FAILED;
			}
		}
		created = 1;
		bzero(&seminit, sizeof(seminit));
		if (write(fd, &seminit, sizeof(seminit)) != sizeof(seminit)) {
			goto err;
		}
		sem = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
		if (sem == MAP_FAILED) {
			goto err;
		}

		if ((i = pthread_mutexattr_init(&mattr)) != 0) {
			goto pthreaderr;
		}
		pthread_mutexattr_setpshared(&mattr, PTHREAD_PROCESS_SHARED);
		i = pthread_mutex_init(&sem->sem_mutex, &mattr);
		pthread_mutexattr_destroy(&mattr);
		if (i != 0) {
			goto pthreaderr;
		}

		if ((i = pthread_condattr_init(&cattr)) != 0) {
			goto pthreaderr;
		}
		pthread_condattr_setpshared(&cattr, PTHEAD_PROCESS_SHARED);
		i = pthread_cond_init(&sem->sem_cond, &cattr);
		pthread_condattr_destroy(&cattr);
		if (i != 0) {
			goto pthreaderr;
		}

		if ((sem->sem_count = value) > sysconf(_SC_SEM_VALUE_MAX)) {
			errno = EINVAL;
			goto err;
		}

		if (fchmod(fd, mode) == -1) {
			goto err;
		}
		close(fd);
		sem->sem_magic = SEM_MAGIC;
		return sem;
	}
exists:
	if ((fd = open(pathname, O_RDWR)) < 0) {
		if (errno = ENOENT && (oflag & O_CREAT)) {
			goto again;
		}
		goto err;
	}
	sem = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (sem == MAP_FAILED) {
		goto err;
	}

	// make centain init is complete. using stat function.
	for (i = 0; i < MAX_TRIES; i++) {
		if (stat(pathname, &statbuff) == -1) {
			if (errno == ENOENT && (oflag & O_CREAT)) {
				close(fd);
				goto again;
			}
			goto err;
		}
		if ((statbuff.st_mode & S_IXUSR) == 0) {
			close(fd);
			sem->sem_magic = SEM_MAGIC;
			return sem;
		}
		sleep(1);
	}

	errno = ETIMEDOUT;
	goto err;
pthreaderr:
	errno = i;
err:
	save_errno = errno;
	if (created) {
		unlink(pathname);
	}
	if (sem != MAP_FAILED) {
		munmap(sem, sizeof(sem_t));
	}
	close(fd);
	errno = save_errno;
	return SEM_FAILED;
}

int sem_close(sem_t* sem) {
	if (sem->sem_magic != SEM_MAGIC) {
		errno = EINVAL;
		return -1;
	}
	if (munmap(sem, sizeof(sem_t)) == -1) {
		return -1;
	}
	return 0;
}

int sem_unlink(const char* pathname) {
	if (unlink(pathname) == -1) {
		return -1;
	}
	return 0;
}


int sem_post(sem_t* sem) {
	int n;
	if (sem->sem_magic != SEM_MAGIC) {
		errno = EINVAL;
		return = -1;
	}

	if ((n = pthread_mutex_lock(&sem->sem_mutex)) != 0) {
		errno = n;
		return -1;
	}

	if (sem->sem_count == 0) {
		pthread_cond_signal(&sem->sem_cond);
	}
	sem->sem_count++;
	pthread_mutex_unlock(&sem->sem_mutex);
	return 0;
}

int sem_wait(sem_t* sem) {
	int n;
	if (sem->sem_magic != SEM_MAGIC) {
		errno = EINVAL;
		return -1;
	}
	if((n = pthread_mutex_lock(&sem->sem_mutex)) != 0) {
		errno = n;
		return -1;
	}

	while (sem->sem_count == 0) {
		pthread_cond_wait(&sem->sem_cond, &sem->sem_mutex);
	}
	sem->sem_count--;
	pthread_mutex_unlock(&sem->sem_mutex);
	return 0
}
