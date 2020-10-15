#include <stdio.h>
#include <time.h>
#include <windows.h>

#define PIPE_NAME "\\\\.\\pipe\\echo"
#define BUF_SIZE 1024

int main(int argc, char **argv)
{
    HANDLE ph = NULL;
    time_t ctime = 0;
    struct tm *ltm=NULL;
    char buf[BUF_SIZE];
    LPDWORD nread =0, nwrite=0;
    BOOL brtv = FALSE;


    ph = CreateFile(PIPE_NAME, GENERIC_READ | GENERIC_WRITE,
   			 0,
   			 NULL,
   			 OPEN_EXISTING,
   			 0,
   			 NULL);
    if (ph == INVALID_HANDLE_VALUE)
    {
   	 printf("Pipe open failure!!\n");
   	 return 1;
    }

    while(1)
    {
   	 printf("> ");
   	 fgets(buf, BUF_SIZE, stdin);
   	 if(strncmp(buf, "quit\n",5) == 0)
   	 {
   		 break;
   	 }
   	 WriteFile(ph, buf, strlen(buf), nwrite, NULL);
   	 memset(buf, 0x00, BUF_SIZE);
   	 ReadFile(ph, buf, BUF_SIZE, nread, NULL);
   
  printf("Server -> %s", buf);
    }
    return 0;
}
