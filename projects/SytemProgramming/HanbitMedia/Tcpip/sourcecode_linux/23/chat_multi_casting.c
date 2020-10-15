#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#define MAXLINE 1024

char *msg_exit ="exit\n";

int main(int argc, char **argv)
{
	int read_fd;
	int send_fd;
	pid_t pid;
	int flag;
	struct sockaddr_in from;
	int addrlen;

	struct sockaddr_in mcast_group;
	struct ip_mreq mreq;

	char name[16];

	char message[MAXLINE];

	if(argc != 4)
	{
		printf("Usage : %s \n", argv[0]);
		return 1;
	}

	sprintf(name, "%s", argv[3]);
	memset(&mcast_group, 0x00, sizeof(mcast_group));

	mcast_group.sin_family = AF_INET;
	mcast_group.sin_port = htons(atoi(argv[2]));
	inet_pton(AF_INET, argv[1], &mcast_group.sin_addr);

	if( read_fd = socket(AF_INET, SOCK_DGRAM, 0) < 0 )
	{
		return 1;
	}

	mreq.imr_multiaddr = mcast_group.sin_addr;
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);

	if(setsockopt(read_fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq))<0)
	{
		printf("error : add group\n");
		return 1;
	}
	flag = 1;
	if(setsockopt(read_fd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag)) < 0)
	{
		printf("Socket Reuseaddr Error\n");
		return 1;
	}

	if(bind(read_fd, (struct sockaddr *)&mcast_group, sizeof(mcast_group)) < 0)
	{
		return 1;
	}

	if( (send_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 )
	{
		return 1;
	}

	pid = fork();
	if(pid < 0)
	{
		printf("fork error\n");
		return 1;
	}
	else if(pid == 0)
	{
		while(1)
		{
			addrlen = sizeof(from);
			memset(message, 0x00, MAXLINE);
			if(recvfrom(read_fd, message, MAXLINE, 0, (struct sockaddr *)&from, &addrlen))
			{
				printf("error : recvfrom\n");
					return 1;
			}
			printf("%s", message);
			if(strstr(message, msg_exit) != NULL)
			{
				printf("EXIT\n");
				if(setsockopt(read_fd, IPPROTO_IP, IP_DROP_MEMBERSHIP, &mreq, sizeof(mreq))	 < 0)
				{
					return 1;
				}
			}
		}
	}
	else
	{
		while(1)
		{
			memset(message, 0x00, MAXLINE);
			read(0, message, MAXLINE);
			sprintf(message, "%s : %s", name, message);
			if(sendto(send_fd, message, strlen(message), 0, (struct sockaddr *)&mcast_group, sizeof(mcast_group)) < strlen(message))
			{
				return 1;
			}
		}
	}
	return 0;
}
