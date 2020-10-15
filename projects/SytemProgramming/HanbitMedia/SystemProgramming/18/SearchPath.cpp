/*
	SearchPath.cpp
	프로그램 설명: SearchPath 함수의 활용.
*/

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int _tmain(int argc, TCHAR* argv[])
{
	TCHAR fileName[MAX_PATH];
	TCHAR bufFilePath[MAX_PATH];
	LPTSTR lpFilePart;
	DWORD result;

	_tprintf( _T("Insert name of the file to find: ") );
	_tscanf( _T("%s"), fileName);

	result = SearchPath(NULL, fileName, NULL, MAX_PATH, bufFilePath, &lpFilePart);
	
	if(result == 0)
	{
		_tprintf( _T("File not found! \n") );
	}
	else
	{
	    _tprintf( _T("File path: %s \n"), bufFilePath);
	    _tprintf( _T("File name only: %s \n"), lpFilePart);
	}

	return 0;
}
