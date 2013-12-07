#include <stdio.h>
#include "/usr/include/apue.h"
#include <pthread.h>

pthread_t ntid;

void print_ids(const char* s) {
	pid_t pid;
	pthread_t tid;
	pid = getpid();
	tid = pthread_self();
	printf("%s pid %u tid %u (0x%x) \n", s, (unsigned int)pid, (unsigned int)tid, (unsigned int)tid);
}

void* thr_fn(void* arg) {
	print_ids("new thread : ");
	return ((void*) 0);
}

int main() {
	int err;

	err = pthread_create(&ntid, NULL, thr_fn, NULL);
	if (err != 0) {
		printf("Cannot create thread ...\n");
	}
	print_ids("main thread : ");
	sleep(1);
	return 0;
}
