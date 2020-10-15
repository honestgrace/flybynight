/*
	GetFileSize.cpp
	프로그램 설명: 파일 크기 얻는 예제.
*/

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>


int _tmain(int argc, TCHAR* argv[])
{
	TCHAR fileName[] = _T("RealScaleViewer.exe");

	HANDLE hFile = CreateFile(
						fileName, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if(hFile == INVALID_HANDLE_VALUE)
	{
		_tprintf( _T("File open fault! \n") );
		return -1; 
	}


	DWORD high4ByteFileSize = 0; 
	DWORD low4ByteFileSize = GetFileSize(hFile, &high4ByteFileSize);
 
	_tprintf( _T("High 4byte file size: %u byte \n"), high4ByteFileSize ); 
	_tprintf( _T("Low 4byte file size: %u byte \n"), low4ByteFileSize ); 


	LARGE_INTEGER fileSize;
	GetFileSizeEx(hFile, &fileSize);

	_tprintf( _T("Total file size: %u byte \n"), fileSize.QuadPart); 


	CloseHandle(hFile); 

	return 0;
}
