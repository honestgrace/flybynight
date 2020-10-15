#include "pch.h"
#include <iostream>

using namespace std;
HANDLE g_hEvent;

DWORD CALLBACK MyThreadProc(LPVOID lpParam)
{
	char *pData = (char *)lpParam;
	cout << "Running " << pData << endl;

	if (!SetEvent(g_hEvent))
	{
		cout << "Failed to set event!!!"<< endl;
	}
	return 0;
}

int main()
{
	//
	// By default, the thread pool has a maximum of 512 threads per process.
	// To raise the queue limit, use the WT_SET_MAX_THREADPOOL_THREAD macro defined in WinNT.h.

	char Data[256] = "abc";
	char *pData = Data;
	
	g_hEvent = CreateEvent(
		NULL,               // default security attributes
		TRUE,               // manual-reset event
		FALSE,              // initial state is nonsignaled
		TEXT("WriteEvent")  // object name
	);

	BOOL  bQuwi = QueueUserWorkItem(MyThreadProc, (PVOID)pData, WT_EXECUTELONGFUNCTION);
	if (! bQuwi)
	{
		cout << "Failed to QueueUserWorkItem()!!!" << endl;
		return 0;
	}

	if (WaitForSingleObject(g_hEvent, INFINITE))
	{
		cout << "Failed to wait for the event to be set!!!" << endl;
		return 0;
	}
}

