/*
    DuplicateHandleChildProcess.cpp
    프로그램 설명: DuplicateHandle 함수의 이해.
    
    실행파일의 이름을 ChildProcess.exe로 만들어야 한다.
*/
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int _tmain(int argc, TCHAR* argv[])
{
	HANDLE hParent = (HANDLE)_ttoi(argv[1]);
	DWORD isSuccess = WaitForSingleObject(hParent, INFINITE);

	_tprintf(_T("[Child Process] \n"));

	if(isSuccess == WAIT_FAILED)
	{
		_tprintf( _T("WAIT_FAILED returned!") ); 
		Sleep(10000);
		return -1;
	}
	else
	{
		_tprintf(_T("General Lee said, \"Don't inform the enemy my death\""));
		Sleep(10000);
		return 0;
	}
}

