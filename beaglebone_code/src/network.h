#ifndef NETWORK_H_
#define NETWORK_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

typedef struct
{
	int socket_fd;
	int new_fd;
	struct addrinfo hints;
	struct addrinfo *p;
	struct sigaction handler;
	socklen_t socket_in_size;
	char s[INET6_ADDRSTRLEN];
} network_state_t;

#endif
