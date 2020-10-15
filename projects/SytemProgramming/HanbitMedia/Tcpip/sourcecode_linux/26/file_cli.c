#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

#include <unistd.h>
#include <getopt.h>

#define PORTNUM 3800
#define MAXLINE 1024

#define Q_UPLOAD	1
#define Q_DOWNLOAD  2
#define Q_LIST	  3

struct Cquery
{
	int command;
	char fname[64];
};

void help(char *progname)
{
	printf("Usage : %s -h -i [ip] -u [upload filename] -d [download filename] -l\n", progname);
}

int main(int argc, char **argv)
{
	struct sockaddr_in addr;
	int sockfd;
	int clilen;
	int opt;
	int optflag=0;
	char ipaddr[36]={0x00,};
	int command_type=0;
	char fname[80]={0x00,};
	char buf[MAXLINE];

	while( (opt = getopt(argc, argv, "hli:u:d:")) != -1)
	{
		switch(opt)
		{
			case 'h':
				help(argv[0]);
				return 1;

			case 'i':
				sprintf(ipaddr, "%s", optarg);
				break;

			case 'u':
				command_type = Q_UPLOAD;
				sprintf(fname, "%s", optarg);
				optflag = 1;
				break;

			case 'd':
				command_type = Q_DOWNLOAD;
				sprintf(fname, "%s", optarg);
				optflag = 1;
				break;

			case 'l':
				command_type = Q_LIST;
				break;

			default:
				help(argv[0]);
				return 1;
		}
	}

	if(ipaddr[0] == '\0')
	{
		printf ("ip address not setting\n");
		return 0;
	}

	if((fname[0] == '\0') && (optflag == 1))
	{
		printf ("fname error\n");
		return 0;
	}

	if( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
	{
		perror("socket error");
		return 1;
	}   
	
	memset((void *)&addr, 0x00, sizeof(addr));
	addr.sin_family  = AF_INET; 
	addr.sin_addr.s_addr = inet_addr(ipaddr);
	addr.sin_port = htons(PORTNUM);
	
	clilen = sizeof(addr);
	if(connect(sockfd, (struct sockaddr *)&addr, clilen) < 0)
	{
		perror("connect error:");
		return 0;
	}   
	
	while(1)
	{
		switch(command_type)
		{
			case (Q_LIST):
				get_list(sockfd);
				break;
			break;
			case (Q_DOWNLOAD):
				download(sockfd, fname);
			break;
			case (Q_UPLOAD):
				upload(sockfd, fname);
			break;
			default:
				printf("Unknown command\n");
			break;
		}   
		break;
	}   
	close(sockfd);
}

int download(int sockfd, char *file)
{
	struct Cquery query;
	int fd;
	int readn, writen;
	char buf[MAXLINE];
	if( (fd = open(file, O_WRONLY)) == -1 )
	{
		return -1;
	}
	memset(buf, 0x00, MAXLINE);
	memset(&query, 0x00, sizeof(query));
	query.command = htonl(Q_DOWNLOAD);
	sprintf(query.fname, "%s", file);
	if(send(sockfd, (void *)&query, sizeof(query), 0) <=0)
	{
		return -1;
	}

	while((readn = recv(sockfd, buf, MAXLINE, 0)) > 0)
	{
		writen = write(fd, buf, readn);
		if(writen != readn)
		{
			return -1;
		}
		memset(buf, 0x00, MAXLINE);
	}
	close(fd);
	return 1;
}

int get_list(int sockfd)
{
	struct Cquery query;
	char buf[MAXLINE];
	int len;
	memset(&query, 0x00, sizeof(query));
	query.command = htonl(Q_LIST);

	if(send(sockfd, (void *)&query, sizeof(query), 0) <=0 )
	{
		perror("Send Error\n");
		return -1;
	}
	memset(buf, 0x00, MAXLINE);
	while(1)
	{
		len = recv(sockfd, buf, MAXLINE, 0);
		if(len <= 0) break;
		printf("%s", buf);
		memset(buf, 0x00, MAXLINE);
	}
	printf("End!\n");
}

int upload(int sockfd, char *file)
{
	struct Cquery query;
	int fd;
	int readn;
	int sendn;
	char buf[MAXLINE];

	if( (fd = open(file, O_RDONLY)) == -1 )
	{
		return -1;
	}

	memset(&query, 0x00, sizeof(query));
	query.command = htonl(Q_UPLOAD);
	sprintf(query.fname, "%s", file);

	if(send(sockfd, (void *)&query, sizeof(query), 0) <=0)
	{
		return -1;
	}

	while((readn = read(fd, buf, MAXLINE)) > 0)
	{
		sendn = send(sockfd, buf, readn, 0);
		if(sendn != readn)
		{
			printf("Upload Error\n");
			return -1;
		}
	}
	close(fd);
	return 1;
}
