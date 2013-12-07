#include "/usr/include/apue.h"
#include <stdlib.h>

void wait_child() {
	sleep(1000);
}

int main() {
	pid_t pid;
	if ((pid = fork()) == 0) {
		// child process
		if ((pid = fork()) == 0) {
			// child process do some thing
		}
	} else if (pid > 0) { 
		wait();
	}
}
