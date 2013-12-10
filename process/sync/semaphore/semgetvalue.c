/*
 * =====================================================================================
 *
 *       Filename:  semgetvalue.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013/12/10 16时16分41秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "/usr/include/apue.h"

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <string.h>

int main(int argc, char** argv) {
	sem_t* sem;
	int val;

	if (argc != 2) {
		printf("usage : semgetvalue <name>\n");
		return -1;
	}

	sem = sem_open(argv[1], 0);
	sem_getvalue(sem, &val);
	printf("value is %d\n", val);

	return 0;
}

