/*
	FileTimeInformation.cpp
	프로그램 설명: 파일 시간 정보 확인하는 예제.
*/

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define STRING_LEN		100

int _tmain(int argc, TCHAR* argv[])
{
	TCHAR fileName[] = _T("RealScaleViewer.exe");

	TCHAR fileCreateTimeInfo[STRING_LEN];
	TCHAR fileAccessTimeInfo[STRING_LEN];
	TCHAR fileWriteTimeInfo[STRING_LEN];

	FILETIME ftCreate, ftAccess, ftWrite;

	SYSTEMTIME stCreateUTC, stCreateLocal;
	SYSTEMTIME stAccessUTC, stAccessLocal;
	SYSTEMTIME stWriteUTC, stWriteLocal;

	HANDLE hFile = CreateFile(
						fileName, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if(hFile == INVALID_HANDLE_VALUE)
	{
		_tprintf( _T("File open fault! \n") );
		return -1; 
	}

    // 파일 정보 추출
    if (!GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite))
	{
		_tprintf( _T("GetFileTime function call fault! \n") );
        return FALSE;
	}


    FileTimeToSystemTime(&ftCreate, &stCreateUTC);
    SystemTimeToTzSpecificLocalTime(NULL, &stCreateUTC, &stCreateLocal);

    FileTimeToSystemTime(&ftAccess, &stAccessUTC);
    SystemTimeToTzSpecificLocalTime(NULL, &stAccessUTC, &stAccessLocal);

    FileTimeToSystemTime(&ftWrite, &stWriteUTC);
    SystemTimeToTzSpecificLocalTime(NULL, &stWriteUTC, &stWriteLocal);


    _stprintf(fileCreateTimeInfo, _T("%02d/%02d/%d  %02d:%02d"),
        stCreateLocal.wMonth, stCreateLocal.wDay, stCreateLocal.wYear,
        stCreateLocal.wHour, stCreateLocal.wMinute);

    _stprintf(fileAccessTimeInfo, _T("%02d/%02d/%d  %02d:%02d"),
        stAccessLocal.wMonth, stAccessLocal.wDay, stAccessLocal.wYear,
        stAccessLocal.wHour, stAccessLocal.wMinute);

    _stprintf(fileWriteTimeInfo, _T("%02d/%02d/%d  %02d:%02d"),
        stWriteLocal.wMonth, stWriteLocal.wDay, stWriteLocal.wYear,
        stWriteLocal.wHour, stWriteLocal.wMinute);

	_tprintf( _T("File created  time : %s \n"), fileCreateTimeInfo ); 
	_tprintf( _T("File accessed time : %s \n"), fileAccessTimeInfo ); 
	_tprintf( _T("File written  time : %s \n"), fileWriteTimeInfo ); 

	CloseHandle(hFile); 

	return 0;
}
