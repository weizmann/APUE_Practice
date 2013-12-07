#include <stdlib.h>
#include "pthread.h"

struct foo {
	int					f_count;
	pthread_mutex_t		f_lock;
	struct foo			*f_next;
	int 				f_id;
};

#define NHASH 29
#define HASH(fp) (((unsigned int) fp) % NHASH)
struct foo *fh[NHASH];

pthread_mutex_t hash_lock = PTHREAD_MUTEX_INITIALIZER;

struct foo* foo_alloc() {
	struct foo* fp;
	int index;

	if ((fp = malloc(sizeof(struct foo))) != NULL) {
		fp->f_count = 1;
		if (pthread_mutex_init(&fp->f_lock, NULL) != 0) {
			free(fp);
			return NULL;
		}
		index = HASH(fp);
		pthread_mutex_lock(&hash_lock);
		fp->f_next = 
	}
}
