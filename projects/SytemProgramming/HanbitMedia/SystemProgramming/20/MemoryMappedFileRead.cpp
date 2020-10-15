/*
	MemoryMappedFileRead.cpp
	���α׷� ����: Memory mapped file ����.
*/
#include <stdio.h>
#include <stdlib.h>
#include <windows.h> 

//version 1
int main(int argc, TCHAR * argv[])
{
	// 1�ܰ�: ���� �ڵ� ����.
	HANDLE hFile = 
		CreateFile (
			_T("data.dat"), GENERIC_READ|GENERIC_WRITE, 0, NULL, 
			CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL
		);
	if (hFile == INVALID_HANDLE_VALUE) 
		_tprintf(_T("Could not open file."));

	TCHAR fileData[] = _T("Best test string~ ^^");
	DWORD numOfByteWritten = 0; 
	WriteFile(hFile, fileData, sizeof(fileData), &numOfByteWritten, NULL);
	
	// 2�ܰ�: ���� ���� ��ü ����.
	HANDLE hMapFile = 
		CreateFileMapping (hFile, NULL, PAGE_READONLY, 0, 0, NULL);
	if (hMapFile == NULL) 
		_tprintf(_T("Could not create map of file. %d \n"), GetLastError() );
	
	// 3�ܰ�: �޸𸮿� ����. 
	TCHAR * pWrite = 
		(TCHAR *)MapViewOfFile (
			hMapFile, FILE_MAP_READ, 0, 0, 0
		);
	if (pWrite == NULL) 
		_tprintf(_T("Could not map view of file. %d \n"), GetLastError() );

	_tprintf( _T("string in file: %s\n"), pWrite ); 
	
	UnmapViewOfFile(pWrite);
	CloseHandle(hMapFile);
	CloseHandle(hFile);
	_tprintf( _T("End of process! \n"));
	return 0;
}

