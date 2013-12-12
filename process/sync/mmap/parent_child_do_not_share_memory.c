/*
 * =====================================================================================
 *
 *       Filename:  parent_child_do_not_share_memory.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013/12/12 22时56分12秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>

#include "/usr/include/apue.h"

#define SEM_NAME "mysem"

int count = 0; // DO not shared by parent and child

int main(int argc, char* argv[])  {
	int i, nloop;
	sem_t* mutex;

	if (argc != 2) {
		printf("usage: filename <loops>\n");
		exit(-1);
	}
	nloop = atoi(argv[1]);
	mutex = sem_open(SEM_NAME, O_CREAT | O_EXCL, FILE_MODE, 1);
	sem_unlink(SEM_NAME);

	setbuf(stdout, NULL);
	if (fork() == 0) {
		for (i = 0; i < nloop; i++) {
			sem_wait(mutex);
			printf("child %d\n", count++);
			sem_post(mutex);
		}
		exit(0);
	} 
	for (i = 0; i < nloop; i++) {
		sem_wait(mutex);
		printf("parent %d\n", count++);
		sem_post(mutex);
	}
	exit(0);
}

