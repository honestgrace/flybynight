#include <stdio.h>
#include <time.h>
#include <windows.h>

#define PIPE_NAME "\\\\.\\pipe\\echo"
#define BUF_SIZE 1024 // IN, OUT

int main(int argc, char **argv)
{
    HANDLE ph = NULL;
    time_t ctime = 0;
    struct tm *ltm=NULL;
    char buf[256];
    DWORD nread =0, nwrite=0;
    BOOL brtv = FALSE;


    ph = CreateNamedPipe(
   	 PIPE_NAME,
   	 PIPE_ACCESS_DUPLEX,
   	 PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
   	 PIPE_UNLIMITED_INSTANCES,
   	 BUF_SIZE,
   	 BUF_SIZE,
   	 NMPWAIT_USE_DEFAULT_WAIT,
   	 NULL);
    if (ph == INVALID_HANDLE_VALUE)
    {
   	 printf("Pipe create failure!!\n");
   	 return 1;
    }

    while(1)
    {
   	 if(ConnectNamedPipe(ph, NULL))
   	 {
   		 brtv = ReadFile(ph, buf, 256, &nread, NULL);
   		 if(!brtv || (nread == 0))
   		 {
   			 printf("Read Pipe Error\n");
   			 return 1;
   		 }
   		 brtv = WriteFile(ph, buf, nread, &nwrite, NULL);
   		 if(!brtv || (nwrite != nread))
   		 {
   			 printf("Write Error\n");
   		 }
   	 }
    }
}

