/*
	completion_routine_file.cpp
	프로그램 설명: 파일 기반의 확장 I/O 예제.
*/
#include <stdio.h>
#include <stdlib.h>
#include <windows.h> 

TCHAR strData[] = 
        _T("Nobody was farther off base than the pundits who said \n")
        _T("Royal Liverpool was outdated and not worthy of hosting the Open again \n")
		 _T("for the first time since 1967. The Hoylake track held up beautifully. \n")
		 _T("Here's the solution to modern golf technology -- firm, \n")
		 _T("fast fairways, penal bunkers, firm greens and, with any luck, lots of wind. \n");	

VOID WINAPI 
FileIOCompletionRoutine ( DWORD, DWORD, LPOVERLAPPED);

int _tmain(int argc, TCHAR* argv[]) 
{
	TCHAR fileName[] = _T("data.txt");

	HANDLE hFile = CreateFile (
						fileName, GENERIC_WRITE, FILE_SHARE_WRITE, 0, CREATE_ALWAYS, FILE_FLAG_OVERLAPPED, 0
				   );
	if(hFile == INVALID_HANDLE_VALUE)
	{
		_tprintf( _T("File creation fault! \n") );
		return -1; 
	}

	OVERLAPPED overlappedInst;
	memset(&overlappedInst, 0, sizeof(overlappedInst));
	overlappedInst.hEvent= (HANDLE)1234;	// 추가로 데이터 전송 가능한 경로.
	WriteFileEx(hFile, strData, sizeof(strData), &overlappedInst, FileIOCompletionRoutine);

	SleepEx(INFINITE, TRUE);
	CloseHandle(hFile); 

	return 1; 
} 

VOID WINAPI 
FileIOCompletionRoutine ( DWORD errorCode, DWORD numOfBytesTransfered, LPOVERLAPPED overlapped)
{
	_tprintf( _T("**********File write result ************\n") );
	_tprintf( _T("Error code: %u \n"), errorCode);
	_tprintf( _T("Transferred bytes len: %u \n"), numOfBytesTransfered);
	_tprintf( _T("The other info: %u \n"), (DWORD)overlapped->hEvent);
}
