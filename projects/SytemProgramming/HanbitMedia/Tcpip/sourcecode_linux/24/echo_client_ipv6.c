#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXLINE 1024
int main(int argc, char **argv)
{
	struct sockaddr_in6 svrsin6;
	struct hostent *hp;
	char   ipv6_addr[16];

	char	addr6_str[40];
	char	buf[MAXLINE];

	int sockfd;
	int clilen;

	if(argc != 3)
	{
		printf("Usage : %s [ip] [portnum]\n", argv[0]);
		return 1;
	}
	sockfd = socket(AF_INET6, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		return 1;
	}

	memset((void *)&svrsin6, 0x00, sizeof(svrsin6));
	svrsin6.sin6_family   = AF_INET6;
	svrsin6.sin6_flowinfo = 0;
	svrsin6.sin6_port	 = htons(atoi(argv[2]));
	svrsin6.sin6_scope_id = if_nametoindex("eth0");

	if(inet_pton(AF_INET6, argv[1], (void *)&svrsin6.sin6_addr) == 0)
	{
		return 1;
	}

	clilen = sizeof(svrsin6);
	if(connect(sockfd, (struct sockaddr *)&svrsin6, clilen) < 0)
	{
		perror("connect error:");
		exit(0);
	}
	while(1)
	{
		memset(buf, 0x00, MAXLINE);
		read(0, buf, MAXLINE);
		write(sockfd, buf, MAXLINE);
		read(sockfd, buf, MAXLINE);
		printf("-->%s", buf);
	}
	close(sockfd);
	exit(0);
}
