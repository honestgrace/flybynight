#include "stdafx.h"

#include <windows.h>

DWORD CALLBACK ThreadProc(PVOID pvParam);

void TestDuplicateHandle()
{
	HANDLE hMutex = CreateMutex(NULL, FALSE, NULL);
	HANDLE hMutexDup, hThread;
	DWORD dwThreadId;

	DuplicateHandle(GetCurrentProcess(),
		hMutex,
		GetCurrentProcess(),
		&hMutexDup,
		0,
		FALSE,
		DUPLICATE_SAME_ACCESS);

	hThread = CreateThread(NULL, 0, ThreadProc,
		(LPVOID)hMutexDup, 0, &dwThreadId);

	// Perform work here, closing the handle when finished with the
	// mutex. If the reference count is zero, the object is destroyed.
	CloseHandle(hMutex);

	// Wait for the worker thread to terminate and clean up.
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);	
}

DWORD CALLBACK ThreadProc(PVOID pvParam)
{
	HANDLE hMutex = (HANDLE)pvParam;

	// Perform work here, closing the handle when finished with the
	// mutex. If the reference count is zero, the object is destroyed.
	CloseHandle(hMutex);
	return 0;
}
