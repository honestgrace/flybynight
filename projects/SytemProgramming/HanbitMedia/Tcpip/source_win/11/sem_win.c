#include <stdio.h>
#include <process.h>
#include <windows.h>

#define MAX_THREAD 2

HANDLE gsem;
int count = 0;

DWORD WINAPI myThread(void *arg)
{
    int threadNum = *((int *)arg);
    int tmp;
    printf("Thread Num %d\n", threadNum);
    while(1)
    {
   	 WaitForSingleObject(gsem, INFINITE);
   	 tmp = count;
   	 Sleep(1000);
   	 count = tmp +1;
   	 printf("#(%d) : Count ++ %d\n",threadNum, count);
   	 ReleaseSemaphore(gsem, 1, NULL);
    }
    return 0;
}

int main(int argc, char **argv)
{
    HANDLE ThreadH[2];
    int iloop;

    gsem = CreateSemaphore(NULL, 1, 1, "TestSem");
    if (gsem == NULL)
    {
   	 printf("Error %d\n", GetLastError( ));
   	 return 1;
    }

    if ( OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "TestSem") == NULL)
    {
   	 printf("Semaphore Open Error %d\n", GetLastError( ));
   	 return 1;
    }

    for (iloop = 0; iloop < MAX_THREAD; iloop++)
    {
   	 printf("Create Thread %d\n", iloop);
   	 ThreadH [iloop] = CreateThread(NULL, 0, myThread, (void *)&iloop, 0, NULL);
   	 Sleep(500);
    }
    WaitForMultipleObjects(MAX_THREAD, ThreadH, TRUE, INFINITE);
    CloseHandle(gsem);
    return 0;
}
