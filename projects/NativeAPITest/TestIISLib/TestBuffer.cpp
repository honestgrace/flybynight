#include "stdafx.h"
#include <iostream>
#include <string>
using namespace std;

class MyCompleletionInfo {
public:
	void Set(HRESULT hr, DWORD bytesReceived) 
	{
		_hr = hr;
		_bytesReceived = bytesReceived;
	}

	HRESULT GetCompletionStatus()
	{
		return _hr;
	}

	DWORD GetCompletionBytes()
	{
		return _bytesReceived;
	}

private:
	HRESULT _hr;
	DWORD  _bytesReceived;
};


//
// Global variables
//

HANDLE              g_EventAsyncCompletion = INVALID_HANDLE_VALUE;
HANDLE				g_hInputFile = INVALID_HANDLE_VALUE;
bool				g_simulateAsyncTask = true;
MyCompleletionInfo	g_CompletionInfo;
DWORD               g_ReadThisAmountForEachTime = 1024 * 16;
DWORD               g_ConfigurableBufferLength = 1024 * 64;
DWORD               g_OutPutFileLength = 0;
int					g_PassesInTatal = 0;
int					g_FailsInTatal = 0;


HRESULT
ReadEntityBody(
	VOID* pvBuffer,
	DWORD             cbBuffer,
	BOOL              fAsync,
	DWORD* pcbBytesReceived,
	BOOL* pfCompletionPending = NULL
)
{
	HRESULT hr = S_FALSE;

	// simulate read a small portion
	int temp = g_ReadThisAmountForEachTime;
	if (cbBuffer < temp)
	{
		temp = cbBuffer;
	}

	//
	// Replace ReadEntityBody with local input file reading because this is a test program.
	//
	if (ReadFile(g_hInputFile, pvBuffer, temp, pcbBytesReceived, NULL) == false)
	{
		if (GetLastError() == ERROR_HANDLE_EOF) 
		{
			hr = HRESULT_FROM_WIN32(ERROR_HANDLE_EOF);
		}
		else
		{
			hr = S_FALSE;
		}
	}
	else
	{
		if (*pcbBytesReceived == 0)
		{
			// meet the end of file
			hr = HRESULT_FROM_WIN32(ERROR_HANDLE_EOF);
			goto exit;
		}
		else
		{
			hr = S_OK;
		}
	}

exit:
	if (g_simulateAsyncTask)
	{	
		SetEvent(g_EventAsyncCompletion);
		g_simulateAsyncTask = false;
		*pfCompletionPending = true;
		g_CompletionInfo.Set(hr, *pcbBytesReceived);
	}

	return hr;
}

#include "ASYNC_FILE_BUFFER_FACTORY.h"

class DAV_HTTP_MODULE {
public:
	PUT_VERB_HANDLER* _handler;

	DAV_HTTP_MODULE() : _handler(NULL) {

	}

	~DAV_HTTP_MODULE() {
		if (_handler)
		{
			delete _handler;
		}
	}

	REQUEST_NOTIFICATION_STATUS
	OnExecuteRequestHandler()
	{
		_handler = new PUT_VERB_HANDLER;
		if (!_handler)
		{
			cout << "Out of Memory" << endl;
			return REQUEST_NOTIFICATION_STATUS::RQ_NOTIFICATION_FINISH_REQUEST;
		}
		return _handler->Execute();
	}

	REQUEST_NOTIFICATION_STATUS
	OnAsyncCompletion(
		    MyCompleletionInfo *CompletionInfo
		)
	{
		return _handler->OnAsyncCompletion(CompletionInfo);
	}
};

BOOL TestWebDav(
	wstring inputFile,
	DWORD expectedOutputFileLength
)
{
	//
	// Initialize global variables
	//

	g_OutPutFileLength = 0;
	BOOL result = FALSE;

	////////////////////////////////
	// Prepare input file. 
	////////////////////////////////
	g_hInputFile = CreateFile(inputFile.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (g_hInputFile == INVALID_HANDLE_VALUE)
	{
		cout << "First CreateFile failed";
		result = FALSE;
	}

	////////////////////////////////
	// Create WebDAV moudule and execute it
	////////////////////////////////
	int module_state = 0;
	DAV_HTTP_MODULE module;
	REQUEST_NOTIFICATION_STATUS status;
	while (true)
	{
		switch (module_state) {
		case 0:
			status = module.OnExecuteRequestHandler();
			if (status == REQUEST_NOTIFICATION_STATUS::RQ_NOTIFICATION_PENDING)
			{
				module_state = 1;
			}
			if (status != REQUEST_NOTIFICATION_STATUS::RQ_NOTIFICATION_PENDING)
			{
				module_state = 2;
			}
			break;
		case 1:
			status = module.OnAsyncCompletion(&g_CompletionInfo);
			if (status != REQUEST_NOTIFICATION_STATUS::RQ_NOTIFICATION_PENDING)
			{
				module_state = 2;
			}
			break;
		}

		if (status == REQUEST_NOTIFICATION_STATUS::RQ_NOTIFICATION_PENDING)
		{
			// Waiting until someone set the event...
			WaitForSingleObject(g_EventAsyncCompletion, INFINITE);
		}

		if (module_state == 2)
		{
			break;
		}
	}

	////////////////////////////////
	// check test result
	////////////////////////////////
	if (status == REQUEST_NOTIFICATION_STATUS::RQ_NOTIFICATION_CONTINUE) {
		result = TRUE;
		cout << "TestBuffer() is successfully completed" << endl;
	}
	else
	{
		result = FALSE;
		cout << "TestBuffer() Unexpected Status!!!" << endl;
	}

	if (expectedOutputFileLength == g_OutPutFileLength)
	{
		result = TRUE;
	}
	else
	{
		cout << "Not matched output file length, expected: " << expectedOutputFileLength << ", actual: " << g_OutPutFileLength << endl;
	}

	if (result)
	{
	    g_PassesInTatal++;
	}
	else
	{
		g_FailsInTatal++;
	}

	////////////////////////////////
	// close input file
	////////////////////////////////
	if (g_hInputFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(g_hInputFile);
		g_hInputFile = INVALID_HANDLE_VALUE;
	}

	return result;
}

int TestBuffer()
{
	////////////////////////////////
	// Initialize Test Environment
	////////////////////////////////
	g_EventAsyncCompletion = INVALID_HANDLE_VALUE;
	g_EventAsyncCompletion = CreateEvent(
		NULL,
		false,
		false, // false means Not Set
		NULL);

	if (g_EventAsyncCompletion == NULL)
	{
		g_EventAsyncCompletion = INVALID_HANDLE_VALUE;
		cout << "Failed to create event" << endl;
		return 0;
	}

	// simulate various input files
	//g_hInputFile = CreateFile(L"c:\\temp\\test100mb.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	//g_hInputFile = CreateFile(L"c:\\temp\\test10mb.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	//g_hInputFile = CreateFile(L"c:\\temp\\test1mb.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	//g_hInputFile = CreateFile(L"c:\\temp\\test.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	wstring inputFile;
	const DWORD TestExeSize = 226304;

	inputFile = L"c:\\temp\\test.exe";
	g_ConfigurableBufferLength = 1024*64;
	g_simulateAsyncTask = true;
	g_ReadThisAmountForEachTime = 1024 * 16;
	TestWebDav(inputFile, TestExeSize);

	for (int i = 0; i < 2; i++)
	{
		inputFile = L"c:\\temp\\test.exe";
		g_ConfigurableBufferLength = 534;
		g_simulateAsyncTask = (i == 0)? false : true;
		g_ReadThisAmountForEachTime = 533;
		TestWebDav(inputFile, TestExeSize);
	}

	for (int i = 0; i < 2; i++)
	{
		inputFile = L"c:\\temp\\test1mb.txt";
		g_ConfigurableBufferLength = 534;
		g_simulateAsyncTask = (i == 0) ? false : true;
		g_ReadThisAmountForEachTime = 533;
		TestWebDav(inputFile, 1000000);
	}

	for (int i = 0; i < 2; i++)
	{
		inputFile = L"c:\\temp\\test.txt";
		g_ConfigurableBufferLength = 534;
		g_simulateAsyncTask = (i == 0) ? false : true;
		g_ReadThisAmountForEachTime = 533;
		TestWebDav(inputFile, 1);
	}

	for (int i = 0; i < 2; i++)
	{
		inputFile = L"c:\\temp\\test10mb.txt";
		g_ConfigurableBufferLength = 64*1024;
		g_simulateAsyncTask = (i == 0) ? false : true;
		g_ReadThisAmountForEachTime = 16*1024;
		TestWebDav(inputFile, 10000000);
	}


	cout << endl << "****************************************" << endl;
	cout << "Tests succeeded: " << g_PassesInTatal << ", Tests failed: " << g_FailsInTatal;
	cout << endl << "****************************************" << endl;

	////////////////////////////////
	// close event
	////////////////////////////////
	if (g_EventAsyncCompletion != INVALID_HANDLE_VALUE)
	{
		CloseHandle(g_EventAsyncCompletion);
		g_EventAsyncCompletion = INVALID_HANDLE_VALUE;
	}

	return 0;
}