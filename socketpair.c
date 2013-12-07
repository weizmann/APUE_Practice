/*
 * =====================================================================================
 *
 *       Filename:  socketpair.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013/12/02 23时20分21秒
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
#include <string.h>

#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>

#include <sys/socket.h>

#define MY_LOGO "_ZHENG_WEI_"

static int send_fd(int fd, int fd_to_send) {
	struct iovec iov[1];
	struct msghdr msg;
	char buf[1];

	if (fd_to_send >= 0) {
		msg.msg_control = (caddr_t)&fd_to_send;
		msg.msg_controllen = sizeof(int);
	} else {
		msg.msg_control = (caddr_t)NULL;
		msg.msg_controllen = 0;
	}
	
	msg.msg_name = NULL;
	msg.msg_namelen = 0;

	iov[0].iov_base = buf;
	iov[0].iov_len = 1;

	msg.msg_iov = iov;
	msg.msg_iovlen = 1;

	if (sendmsg(fd, &msg, 0) < 0) {
		printf("send message error [errno] = %d\n", errno);
		return errno;
	}
	return 0;
}

static int recv_fd(int fd, int*fd_to_recv) {
	struct iovec iov[1];
	struct msghdr msg;
	char buf[1];

	msg.msg_control = (caddr_t)fd_to_recv;
	msg.msg_controllen = sizeof(int);

	msg.msg_name = NULL;
	msg.msg_namelen = 0;

	iov[0].iov_base = buf;
	iov[0].iov_len = 1;
	msg.msg_iov = iov;
	msg.msg_iovlen = 1;

	if (recvmsg(fd, &msg, 0) < 0) {
		return errno;
	}
	if (msg.msg_controllen != sizeof(int)) {
		*fd_to_recv = -1;
	}
	return 0;
}

int x_sock_set_block(int sock, int on) {
	int val;
	int rv;

	val = fcntl(sock, F_SETFL, 0);
	if (on) {
		rv = fcntl(sock, F_SETFL, ~O_NONBLOCK & val);
	} else {
		rv = fcntl(sock, F_SETFL, O_NONBLOCK | val);
	}

	if (rv) {
		return errno;
	}
	return rv;
}
	


int main() {
	pid_t pid;
	int sockpair[2];
	int rv;
	char fname[256];
	int fd;

	rv = socketpair(AF_UNIX, SOCK_STREAM, 0, sockpair);
	if (rv < 0) {
		printf("Call socketpair error, errno is %d\n", errno);
		return errno;
	}

	pid = fork();
	if (pid == 0) {
		close(sockpair[1]);
		for(;;) {
			rv = x_sock_set_block(sockpair[0], 1);
			if (rv != 0) {
				printf("[CHILD]: recv_fd error, errno is %d\n", rv);
				break;
			}

			rv = recv_fd(sockpair[0], &fd);
			if (rv < 0) {
				printf("[CHILD]: recv error, errno is %d\n", rv);
				break;
			}

			if (fd < 0) {
				printf("[CHILD]: child process exit normally!\n");
				break;
			}

			rv = write(fd, MY_LOGO, strlen(MY_LOGO));
			if (rv < 0) {
				printf("[CHILD]: write error, errno is %d\n", errno);
			} else {
				printf("[CHILD]: append logo successfully~~\n");
			}
			close(fd);
		}
		exit(0);
	} else if (pid > 0) {
		for(;;) {
			memset(fname, 0, sizeof(fname));
			printf("[PARENT]: please enter your file name:\n");
			scanf("%s", fname);

			if (strcmp(fname, "exit" == 0)) {
				rv = send_fd(sockpair[1], -1);
				if (rv < 0) {
					printf("[PARENT]: send_fd_error");
				}
				break;
			}

			fd = open(fname, O_RDWR | O_APPEND);
			if (fd < 0) {
				if (errno == ENOENT) {
					printf("[PARENT]: cannot find file %s\n", fname);
					continue;
				}
				printf("[PARENT]:open file error....\n");
			}

			rv = send_fd(sockpair[1], fd);
			if (rv != 0) {
				printf("[]PARENT: send_fd error, errno is %d\n", rv);
			}
			close(fd);
		}
		wait(NULL);
		return 0;
	}

}































