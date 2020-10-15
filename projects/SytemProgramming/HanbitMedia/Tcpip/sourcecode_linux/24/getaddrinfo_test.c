#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>


int main(int argc, char **argv)
{
	struct addrinfo hints;
	struct addrinfo *result, *rp;
	struct sockaddr_in *sin;
	struct sockaddr_in6 *sin6;
	int *listen_fd;
	int listen_fd_num=0;

	char buf[80] = {0x00,};
	int i = 0;
	if(argc != 2)
	{
		printf("Usage : %s [port]\n", argv[0]);
		return 1;
	}

	memset(&hints, 0x00, sizeof(struct addrinfo));

	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if(getaddrinfo(NULL, argv[1], &hints, &result) != 0 )
	{
		perror("getaddrinfo");
		return 1;
	}

	for(rp = result ; rp != NULL; rp = rp->ai_next)
	{
		listen_fd_num++;
	}
	listen_fd = malloc(sizeof(int)*listen_fd_num);

	for(rp = result, i=0 ; rp != NULL; rp = rp->ai_next, i++)
	{
		if(rp->ai_family == AF_INET)
		{
			sin = (void *)rp->ai_addr;
			inet_ntop(rp->ai_family, &sin->sin_addr, buf, sizeof(buf));
			printf("<bind 정보 %d %d %s>\n", rp->ai_protocol, rp->ai_socktype, buf);
		}
		else
		{
			sin6 = (void *)rp->ai_addr;
			inet_ntop(rp->ai_family, &sin6->sin6_addr, buf, sizeof(buf));
			printf("<bind 정보 %d %d %s>\n", rp->ai_protocol, rp->ai_socktype, buf);
		}
		if((listen_fd[i] = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol)) < 0)
		{
			printf("Socket Create Error\n");
		}
		if(rp->ai_family == AF_INET6)
		{
			int opt = 1;
			setsockopt(listen_fd[i], IPPROTO_IPV6, IPV6_V6ONLY, (char *)&opt, sizeof(opt));
		}

		if(bind(listen_fd[i], rp->ai_addr, rp->ai_addrlen) != 0)
		{
			if(errno != EADDRINUSE);
			{
				perror("bind error\n");
				return 1;
			}
		}
		if(listen(listen_fd[i], 5) != 0)
		{
			perror("listen error\n");
			return 1;
		}
	}
	freeaddrinfo(result);
	pause();
	return 0;
}

