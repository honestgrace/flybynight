#include <stdio.h>
#include <windows.h>

#define MAX_LINE 1024
int main(int argc, char **argv)
{
    HANDLE readPipe = (HANDLE)(atoi(argv[1]));
    DWORD readn;
    char buf[MAX_LINE];

    while(1)
    {
        memset(buf, 0x00, MAX_LINE);
        ReadFile (
            readPipe,
            buf,
            MAX_LINE,
            &readn,
        NULL);
        printf(" --> %s", buf);
    }
}
