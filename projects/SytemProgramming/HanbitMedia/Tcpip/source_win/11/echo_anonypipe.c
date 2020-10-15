#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define MAX_LINE 1024
int main(int argc, char **argv)
{
    HANDLE readPipe, writePipe;
    SECURITY_ATTRIBUTES sec;
    DWORD writen;
    int rtv;
    TCHAR command[80] = {0,};
    char buf[MAX_LINE];
    STARTUPINFO si = {0,};
    PROCESS_INFORMATION pi;

    sec.nLength = sizeof(SECURITY_ATTRIBUTES);
    sec.bInheritHandle = TRUE;
    sec.lpSecurityDescriptor = NULL;

    CreatePipe(&readPipe, &writePipe, &sec, 0);

    _snprintf ((char *)command, 80, "C:\\echo_anonypipe_cli.exe %d", (HANDLE)readPipe);

    si.cb = sizeof(si);
    rtv = CreateProcess(
   	 NULL, command,
   	 NULL, NULL,
   	 TRUE,
   	 CREATE_NEW_CONSOLE,
   	 NULL, NULL,
   	 &si, &pi);
    if (!rtv)
    {
   	 printf("Error %d\n", GetLastError( ));
   	 return 1;
    }

    while(1)
    {
   	 printf("> ");
   	 fgets(buf, MAX_LINE, stdin);
   	 if(strncmp(buf, "quit\n", 5)==0) break;
   	 WriteFile(writePipe, buf, strlen(buf), &writen, NULL);
    }
    CloseHandle(readPipe);
    CloseHandle(writePipe);
}
