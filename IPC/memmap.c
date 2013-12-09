/*
 * =====================================================================================
 *
 *       Filename:  memmap.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013/12/08 13时58分53秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "/usr/include/apue.h"
#include <fcntl.h>
#include <sys/mman.h>

void printf_error_msg(const char* msg) {
	printf("error message is : %s\n", msg);
	exit(-1);
}


int main(int argc, char*argv[]) {
	int fdin, fdout;
	void *src, *dst;
	struct stat statbuf;

	umask(0);
	if (argc != 3) {
		printf("usage : %s <fromfile> <tofile>\n", argv[0]);
		return -1;
	}

	if ((fdin = open(argv[1], O_RDONLY)) < 0) {
		printf_error_msg("open fdin error..");	
	}

	if ((fdout = open(argv[2], O_RDWR | O_CREAT | O_TRUNC), FILE_MODE) < 0) {
		printf_error_msg("open fdout error..");
	}

	if (fstat(fdin, &statbuf) < 0) {
		printf_error_msg("stat error to fdin");
	}

	printf("statbuf.st_size is %lld\n", statbuf.st_size);
	if (lseek(fdout, statbuf.st_size - 1, SEEK_SET) == -1) {
		printf_error_msg("lseek error...");
	}
	if (write(fdout, "", 1) != 1) {
		printf_error_msg("write to fdout error");
	}

	if ((src = mmap(0, statbuf.st_size, PROT_READ, MAP_SHARED, fdin, 0)) == MAP_FAILED) {
		printf_error_msg("mmap src error...");
	} 
	if ((dst = mmap(0, statbuf.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fdout, 0)) == MAP_FAILED) {
		printf_error_msg("mmap dst error...");
	}

	memcpy(dst, src, statbuf.st_size);
	exit(0);
}


































