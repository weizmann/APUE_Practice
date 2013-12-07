#include "/usr/include/apue.h"
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <time.h>


int server_listen(const char*name);
int server_accept(int listenfd, uid_t* uidptr);
int cli_conn(const char* name);
