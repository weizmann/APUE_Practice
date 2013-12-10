/*
 * =====================================================================================
 *
 *       Filename:  producer_consumer_loop.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013/12/10 16时54分39秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "/usr/include/apue.h"
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>

#define NBUFF 10

#define SEM_MUTEX "/tmp/mutex"
#define SEM_NEMPTY "/tmp/nempty"
#define SEM_NSTORED "/tmp/nstored"

#define OPEN_MODE O_CREAT | O_EXCL

int nitems;
struct {
	int buff[NBUFF];
	sem_t *mutex, *nempty, *nstored;
} shared;

void *produce(void*), *consume(void*);

int main(int argc, char* argv[]) {
	pthread_t tid_produce, tid_consume;

	if (argc != 2) {
		printf("usage: prodcons1 <#items>\n");
		return -1;
	}

	nitems = atoi(argv[1]);

	shared.mutex = sem_open(SEM_MUTEX, OPEN_MODE, FILE_MODE, 1);
	shared.nempty = sem_open(SEM_NEMPTY, OPEN_MODE, FILE_MODE, NBUFF);
	shared.nstored = sem_open(SEM_NSTORED, OPEN_MODE, FILE_MODE, 0);

	pthread_create(&tid_produce, NULL, produce, NULL);
	pthread_create(&tid_consume, NULL, consume, NULL);

	pthread_join(tid_produce, NULL);
	pthread_join(tid_consume, NULL);

	sem_unlink(SEM_MUTEX);
	sem_unlink(SEM_NEMPTY);
	sem_unlink(SEM_NSTORED);
	return 0;
}

void* produce(void* arg) {
	int i;
	for (i = 0; i < nitems; i++) {
		sem_wait(shared.nempty);
		sem_wait(shared.mutex);
		shared.buff[i % NBUFF] = i;
		sem_post(shared.mutex);
		sem_post(shared.nstored);
	}
	return NULL;
}

void* consume(void* arg) {
	int i;
	for (i = 0; i < nitems; i++) {
		sem_wait(shared.nstored);
		sem_wait(shared.mutex);
		if (shared.buff[i % NBUFF] != i) {
			printf("error in buffer %d !!\n", i);
		} else {
			printf("buffer %d is correct!!\n", i);
		}
		sem_post(shared.mutex);
		sem_post(shared.nempty);
	}
	return NULL;
}


