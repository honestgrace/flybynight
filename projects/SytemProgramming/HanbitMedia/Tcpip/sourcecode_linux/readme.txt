공통 : 
모든 <코드>영역에서 문자열을 감싸는 함수들(printf계열, strcmp, strstr등 str계열 함수)에서 '를 "로 변경해야함. 리눅스 소스코드를 워드로 copy&paste 하면서 "이 '로 변경되어 버린 것 같음 

5장
stdin_out.c 에서
#include <unistd.h>  // 헤더파일 추가 (현재 경고 메시지 발생)
#include <string.h>   // 헤더파일 추가 

printf('Input Your Message : \n'); ---> "로 바뀌어야함
printf('Your Message is : \n'); ---> "로 바뀌어야함

set_resource.c에서
형변환 경고
  printf("Open file %d : %d\n", (int)rlim.rlim_cur, (int)rlim.rlim_max); 
  printf("Open file %d : %d\n", (int)rlim.rlim_cur, (int)rlim.rlim_max); 
main 함수에 반환값 없는 경우
  return 0; // main 함수 마지막에 추가

mycopy.c에서
#include <unsitd.h>  // 헤더파일 추가 (현재 경고 메시지 발생)

6장. 
check_endian.c
함수가 잘못 되었음.
<코드>
#include <stdio.h>

#define BIG 0
#define LITTLE 1

int check_endian(void);

int main(int argc, char **argv)
{
        int endian;
        endian = check_endian();
        if (endian == LITTLE)
                printf("little endian\n");
        else
                printf("big endian\n");
        return 0;
}

int check_endian(void)
{
        int i = 0x00000001;
        return ((char *)&i)[0]?LITTLE:BIG;
}
</코드>

echo_client.c에서

#include <unistd.h>    // read함수 호출에서 경고 발생함. 헤더파일 포함
      ...
     /* 서버에 연결을 시도 한다. */
     if (connect(server_sockfd, (struct sockaddr *)&serveraddr, client_len) < 0)
     {
             perror("connect error :");
             reutnr 1;            // return 으로 바꿔야함.
     }
     ...
     read(STDIN, buf, MAXLINE);     // STDIN을 stdin으로 바꿔야함.


7장.

check_class.c
...
int main(int argc, char **argv)
{
	char buf[256];    // 사용하지 않는 변수. 경고 발생. 삭제
        ...
}

inet_addr.c
모든 ' 를 "로 변경해야 함. 
...
int main (int argc, char **argv)
{
   char buf[MAXLINE];        // 사용하지 않는 변수, 경고 발생. 삭제
   ...
}

namelookup.c

if(myent == NULL)
{
    perror('ERROR ');    // '를 "로 변경해야 함.
}

8장. 
23 페이지
<여기서 잠깐>
컴파일에서 발생하는 경고(warning) 메시지 처리
gcc에 -Wall 옵션을 주고 컴파일하면 경고 메시지를 확인할 수 있다. -Wall 옵션을 주고 위 프로그램을 컴파일 해보자. 다음과 같은 경고를 확인할 수 있을 것이다. 
<코드>
# gcc -Wall -o cal cal.c          // gcc -Wall -o cal_server cal_server.c로 변경

9장.
cal_linux_cli.c
  close(sockfd);    // ;이 빠졌음. 

11장.
첫번째 예제, 예제 이름이 없음. 예제이름은 pipe.c로

공유메모리 예제(24페이지), 예제 이름이 없음. 예제 이름은 sharedmem.c 로

17장.
채팅 클라이언트의 이름은 chat_cli.c로 ??장에 소개되었다.
채팅 서버 예제 이름이 없음. chat_server_epoll.c로 

24장.
echo_server_ipv6.c
#include <stdio.h>   // 헤더파일 포함, 경고 메시지 발생함.

echo_client_ipv6.c
main 함수의 } 가 빠졌음.

26장.
file_serv.c
컴파일 방법을 명시해야 할 것 같음
5절 바로 위에 다음 줄 추가.
"소스코드를 컴파일하기 위해서는 sqlite 라이브러리를 링크시켜야 한다. 다음과 같이 컴파일 하자"
<코드>
# gcc -o file_serv file_serv.c -lsqlite3
</코드>

file_cli.c
23 줄 char f_name[64]를 char fname[64]로
<코드>
struct Cquery
{
  int command;
  char fname[64];   // 지금은 f_name[64]로 되어 있어서 에러 발생
}
</코드>
