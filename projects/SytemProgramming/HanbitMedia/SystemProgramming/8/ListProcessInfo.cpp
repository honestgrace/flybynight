/*
    ListProcessInfo.cpp
    프로그램 설명: 현재 실행중인 프로세스 정보 출력.
*/

#include <stdio.h>
#include <tchar.h>
#include <windows.h> 
#include <tlhelp32.h> /* 프로세스 정보 추출관련 함수 선언 */

int _tmain(int argc, TCHAR * argv[])
{
	/**** Snapshot! 말 그대로 사진을 찍는 함수. ************************
	 **** 프로세스 상태 사진을 찍기 위해 TH32CS_SNAPPROCESS 인자 전달 **/
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0 );
	if( hProcessSnap == INVALID_HANDLE_VALUE )
	{
		_tprintf( _T("CreateToolhelp32Snapshot error! \n") );
		return -1;
	}

	/** 위 함수 CreateToolHelp32Snapshot을 통해 정보는 이미 얻었다. **
	 ** 다음은 추출한 프로세스 정보를 순차 접근하는 과정을 보여한다 **/

	PROCESSENTRY32 pe32;   /* 프로세스 정보 얻기위한 구조체 변수 */

	/* PROCESSENTRY32 변수는 사용하기 전에 크기 정보 초기화 해야한다 */
	pe32.dwSize = sizeof( PROCESSENTRY32 );

	/** 첫 번째 프로세스 정보 얻을 때는 Process32First 함수 사용 ***
	 ** 그 이후 프로세스 정보 얻을 때는 Process32Next 함수 사용 ****/
	if( !Process32First( hProcessSnap, &pe32 ) )
	{
		_tprintf( _T("Process32First error! \n") );
		CloseHandle( hProcessSnap );
		return -1;
	}

	HANDLE hProcess;
	do
	{
		/* 프로세스 이름, ID 정보 출력 */
		_tprintf(_T("%25s %5d \n"), pe32.szExeFile, pe32.th32ProcessID); 

	} while( Process32Next( hProcessSnap, &pe32 ) );

	CloseHandle( hProcessSnap );
	return 0; 
}
