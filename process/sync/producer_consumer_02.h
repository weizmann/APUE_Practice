/*
 * =====================================================================================
 *
 *       Filename:  producer_consumer_02.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013/12/09 11时31分47秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <pthread.h>

#define MAX_ITEMS 10000
#define MAX_THREADS 100

int nitems;
int buff[MAX_ITEMS];

struct {
	pthread_mutex_t mutex;
	int nput;
	int nval;
} put = {
	PTHREAD_MUTEX_INITIALIZER
};

struct {
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	int nready;
} nready = {
	PTHREAD_MUTEX_INITIALIZER,
	PTHREAD_COND_INITIALIZER,
};

























