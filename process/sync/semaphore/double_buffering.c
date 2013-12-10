/*
 * =====================================================================================
 *
 *       Filename:  double_buffering.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013/12/10 17时47分18秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#include "/usr/include/apue.h"

#define NBUFF 8
#define BUFFSIZE 100

struct {
	struct {
		char data[BUFFSIZE];
		ssize_t n;
	} buff[NBUFF];
	sem_t mutex, nempty, nstored;
} shared;

int fd;
void *produce(void*), *consume(void*);

int main(int argc, char* argv[]) {
	pthread_t tid_produce, tid_consume;
	if (argc != 2) {
		printf("usage: double_buffering <pathname>\n");
		return -1;
	}

	fd = open(argv[1], O_RDONLY);
	
	sem_init(&shared.mutex, 0, 1);
	sem_init(&shared.nempty, 0, NBUFF);
	sem_init(&shared.nstored, 0, 0);

	pthread_create(&tid_produce, NULL, produce, NULL);
	pthread_create(&tid_consume, NULL, consume, NULL);

	pthread_join(tid_produce, NULL);
	pthread_join(tid_consume, NULL);

	sem_destroy(&shared.mutex);
	sem_destroy(&shared.nempty);
	sem_destroy(&shared.nstored);

	return 0;
}
void* produce(void* arg) {
	int i;
	for (i = 0;;) {
		sem_wait(&shared.nempty);
		shared.buff[i].n = read(fd, shared.buff[i].data, BUFFSIZE);
		if (shared.buff[i].n == 0) {
			sem_post(&shared.nstored);
			return NULL;
		}
		if (++i >= NBUFF) {
			i = 0;
		}
		sem_post(&shared.nstored);
	}
}

void* consume(void* arg) {
	int i;
	for (i = 0; ;) {
		sem_wait(&shared.nstored);
		if (shared.buff[i].n == 0) {
			return NULL;
		}
		write(STDOUT_FILENO, shared.buff[i].data, shared.buff[i].n);
		if (++i >= NBUFF) {
			i = 0;
		}
		sem_post(&shared.nempty);
	}
}


