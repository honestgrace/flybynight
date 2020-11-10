#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <conio.h>

DWORD WINAPI ThreadProcedure(LPVOID lpParameter);
VOID ProcA();
VOID Sum(int* numArray, int iCount, int* sum);

class Job
{
public:
	virtual void Do()
	{

	}
};

class JobA: public Job
{
public:
	virtual void Do()
	{
		printf("This is job A\n");
	}
};

class JobB : public Job
{
public:
	virtual void Do()
	{
		printf("This is job B\n");
	}
};

void CreateThreadForStackTracking()
{
	//DebugBreak();
	HANDLE hThread, hThread2 = NULL;

	printf("Starting new thread...\n");
	
	JobA a;
	JobB b;

	hThread = CreateThread(NULL, 0, ThreadProcedure, (LPVOID)&a, 0, NULL);
	hThread2 = CreateThread(NULL, 0, ThreadProcedure, (LPVOID)&b, 0, NULL);

	if (hThread != NULL)
	{
		printf("success\n");
		WaitForSingleObject(hThread, INFINITE);
		CloseHandle(hThread);
	}

	if (hThread2 != NULL)
	{
		printf("success\n");
		WaitForSingleObject(hThread2, INFINITE);
		CloseHandle(hThread2);
	}
}

DWORD WINAPI ThreadProcedure(LPVOID lpParameter)
{
	Job* p = (Job*) lpParameter;
	p->Do();
	ProcA();
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