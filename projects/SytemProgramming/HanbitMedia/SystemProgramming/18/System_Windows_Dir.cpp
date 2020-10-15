/*
	System_Windows_Dir.cpp
	프로그램 설명: 시스템 디렉터리와 Windows 디렉터리의 확인. 
*/
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int _tmain(int argc, TCHAR* argv[])
{
	TCHAR sysPathBuf[MAX_PATH];
	TCHAR winPathBuf[MAX_PATH];

	GetSystemDirectory(sysPathBuf, MAX_PATH);
	GetWindowsDirectory(winPathBuf, MAX_PATH);

	_tprintf( _T("System dir: %s \n"), sysPathBuf);
	_tprintf( _T("Windows dir: %s \n"), winPathBuf);

	return 0;
}

