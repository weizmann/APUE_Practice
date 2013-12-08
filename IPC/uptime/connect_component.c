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
#include <string.h> 
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

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
	int i, fd0, fd1, fd2;
	pid_t pid;
	struct rlimit rl;
	struct sigaction sa;

	umask(0);

	if (getrlimit(RLIMIT_NOFILE, &rl) < 0) {
		printf("error when getting resource limit....\n");
		exit(-1);
	}

	if ((pid = fork()) < 0) {
		printf("cannot fork ....\n");
		exit(-1);
	} else if (pid != 0) {
		exit(0); // parent exit.
	}

	setsid();

	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGHUP, &sa, NULL) < 0) {
		printf("cannot ignore SIGHUP....\n");
		exit(-1);
	}
	if ((pid = fork()) < 0) {
		printf("error forking ...\n");
		exit(-1);
	} else if (pid != 0) {
		exit(0);
	}
	
	if (chdir("/") < 0) {
		printf("change directory error \n");
		exit(-1);
	} 

	if (rl.rlim_max == RLIM_INFINITY) {
		rl.rlim_max = 1024;
	} 
	for (i = 0; i < rl.rlim_max; i++) {
		close(i);
	}

	fd0 = open("/dev/null", O_RDWR);
	fd1 = dup(0);
	fd2 = dup(0);

	openlog(cmd, LOG_CONS, LOG_DAEMON);
	if (fd0 != 0 || fd1 != 1 || fd2 != 2) {
		syslog(LOG_ERR, "unexpected file descriptions %d, %d, %d\n", fd0, fd1, fd2);
		exit(-1);
	}

}

int lockfile(int fd) {
	struct flock fl;

	fl.l_type = F_WRLCK;
	fl.l_start = 0;
	fl.l_whence = SEEK_SET;
	fl.l_len = 0;

	return (fcntl(fd, F_SETLK, &fl));
}

#define LOCKFILE "/var/run/daemon.pid"
#define LOCKMODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) 

int already_running() {
	int fd;
	char buf[16];

	fd = open(LOCKFILE, O_RDWR | O_CREAT, LOCKMODE);
	if (fd < 0) {
		printf("open error in already running...\n");
		exit(-1);
	}

	if (lockfile(fd) < 0) {
		if (errno == EACCES || errno == EAGAIN) {
			close(fd);
			return 1;
		}
	}

	ftruncate(fd, 0);
	sprintf(buf, "%ld", (long)getpid());
	write(fd, buf, strlen(buf) + 1);
	return 0;
}
















