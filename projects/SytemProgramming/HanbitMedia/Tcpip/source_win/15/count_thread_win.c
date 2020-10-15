#include <windows.h>
#include <stdio.h>
#define MAX_THREAD 4
DWORD WINAPI t_func(void *data)
{
	int *count = (int *)data;
	while(1)
	{
		printf("%d %d\n", GetCurrentThreadId( ), *count);
		*count = *count+1;
		Sleep(1000);
		if(*count == 20)
			break;
	}
	return 0;
}
int main(int argc, char **argv)
{
	DWORD dwThreadArray[MAX_THREAD];
	HANDLE hThreadArray[MAX_THREAD];
	int i;
	int count = 0;
	for(i = 0; i < MAX_THREAD; i++)
	{
		hThreadArray[i] = CreateThread(NULL, 0, t_func, (void *)&count, 0, &dwThreadArray[i]);
	}
	WaitForMultipleObjects(MAX_THREAD, hThreadArray, TRUE, INFINITE);
	for (i = 0; i < MAX_THREAD; i++)
	{
		CloseHandle(hThreadArray[i]);
	}
	return 0;
}
