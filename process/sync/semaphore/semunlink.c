/*
 * =====================================================================================
 *
 *       Filename:  semunlink.c
 *
 *    Description:  G=
 *
 *        Version:  1.0
 *        Created:  2013/12/10 16时14分29秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "/usr/include/apue.h"
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <string.h>

int main(int argc, char** argv) {
	if (argc != 2) {
		printf("usage : semunlink <name>\n");
		return -1;
	}
	sem_unlink(argv[1]);
	return 0;
}
