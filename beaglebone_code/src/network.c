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
#include "network.h"

#define PORT 12345

void sigchld_handler(int s)
{
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    while(waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}

int network_init(network_state_t *state)
{
	int yes = 1;
	int rv;
	struct addrinfo *servinfo;
	memset(&(state -> hints), 0, sizeof(addrinfo));
	state -> hints.ai_family = AF_UNSPEC;
	state -> hints.ai_socktype = SOCK_STREAM;
	state -> hints.ai_flags = AI_PASSIVE;
	rv = getaddrinfo(NULL, PORT, &(state -> hints), &servinfo);
	if (rv != 0)
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}
	for (state -> p = servinfo; state -> p != NULL; state -> p = state -> p -> ai_next)
	{
		state -> socket_fd = socket(state -> p -> ai_family, state -> p -> ai_socktype, state -> p -> ai_protocol);
		if (state -> socket_fd == -1)
		{
			perror("server: socket");
			continue;
		}
		rv = setsockopt(state -> socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
		if (rv == -1)
		{
			perror("setsockopt");
			return 1;
		}
		rv = bind(state -> socket_fd, state -> p -> ai_addr, state -> p -> ai_addrlen);
		if (rv == -1)
		{
			close(state -> socket_fd);
			perror("server: bind");
			continue;
		}
		break;
	}
	freeaddrinfo(servinfo);
	if (state -> p == NULL)
	{
		fprintf(stderr, "server: failed to bind\n");
		return 1;
	}
	rv = listen(state -> socket_fd, 2);
	if (rv == -1)
	{
		perror("listen");
		return 1;
	}
	state -> handler.sa_handler = sigchld_handler;
	sigemptyset(&(state -> handler.sa_mask));
	state -> handler.sa_flags = SA_RESTART;
	rv = sigaction(SIGCHLD, &(state -> handler), NULL);
	if (rv == -1)
	{
		
		return 1;
	}	
	return 0;
}

int network_deinit(network_state_t *state)
{
	return 0;
}

int 
