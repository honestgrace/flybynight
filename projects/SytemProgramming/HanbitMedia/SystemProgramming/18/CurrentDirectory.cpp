/*
	CurrentDirectory.cpp
	프로그램 설명: 현재 디렉터리의 이해. 
*/
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define STRING_LEN		100


TCHAR fileData[] = _T("abcdefghijklmnopqrstuvwxyz");

int _tmain(int argc, TCHAR* argv[])
{
	TCHAR fileName[] = _T("abcd.dat");
	TCHAR readBuf[STRING_LEN];

	HANDLE hFileWrite; 
	HANDLE hFileRead;
	DWORD numOfByteWritten;

	/*********** file write ********************/
	hFileWrite = CreateFile (
				fileName, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	WriteFile ( hFileWrite, fileData, sizeof(fileData), &numOfByteWritten, NULL );
	CloseHandle(hFileWrite); 

	/* 현재 디렉터리 변경 */
	SetCurrentDirectory( _T("C:\\") );

	/*********** file read ********************/
	hFileRead = CreateFile(
				fileName, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
	if(hFileRead == INVALID_HANDLE_VALUE)
	{
		_tprintf( _T("File open error! \n") );
		return -1;
	}

	ReadFile(hFileRead, readBuf, sizeof(readBuf), &numOfByteWritten, NULL);
	_tprintf( _T("%s \n"), readBuf); 
	CloseHandle(hFileRead); 

	return 0;
}

