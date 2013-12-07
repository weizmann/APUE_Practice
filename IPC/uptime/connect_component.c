/*
 * =====================================================================================
 *
 *       Filename:  connect_component.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013/12/07 11时54分44秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "connect_component.h"

#include "/usr/include/apue.h"
#include <sys/socket.h>
#include <errno.h>

#include <syslog.h>
#include <sys/resource.h>
#define <fcntl.h>

#define MAXSLEEP 128

int connect_retry(int sockfd, const struct sockaddr* addr, socklen_t alen) {
	int nsec;

	for (nsec = 1; nsec <= MAXSLEEP; nsec << = 1) {
		if (connect(sockfd, addr, alen) == 0) {
			return 0;
		}
		if (nsec <= MAXSLEEP / 2) {
			sleep(nsec);
		}
	}
	return -1;
}

int init_server(int type, const struct sockaddr* addr, socklen_t alen, int qlen) {
	int fd;
	int err = 0;
	if ((fd = socket(addr->sa_family, type, 0)) < 0) {
		return -1;
	}
	
	if (bind(fd, addr, alen) < 0) {
		err = errno;
		close(fd);
		return -1;
	}

	if (type == SOCK_STREAM || type == SOCK_SEQPACKET) {
		if (listen(fd, qlen) < 0) {
			err = errno;
			close(fd);
			return -1;
		}
	}

	return fd;
}

void daemonize(const char* cmd) {
	
}




















