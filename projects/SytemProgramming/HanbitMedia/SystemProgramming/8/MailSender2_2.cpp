/* 
	MailSender2_2.cpp
	프로그램 설명 : 핸들의 상속 확인용 예제2.
*/

#include <windows.h>
#include <tchar.h>
#include <stdio.h>

int _tmain(int argc, LPTSTR argv[])
{
	HANDLE hMailSlot; 
	TCHAR message[50];
    DWORD bytesWritten;  // number of bytes write

	/************* 핸들을 얻는 코드 *****************/

	FILE* file=_tfopen(_T("InheritableHandle.txt"), _T("rt"));
	_ftscanf(file, _T("%d"), &hMailSlot);
	fclose(file);

	_tprintf(_T("Inheritable Handle : %d \n"), hMailSlot);

	/**********************************************/

	while(1)
	{
		_fputts(_T("MY CMD>"), stdout);
		_fgetts(message, sizeof(message)/sizeof(TCHAR), stdin);

		if(!WriteFile(hMailSlot, message, _tcslen(message)*sizeof(TCHAR), &bytesWritten, NULL))
		{
			_fputts(_T("Unable to write!"), stdout);
			_gettchar();
			CloseHandle(hMailSlot);
			return 1;
		}

		if(!_tcscmp(message, _T("exit")))
		{
			_fputts(_T("Good Bye!"), stdout);
			break;
		}
	}

	CloseHandle(hMailSlot);
	return 0;
}
