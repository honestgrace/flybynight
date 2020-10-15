#include "stdafx.h"
#include "ProcessStarter.h"
#include "userenv.h"
#include "wtsapi32.h"
#include "winnt.h"

#include <iostream>
using namespace std;

void TestCreateProcess()
{
	/*
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	LPWSTR programPath = L"C:\\windows\\system32\\notepad.exe";

	int ret;
	if (!CreateProcess(programPath,   // No module name (use command line)  
		NULL,        // Command line  
		NULL,           // Process handle not inheritable  
		NULL,           // Thread handle not inheritable  
		FALSE,          // Set handle inheritance to FALSE  
		0,              // No creation flags  
		NULL,           // Use parent's environment block  
		NULL,           // Use parent's starting directory   
		&si,            // Pointer to STARTUPINFO structure  
		&pi)           // Pointer to PROCESS_INFORMATION structure  
		)
	{
		ret = (int)(::ShellExecute(NULL, _T("open"), programPath, NULL, NULL, SW_NORMAL));
		if (ret<32) {
			return;
		}
	}
	*/

	auto dwCreationFlags = CREATE_NO_WINDOW | NORMAL_PRIORITY_CLASS | CREATE_UNICODE_ENVIRONMENT; 
	STARTUPINFOW startupInfo;
	PROCESS_INFORMATION processInformation;

	ZeroMemory(&startupInfo, sizeof(startupInfo));
	startupInfo.cb = sizeof(startupInfo);

	ZeroMemory(&processInformation, sizeof(processInformation));

	GetStartupInfoW(&startupInfo);

	//LPWSTR programPath = L"C:\\git\\tools\\src\\TestAPI\\x64\\Debug\\NamedPipeServer.exe";
	//LPWSTR programPath = L"C:\\windows\\system32\\notepad.exe";
	LPWSTR programPath = L"C:\\windows\\system32\\ktmrmsvc.exe"; 
    if (CreateProcessW(programPath,
		NULL,
		NULL,                   // processAttr
		NULL,                   // threadAttr
		TRUE,                   // inheritHandles
		dwCreationFlags,
		NULL,
		L"C:\\windows\\system32",
		&startupInfo,
		&processInformation
	)) 
	{
		WaitForSingleObject( processInformation.hProcess, INFINITE );
		CloseHandle( processInformation.hProcess );
		CloseHandle( processInformation.hThread );
	}
	else
	{
		cout << "Error : " << GetLastError() << endl;
	} 
}