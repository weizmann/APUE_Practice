#include <pthread.h>
#include "/usr/include/apue.h"
#include "/usr/include/my_error.h"

void* thr_fn1(void* arg) {
	printf("thread 1 returning ....\n");
	return ((void*)1);
}

void* thr_fn2(void* arg) {
	printf("thread 2 exiting.... \n");
	pthread_exit((void*)2);
}

int main() {
	int err;
	pthread_t tid_1, tid_2;
	void *thread_return_value;

	err = pthread_create(&tid_1, NULL, thr_fn1, NULL);
	if (err != 0) {
		printf("Cannot create thread %s \n", strerror(err));
	}
	err = pthread_join(tid_1, &thread_return_value);
	if (err != 0) {
		printf("Cannot join thread 1, %s \n", strerror(err));
	} else {
		printf("thread 1 exit code is %d\n", (int)thread_return_value);
	}
	exit(0);

}
