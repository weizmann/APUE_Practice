/*
 * =====================================================================================
 *
 *       Filename:  producer_consumer_02.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013/12/09 12时36分02秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "producer_consumer_02.h"

void* produce(void *arg) {
	for (;;) {
		pthread_mutex_lock(&put.mutex);
		if (put.nput >= nitems) {
			pthread_mutex_unlock(&put.mutex);
			return NULL;
		}
		buff[put.nput] = put.val;
		put.nput++;
		put.nval++;
		pthread_mutex_unlock(&put.mutex);

		pthread_mutex_lock(&nready.mutex);
		if (nready.nready == 0) {
			pthread_cond_signal(&nready.cond);
		}
		nready.nready++;
		pthread_mutex_unlock(&nready.mutex);
		*((int*)arg)+=1;
	}
	return NULL;
}

void* consume(void* arg) {
	int i;
	for (i = 0; i < nitems; i++) {
		pthread_mutex_lock(&nready.mutex);
		while(nready.nready == 0) {
			pthread_cond_wait(&nready.cond, &nready.mutex);
		}
		nready.nready--;
		pthread_mutex_unlock(&nready.mutex);

		if (buff[i] != i) {
			printf("mutex error in %dth element!!!\n", i);
		}
	}
	return NULL;
}

