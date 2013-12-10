#ifndef MY_SEMAPHORE
#define MY_SEMAPHORE

typedef struct {
	int sem_fd[2];
	int sem_magic;
} sem_t;

#define SEM_MAGIC 0x123456

#ifdef SEM_FAILED
#undef SEM_FAILED
#define SEM_FAILED ((sem_t*) (-1))
#endif

#endif
