#ifndef CONN_COMPONENT
#define CONN_COMPONENT

int connect_retry(int, const struct sockaddr*, socklen_t);
int init_server(int, struct sockaddr*, socklen_t, int);

void daemonize(const char* cmd);

#endif
