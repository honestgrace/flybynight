/*
	SEH_FlowView.cpp
	프로그램 설명: SEH의 실행 흐름 파악하기
*/

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int _tmain(int argc, TCHAR* argv[])
{
	_tprintf( _T("start point! \n") );

	int * p = NULL;

	__try
	{
		*p = 100;	// 예외 발생 지점.
		_tprintf( _T("value: %d \n"), *p) ;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		_tprintf( _T("exception occurred! \n") );
	}

	_tprintf( _T("end point! \n") );

	return 0;
}

