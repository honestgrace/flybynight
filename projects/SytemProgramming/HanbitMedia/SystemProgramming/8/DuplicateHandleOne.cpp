/*
    DuplicateHandleOne.cpp
    프로그램 설명: DuplicateHandle 함수의 이해.
*/
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int _tmain(int argc, TCHAR* argv[])
{
	HANDLE hProcess;
	TCHAR cmdString[1024];

	DuplicateHandle(
		GetCurrentProcess(), GetCurrentProcess(), GetCurrentProcess(), 
		&hProcess, 0, FALSE, DUPLICATE_SAME_ACCESS
	);

	_stprintf(cmdString, _T("%s %u"), _T("ChildProcess.exe"), (unsigned)hProcess);

	STARTUPINFO si={0,};
	PROCESS_INFORMATION pi={0,};
    si.cb = sizeof(si);

	BOOL isSuccess = CreateProcess(
		NULL, cmdString, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
                       
	if(isSuccess == FALSE)
	{
		_tprintf( _T("CreateProcess failed \n") );
		return -1;
    }
	
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	_tprintf(_T("[Parent Process]\n"));
	_tprintf( _T("ooooooooooooooooooooooopps! \n"));
	return 0;
}
