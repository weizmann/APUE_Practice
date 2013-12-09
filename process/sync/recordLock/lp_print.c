/*
 * =====================================================================================
 *
 *       Filename:  lp_print.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013/12/09 21时19分36秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "/usr/include/apue.h"

#define SEQFILE "seqno"

void my_lock(int), my_unlock(int);

int main(int argc, char*argv[]) {
	int fd;
	long i, seqno;
	pid_t pid;
	ssize_t n;
	char line[MAXLINE + 1];
	int result;

	pid = getpid();
	fd = open(SEQFILE, O_CREAT | O_RDWR, FILE_MODE);
	
	for (i = 0; i < 20; i++) {
		my_lock(fd);
		if ((result = lseek(fd, 0, SEEK_SET)) == -1) {
			printf("lseek error ...\n");
			return -1;
		}
		n = read(fd, line, MAXLINE);
		line[n] = '\0';

		n = sscanf(line, "%ld\n", &seqno);
		printf("%s: pid = %ld, seq# = %ld\n", argv[0], (long)pid, seqno);

		seqno++;
		snprintf(line, sizeof(line), "%ld\n", seqno);
		lseek(fd, 0L, SEEK_SET);
		write(fd, line, strlen(line));

		my_unlock(fd);
	}
}


void my_lock(int fd) {

	return;
}

void my_unlock(int fd) {
	return;
}



