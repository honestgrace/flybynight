/*
	Operation1.cpp
	프로그램 설명 : 단순 출력 프로그램1.
*/

#include <stdio.h>
#include <windows.h>
#include <tchar.h>

int _tmain(int argc, TCHAR* argv[])
{
	STARTUPINFO si={0,};
	PROCESS_INFORMATION pi;
	
	si.cb=sizeof(si);

	TCHAR command[]=_T("Operation2.exe");

	CreateProcess(NULL,     // 프로세스 생성.
			      command,
				  NULL,
				  NULL, 
				  TRUE, 
				  0, 
				  NULL, 
				  NULL, 
				  &si, 
				  &pi
	);  //CreateProcess

	while(1)
	{
		for(DWORD i=0; i<10000; i++)
			for(DWORD i=0; i<10000; i++); //Busy Waiting!!
		_fputts(_T("Operation1.exe \n"), stdout); 
	}

	return 0;
}

