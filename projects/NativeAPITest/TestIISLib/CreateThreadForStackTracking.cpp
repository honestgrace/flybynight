#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <conio.h>

DWORD WINAPI ThreadProcedure(LPVOID lpParameter);
VOID ProcA();
VOID Sum(int* numArray, int iCount, int* sum);

void CreateThreadForStackTracking()
{

	HANDLE hThread = NULL;

	printf("Starting new thread...");

	hThread = CreateThread(NULL, 0, ThreadProcedure, NULL, 0, NULL);
	if (hThread != NULL)
	{
		printf("success\n");
		WaitForSingleObject(hThread, INFINITE);
		CloseHandle(hThread);
	}
}

DWORD WINAPI ThreadProcedure(LPVOID lpParameter)
{
	ProcA();
	printf("Press any key to exit thread\n");
	_getch();
	return 0;
}

VOID ProcA()
{
	int iCount = 3;
	int iNums[] = { 1,2,3 };
	int iSum = 0;

	Sum(iNums, iCount, &iSum);
	printf("Sum is: %d\n", iSum);
}

VOID Sum(int* numArray, int iCount, int* sum)
{
	for (int i = 0; i<iCount; i++)
	{
		*sum += numArray[i];
	}
}