/*
 * =====================================================================================
 *
 *       Filename:  my_semaphore.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013/12/10 18时56分08秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "my_semaphore.h"

#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

sem_t* sem_open(const char* pathname, int oflag, ...) {
	int i, flags, save_errno;
	char c;
	mode_t mode;
	va_list ap;
	sem_t* sem;
	unsigned int value;

	if (oflag & O_CREAT) {
		va_start(ap, oflag);
		mode = va_arg(ap, va_mode_t);
		value = va_arg(ap, unsigned int);
		va_end(ap);

		if (mkfifo(pathname, mode) < 0) {
			if (errno == EEXIST && (oflag & O_EXCL) == 0) {
				oflag &= ~O_CREAT;
			} else {
				return SEM_FAILED;
			}
		}
	}

	if ((sem = malloc(sizeof(sem_t))) == NULL) {
		return SEM_FAILED;
	}
	sem->sem_fd[0] = sem->sem_fd[1] = -1;

	if ((sem->sem_fd[0] = open(pathname, O_RDONLY | O_NONBLOCK)) < 0) {
		goto error;
	}
	if ((sem->sem_fd[1] = open(pathname, O_WRONLY | O_NONBLOCK)) < 0) {
		goto error;
	}

	if ((flags = fcntl(sem->sem_fd[0], F_GETFL, 0)) < 0) {
		goto error;
	}
	flags &= ~O_NONBLOCK;
	if (fcntl(sem->sem_fd[0], F_SETFL, 0) < 0) {
		goto error;
	}

	if (oflag & O_CREAT) {
		for (i = 0; i < value; i++) {
			if (write(sem->sem_fd[1], &c, 1) != 1) {
				goto error;
			}
		}
	}

	sem->sem_magic = SEM_MAGIC;
	return sem;
error:
	save_errno = errno;
	if (oflag & O_CREAT) {
		unlink(pathname);
	}
	close(sem->sem_fd[0]);
	close(sem->sem_fd[1]);
	free(sem);
	errno = save_errno;
	return SEM_FAILED;
}


int sem_close(sem_t* sem) {
	if (sem->sem_magic != SEM_MAGIC) {
		errno = EINVAL;
		return -1;
	}

	sem->sem_magic = 0;
	if (close(sem->sem_fd[0]) == -1 || close(sem->sem_fd[1]) == -1) {
		free(sem);
		return -1;
	}
	free(sem);
	return 0;
}

int sem_unlink(const char* pathname) {
	return unlink(pathname);
}

int sem_post(sem_t* sem) {
	char c;
	if (sem->sem_magic != SEM_MAGIC) {
		errno = EINVAL;
		return -1;
	}

	if (write(sem->sem_fd[1], &c, 1) == 1) {
		return 0;
	}
	return -1;
}

int sem_wait(sem_t* sem) {
	char c;
	if (sem->sem_magic != SEM_MAGIC) {
		errno = EINVAL;
		return -1;
	}

	if (read(sem->sem_fd[0], &c, 1) == 1) {
		return 0;
	}
	return -1;
}
