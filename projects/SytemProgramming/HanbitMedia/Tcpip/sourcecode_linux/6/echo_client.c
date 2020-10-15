#include <unistd.h>
#include <sys/stat.h>	 /* 소켓 프로그래밍에 필요한 헤더파일 include */
#include <arpa/inet.h>
#include <stdio.h>			/* 표준 입출력 관련 */
#include <string.h>		 /* 문자열 함수 지원 */

#define MAXLINE		1024

int main(int argc, char **argv)
{
	 struct sockaddr_in serveraddr;
	 int server_sockfd;
	 int client_len;
	 char buf[MAXLINE];
	 char rbuf[MAXLINE];

	 if ((server_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) /* 소켓 생성 */
	 {
			 perror("error :");
			 return 1;
	 }
	 /* 연결할 서버의 주소와 IP 사용할 프로토콜을 명시한다. */
	 server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	 serveraddr.sin_family = AF_INET;
	 serveraddr.sin_addr.s_addr = inet_addr("127.0.01");
	 serveraddr.sin_port = 3500;

	 client_len = sizeof(serveraddr);
	 /* 서버에 연결을 시도 한다. */
	 if (connect(server_sockfd, (struct sockaddr *)&serveraddr, client_len) < 0)
	 {
			 perror("connect error :");
			 return 1;
	 }

	 memset(buf, 0x00, MAXLINE);
	 read(stdin, buf, MAXLINE);		/* 키보드로부터 데이터를 입력 받는다. */
	 if (write(server_sockfd, buf, MAXLINE) <= 0) /* 입력받은 데이터를 소켓에 쓴다. */
	 {
			 perror("write error : ");
			 return 1;
	 }
	 memset(buf, 0x00, MAXLINE);
	 if (read(server_sockfd, buf, MAXLINE) <= 0) /* 서버로부터 데이터를 읽는다. */
	 {														 /* 제대로 동작한다면 입력받은 데이터가 그대로 출력될 것이다. */
			 perror("read error : ");
			 return 1;
	 }
	 close(server_sockfd);
	 printf("read : %s", buf);
	 return 0;
}
