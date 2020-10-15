/* 
	MailSender2_1.cpp
	프로그램 설명 : 핸들의 상속 확인용 예제1.
*/

#include <windows.h>
#include <tchar.h>
#include <stdio.h>

#define SLOT_NAME _T("\\\\.\\mailslot\\mailbox")

int _tmain(int argc, LPTSTR argv[])
{
	HANDLE hMailSlot; 
	TCHAR message[50];
  DWORD bytesWritten;  // number of bytes write
	
	SECURITY_ATTRIBUTES sa;
	sa.nLength=sizeof(sa);
	sa.lpSecurityDescriptor=NULL;
	sa.bInheritHandle=TRUE;

	hMailSlot=CreateFile(SLOT_NAME, GENERIC_WRITE, FILE_SHARE_READ, &sa,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hMailSlot==INVALID_HANDLE_VALUE)
	{
		_fputts(_T("Unable to create mailslot!\n"), stdout);
		return 1;
	}

	_tprintf(_T("Inheritable Handle : %d \n"), hMailSlot);
	FILE* file=_tfopen(_T("InheritableHandle.txt"), _T("wt"));
	_ftprintf(file, _T("%d"), hMailSlot);
	fclose(file);

	STARTUPINFO si={0,};
	PROCESS_INFORMATION pi;	
	si.cb=sizeof(si);

	TCHAR command[]=_T("MailSender2_2.exe");

	CreateProcess(NULL,
			      command,
				  NULL,
				  NULL, 
				  TRUE,  // 자식 프로세스에게 핸들 상속!
				  CREATE_NEW_CONSOLE,
				  NULL, 
				  NULL, 
				  &si, 
				  &pi
	);  //CreateProcess

	while(1)
	{
		_fputts(_T("MY CMD>"), stdout);
		_fgetts(message, sizeof(message)/sizeof(TCHAR), stdin);

		if(!WriteFile(hMailSlot, message, _tcslen(message)*sizeof(TCHAR), &bytesWritten, NULL))
		{
			_fputts(_T("Unable to write!"), stdout);
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
