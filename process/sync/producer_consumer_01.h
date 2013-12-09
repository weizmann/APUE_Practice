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


