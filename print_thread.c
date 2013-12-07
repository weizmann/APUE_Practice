#include "apue.h"
#include "my_err.h"

pthread_t ntid;

void printids(const char* s)
{
	pid_t pid;
	pthread_t tid;

	pid = getpid();
	tid = pthread_self();
	printf("%s pid %u tid %u (0x%x)\n", s, (unsigned int)pid,
						(unsigned int) tid, (unsigned int)tid);
}

void* my_thr_fn(void* arg)
{
	printtids("new thread: ");
	return (void*)0;
}

int main(){
	int err;
	err = pthread_create(&ntid, NULL, my_thr_fn, NULL);
	if(err != 0)
		printf("cannot create correctly...\n");
	printids("main thread: ");
	sleep(1);
	exit(0);
}










