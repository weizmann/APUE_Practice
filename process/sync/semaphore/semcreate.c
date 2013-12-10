/*
 * =====================================================================================
 *
 *       Filename:  semcreate.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013/12/10 16时02分39秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <semaphore.h>
#include <string.h>
#include <unistd.h>

#include "/usr/include/apue.h"

int main(int argc, char* argv[]) {
	int c, flags;
	sem_t *sem;
	unsigned int value;

	flags = O_RDWR | O_CREAT;
	value = 1;

	while ((c = getopt(argc, argv, "ei:")) != -1) {
		switch (c) {
		case 'e':
			flags |= O_CREAT;
			break;
		case 'i':
			value = atoi(optarg);
			break;
		}
	}

	if (optind != argc - 1) {
		printf("usage : semcreate [-e] [-i initvalue] <name>\n");
		return -1;
	}
	sem = sem_open(argv[optind], flags, FILE_MODE, value);
	sem_close(sem);
	exit(0);

}

