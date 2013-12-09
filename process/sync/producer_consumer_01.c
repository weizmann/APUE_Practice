/*
 * =====================================================================================
 *
 *       Filename:  producer_consumer_01.c
 *
 *    Description: This file is an example in UNP vol.2
 *    1. learn how to use pthread lib APIs; pthread_create / pthread_join
 *    2. using Macro variables;
 *    3. lock a data and operation together.
 *
 *        Version:  1.0
 *        Created:  2013/12/08 23时37分43秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <pthread.h>

#define MAXITEMS 1000000
#define MAXTHREADS 100

int nitems;
struct {
	pthread_mutex_t mutex;
	int buff[MAXITEMS];
	int nputs;
	int nval;
} shared = {
	PTHREAD_MUTEX_INITIALIZER
};

void *produce(void*), *consume(void*);

#define min(a, b) ((a) > (b) ? (b) : (a))
#define max(a, b) ((a) < (b) ? (a) : (b))

int main(int argc, char* argv[]) {
	int i, nthreads, count[MAXTHREADS];
	pthread_t tid_produce[MAXTHREADS], tid_consume;

	if (argc != 3) {
		printf("Usage : produce_consume <#items> <#threads>\n");
		return -1;
	}

	nitems = min(atoi(argv[1]), MAXITEMS);
	nthreads = min(atoi(argv[2]), MAXTHREADS);

	for (i = 0; i < nthreads; i++) {
		count[i] = i;
		if (pthread_create(&tid_produce[i], NULL, produce, &count[i]) != 0) {
			printf("create thread error...\n");
			return(-1);
		}
	}

	for (i = 0; i < nthreads; i++) {
		if (pthread_join(tid_produce[i], NULL) != 0) {
			printf("pthread join error...\n");
			return(-1);
		} 
		printf("count[%d] = %d\n", i, count[i]);
	}

	if (pthread_create(&tid_consume, NULL, consume, NULL) != 0) {
		printf("create consume thread error..\n");
		return -1;
	}

	if (pthread_join(tid_consume, NULL) != 0) {
		printf("pthread join consume thread error...\n");
		return -1;
	}
	return(0);

}

#ifndef USE_LOCK
#define USE_LOCK 1
#endif

void *produce(void* arg) {
	for (;;) {
#ifdef USE_LOCK
		pthread_mutex_lock(&shared.mutex);
#endif
		if (shared.nputs >= nitems) {
#ifdef USE_LOCK
			pthread_mutex_unlock(&shared.mutex);
#endif
			return NULL;
		}
		shared.buff[shared.nputs] = shared.nval;
		shared.nputs++;
		shared.nval++;
#ifdef USE_LOCK
		pthread_mutex_unlock(&shared.mutex);
#endif
		*((int*) arg) += 1;
	}
	return NULL;
}

void *consume(void* arg) {
	int i;
	for (i = 0; i< nitems; i++) {
		if (shared.buff[i] != i) {
			printf("consume thread check failed: %d is an error number\n", i);
		}
	}
	return NULL;
}















