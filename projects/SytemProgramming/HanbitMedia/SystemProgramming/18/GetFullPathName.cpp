/*
	GetFullPathName.cpp
	���α׷� ����: �����̸� ������� ���� ��� ������.
*/

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define STRING_LEN		100

int _tmain(int argc, TCHAR* argv[])
{
	TCHAR fileName[] = _T("RealScaleViewer.exe");
	TCHAR fileFullPathName[STRING_LEN];
	LPTSTR filePtr;

	GetFullPathName(fileName, STRING_LEN, fileFullPathName, &filePtr);

	_tprintf( _T("%s \n"), fileFullPathName); 
	_tprintf( _T("%s \n"), filePtr); 

	return 0;
}

