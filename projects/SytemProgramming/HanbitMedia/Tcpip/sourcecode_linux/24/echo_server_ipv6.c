#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE 1024
int main(int argc, char **argv)
{
	struct sockaddr_in6 sin6;
	struct sockaddr_storage cliaddr;
	int sockfd, clisockfd;
	socklen_t addr_size = sizeof(cliaddr);
	char buf[MAXLINE];

	if(argc != 2)
	{
		printf("Usage : %s [portnum]\n", argv[0]);
	}

	sockfd = socket(AF_INET6, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		perror("socket create error:");
		return 1;
	}

	sin6.sin6_family   = AF_INET6;
	sin6.sin6_flowinfo = 0;
	sin6.sin6_port	 = htons(atoi(argv[1]));
	sin6.sin6_addr	 = in6addr_any;

	if (bind(sockfd, (struct sockaddr *)&sin6, sizeof(sin6)) == -1)
	{
		return;
	}

	if (listen(sockfd, 5) == -1)
	{
		return 1;
	}

	while(1)
	{
		clisockfd = accept(sockfd, (struct sockaddr *)&cliaddr,
					   (socklen_t *)&addr_size);
		printf("IP VERSION %d %d\n", AF_INET6, cliaddr.ss_family);
		while(1)
		{
			memset(buf, 0x00, MAXLINE);
			if(read(clisockfd, buf, MAXLINE) <=0)
				break;
			if(write(clisockfd, buf, MAXLINE) <= 0)
				break;
		}
	}
	close(clisockfd);
}

