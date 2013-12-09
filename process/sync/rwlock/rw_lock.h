#ifndef RW_LOCK_H
#define RW_LOCK_H

typedef struct {
	pthread_mutex_t rw_mutex;
	pthread_cond_t  rw_cond_readers;
	pthread_cond_t  rw_cond_writers;

	int 			rw_magic;
	int 			rw_nwaitreaders;
	int 			rw_nwaitwriters;

	int 			rw_refcount;
} rwlock_t;

#define RW_MAGIC 0x11223344
#define RWLOCK_INIT { \
	PTHREAD_MUTEX_INITIALIZER, \
	PTHREAD_COND_INITIALIZER, \
	PTHREAD_COND_INITIALIZER, \
	RW_MAGIC, 0, 0, 0 \
}

typedef int rwlock_attr_t;

int rwlock_destroy(rwlock_t*);
int rwlock_init(rw_lock_t*, rwlock_attr_t*);

int rwlock_rdlock(rwlock_t*);
int rwlock_tryrdlock(rwlock_t*);

int rwlock_wrlock(rwlock_t*);
int rwlock_trywrlock(rwlock_t*);

int rwlock_unlock(rwlock_t*);

#endif
