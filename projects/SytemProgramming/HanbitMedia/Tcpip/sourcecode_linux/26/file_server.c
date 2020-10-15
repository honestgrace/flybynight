#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/signal.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

#include <time.h>

#include <sqlite3.h>

#define PORTNUM 3800
#define MAXLINE 1024

#define Q_UPLOAD	1
#define Q_DOWNLOAD  2
#define Q_LIST	  3

#define LOWER_BOUNDER  "========================================="
struct Cquery
{
	int command;
	char f_name[64];
};

int callback(void *not, int argc, char **argv, char **ColName);
int proc(int sockfd);
int serv_file_list();
int gettime(char *tstr);

int main(int argc, char **argv)
{
	struct sockaddr_in addr;
	int sockfd, cli_sockfd;
	int clilen;
	int pid;
	char tstr[36];
	if( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket error");
		return 1;
	}
	printf("Start\n");

	memset(&addr, 0x00, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(PORTNUM);
	if( bind (sockfd, (struct sockaddr *)&addr, sizeof(addr)) != 0)
	{
		perror("bind error");
		return 1;
	}

	if( listen(sockfd, 5) != 0)
	{
		perror("listen error");
		return 1;
	}
	signal(SIGCHLD, SIG_IGN);
	while(1)
	{
		clilen = sizeof(addr);
		cli_sockfd = accept(sockfd, NULL, &clilen);
		if(cli_sockfd < 0) exit(0);
		pid = fork();
		if(pid == 0)
		{
			proc(cli_sockfd);
			printf("close socket\n");
			sleep(5);
			close(cli_sockfd);
			exit(0);
		}
		else if(pid == -1)
		{
			printf("fork error\n");
			exit(0);
		}
		else
		{
			close(cli_sockfd);
		}
	}
}

int proc(int sockfd)
{
	char buff[MAXLINE];
	struct Cquery query;
	printf("OK Server\n");
	while(1)
	{
		if(recv(sockfd, (void *)&query, sizeof(query), 0) <= 0)
		{
			return -1;
		}
		printf("read ok\n");
		query.command = ntohl(query.command);
		switch(query.command)
		{
			case (Q_UPLOAD):
				serv_file_upload(sockfd, query.f_name);
			break;
			case (Q_DOWNLOAD):
				serv_file_download(sockfd, query.f_name);
			break;
			case (Q_LIST):
				serv_file_list(sockfd);
			break;
		}
		break;
	}
	return 1;
}   

int serv_file_upload(int sockfd, char *filename)
{
	int fd;
	int readn;
	int writen;
	char buf[MAXLINE];
	struct sockaddr_in addr;
	int addrlen;
	sqlite3 *db;
	int ret;

	if( (fd = open(filename, O_WRONLY)) == -1)
	{
		return -1;
	}

	memset(buf, 0x00, MAXLINE);
	while((readn = recv(sockfd, buf, MAXLINE, 0)) > 0)
	{
		writen = write(fd, buf, readn);
		if(writen < 0) break;
		memset(buf, 0x00, MAXLINE);
	}

	ret = sqlite3_open("count.db", &db);
	if(ret != 0)
	{
		return -1;
	}
	addrlen = sizeof(addr);
	getpeername(sockfd, (struct sockaddr *)&addr, &addrlen);
	printf("File Upload %s\n", inet_ntoa(addr.sin_addr));

	return 1;
}

int serv_file_download(int sockfd, char *filename)
{
	int fd;
	int readn;
	int sendn;
	char buf[MAXLINE];
	if( (fd = open(filename, O_RDONLY)) == -1)
	{
		return -1;
	}

	memset(buf, 0x00, MAXLINE);
	while( (readn = read(fd, buf, MAXLINE)) > 0)
	{
		sendn = send(sockfd, buf, readn, 0);
		if(sendn < 0) break;
		memset(buf, 0x00, MAXLINE);
	}
	if (sendn == -1) return -1;

	return 1;
}

int callback(void *farg, int argc, char **argv, char **ColName)
{
	int i;
	int sockfd = *(int *)farg;
	int sendn = 0;
	char buf[MAXLINE];
	for(i = 0; i < argc; i++)
	{
		sprintf(buf, "%10s = %s\n", ColName[i], argv[i] ? argv[i] : "NULL");
		sendn = send(sockfd, buf, strlen(buf), 0);
		printf("--> %d\n", sendn);
	}
	sprintf(buf, "%s\n", LOWER_BOUNDER);
	sendn = send(sockfd, buf, strlen(buf), 0);
	printf("--> %d\n", sendn);
	return 0;
}

int gettime(char *tstr)
{
	struct tm *tm_ptr;
	time_t the_time;

	time(&the_time);
	tm_ptr = localtime(&the_time);

	sprintf(tstr, "%d/%d/%d %d:%d:00",
		tm_ptr->tm_year + 1900,
		tm_ptr->tm_mon + 1,
		tm_ptr->tm_mday,
		tm_ptr->tm_hour,
		tm_ptr->tm_min
		);
}

int serv_file_list(int sockfd)
{
	char *ErrMsg;
	int ret;
	sqlite3 *db;

	ret = sqlite3_open("count.db", &db);
	if(ret != 0)
	{
		printf("Database Open Error\n");
		return -1;
	}
	printf("Database Open Success\n");
	ret= sqlite3_exec(db,
			"select * from file_info",
			callback, (void *)&sockfd, &ErrMsg);
	if(ret != 0)
	{
		printf("Error %s\n", ErrMsg);
		return 1;
	}
	sqlite3_close(db);
	return 1;
}
