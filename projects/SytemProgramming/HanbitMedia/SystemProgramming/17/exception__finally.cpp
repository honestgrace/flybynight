/*
	exception__finally.cpp
	���α׷� ����: ���� �ڵ鷯�� ����.
*/

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int _tmain(int argc, TCHAR* argv[])
{
	TCHAR str[2];

	__try
	{
		_tcscpy(str, _T("ABCDEFGHIJKLMNOPQRSTUVWXYZ~") );

		_tprintf( _T("%s \n"), str);

	}
	__finally
	{
		_tprintf( _T("__finally block! \n") );
	}

	return 0;
}
