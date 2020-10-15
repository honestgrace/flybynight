/*
	__try__finally.cpp
	프로그램 설명: 종료 핸들러의 동작.
*/

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int _tmain(int argc, TCHAR* argv[])
{

	int a, b;

	__try
	{
		_tprintf( _T("input divide string [ a / b ]: ") );
		_tscanf( _T("%d / %d"), &a, &b);

		if(b == 0)
			return -1; 

	}
	__finally
	{
		_tprintf( _T("__finally block! \n") );
	}

	_tprintf( _T("result: %d \n"), a/b);

	return 0;
}
