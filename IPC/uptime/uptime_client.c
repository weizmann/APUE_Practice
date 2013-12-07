/*
 * =====================================================================================
 *
 *       Filename:  uptime_client.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013/12/07 11时50分34秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "/usr/include/apue.h"

#include <netdb.h>
#include <errno.h>
#include <sys/socket.h>

#define MAXADDRLEN 256
#define BUFLEN 128

#define SERVICE_NAME "ruptime"

void print_uptime(int sockfd) {
	int n;
	char buf[BUFLEN];
	while ((n = recv(sockfd, buf, BUFLEN, 0)) > 0) {
		write(STDOUT_FILENO, buf, n);
	}
	if (n < 0) {
		printf("recv error in client program....\n");
	}
}

int main(int argc, char* argv[]) {
	struct addrinfo* ailist, *aip;
	struct addrinfo hint;
	int sockfd, err;

	if (argc != 2) {
		print("usage: ruptime hostname\n");
		return -1;
	}

	hint.ai_flag = 0;
	hint.ai_family = 0;
	hint.ai_socktype = SOCK_STREAM;
	hint.ai_protocol = 0;
	hint.ai_addrlen = 0;
	hint.ai_canonname = NULL;
	hint.ai_addr = NULL;
	hint.ai_next = NULL;

	if ((err = getaddrinfo(argv[1], SERVICE_NAME, &hint, &ailist)) != 0) {
		printf("got an error in getaddrinfo \n");
		return -1;
	}

	for (aip = ailist; aip != NULL; aip = aip->ai_next) {
		if ((sockfd = socket(aip->ai_family, SOCK_STREAM, 0)) < 0) {
			printf("got an error in socket init....\n");
			return -1;
		}

		if (connect_retry(sockfd, aip->ai_addr, aip->ai_addrlen) < 0) {
			err = errno;
			return -1;
		} else {
			print_uptime(sockfd);
			exit(0);
		}
	}
	
	printf("Cannot connect to %s:%d\n", argv[1], errno);
	exit(-1);

}































