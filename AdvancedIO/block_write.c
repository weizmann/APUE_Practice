/*
 * =====================================================================================
 *
 *       Filename:  block_write.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013/12/07 20时43分05秒
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
#include <errno.h>
#include <stdio.h>

void set_flag(int fd, int flags, int is_clear) {
	int val;
	if ((val = fcntl(fd, F_GETFL, 0)) < 0) {
		printf("function get fcntl error...\n");
		return;
	}

	if (is_clear) {
		val &= ~flags;
	} else {
		val |= flags;
	}

	if ((val = fcntl(fd, F_SETFL, val)) < 0) {
		printf("function set fcntl error...\n");
	}
}

void set_fl(int fd, int flags) {
	set_flag(fd, flags, 1);
}

void clear_fl(int fd, int flags) {
	set_flag(fd, flags, 0);
}

#define BUFLEN 500000000
char buf[BUFLEN];

int main() {
	int need_to_write, had_written;
	char* ptr;
	memset(buf, 1, sizeof(buf));
	const char* file_name = "test_file.tmp";
	int fd = open(file_name, /* O_EXCL | */O_CREAT | O_RDWR);
	if (fd < 0) {
		printf("file already exists....\n");
	} else {
		int total_size = (int)sizeof(buf);
		char* p = buf;
		while (total_size > 0) {
			int write_bytes = write(fd, buf, total_size);
			if (write_bytes > 0) {
				total_size -= write_bytes;
				printf("%d bytes have been written into the file \n", write_bytes);
			}
		}
		printf("write to file %d bytes \n", (int)sizeof(buf));
		close(fd);
	}

	need_to_write = read(STDIN_FILENO, buf, sizeof(buf));
	fprintf(stderr, "read %d bytes\n", need_to_write);


	set_fl(STDOUT_FILENO, O_NONBLOCK);
	ptr = buf;

	while(need_to_write > 0) {
		errno = 0;
		had_written = write(STDOUT_FILENO, ptr, need_to_write);
		fprintf(stderr, "had_written is %d, errno = %d\n", had_written, errno);

		if (had_written > 0) {
			ptr += had_written;
			need_to_write -= had_written;
		}
	}
	
	clear_fl(STDOUT_FILENO, O_NONBLOCK);
	return 0;
		
}











