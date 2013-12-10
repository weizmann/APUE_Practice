#ifndef MMAP_SEMAPHORE
#define MMAP_SEMAPHORE

typedef struct {
	pthread_mutex_t sem_mutex;
	pthread_cond_t sem_cond;
	unsigned int sem_count;
	int sem_magic;
} sem_t;

#define SEM_MAGIC 0x123456
#ifdef SEM_FAILED
#undef SEM_FAILED
#define SEM_FAILED ((sem_t*) -1)
#endif

#endif
