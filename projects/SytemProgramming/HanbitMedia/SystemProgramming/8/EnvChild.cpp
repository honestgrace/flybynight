/* 
	EnvChild.cpp
	프로그램 설명: 환경 변수 참조하는 자식 프로세스.
*/
#include <stdio.h>
#include <stdlib.h>
#include <windows.h> 

#define BUFSIZE 1024

int _tmain2(int argc, TCHAR * argv[])
{
	TCHAR value[BUFSIZE]; 
	
	if(GetEnvironmentVariable(_T("Good"), value, BUFSIZE) > 0) 
		_tprintf(_T("[%s = %s] \n"), _T("Good"), value);
	
	if(GetEnvironmentVariable(_T("Hey"), value, BUFSIZE) > 0) 
		_tprintf(_T("[%s = %s] \n"), _T("Hey"), value);
	
	if(GetEnvironmentVariable(_T("Big"), value, BUFSIZE) > 0) 
		_tprintf(_T("[%s = %s] \n"), _T("Big"), value);
	
	Sleep(10000);

	return 0;
}

