#include <fcntl.h>
#include <stdio.h>

int main() {
	char* pathname = "./1014.c";
	int fd = open(pathname, O_RDWR | O_CREAT | O_EXCL);
	if (fd > 0) {
		printf("open %s correctly fd is %d\n", pathname, fd);
	} else {
		printf("cannot open file....\n");
	}
}
