/*
 * =====================================================================================
 *
 *       Filename:  unix_domain_ipc.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013/12/06 23时34分15秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "unix_domain_ipc.h"

#define QLEN 10;

int server_listen(const char* name) {
	int fd, len, err, rval;
	struct sockarr_un un;

	if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
		return -1;
	}

	unlink(name);

	memset(&un, 0, sizeof(un));
	un.sun_family = AF_UNIX;
	strcpy(un.sun_path, name);
	len = offsetof(struct sockaddr_un, sun_path) + strlen(name);

	if (bind(fd, (struct sockaddr*)&un, len) < 0) {
		rval = -2;
		goto errout;
	}

	if (listen(fd, QLEN) < 0) {
		rval = -3;
		goto errout;
	}
	return fd;

errout:
	err = errno;
	close(fd);
	errno = err;
	return (rval);
}

int server_accept(int listenfd, uid_t *uidptr) {
	int clifd, len, err, rval;
	time_t staletime;
	struct sockaddr_un un;
	struct stat statbuf;

	len = sizeof(un);

}










































