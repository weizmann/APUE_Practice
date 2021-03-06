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
#include "producer_consumer_01.h"

#define USE_LOCK 1
// those macro var would be needless in an advanced programming language with boolean type.

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















