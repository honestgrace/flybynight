/*
	OperationStateChild.cpp
	프로그램 설명 : 프로그램 실행 결과에 따른 반환 값.
*/
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <tchar.h>

int _tmain(int argc, TCHAR* argv[])
{
	float num1, num2;

	_fputts(_T("Return Value Test \n"), stdout); 

	_tscanf(_T("%f %f"), &num1, &num2);

	if(num2 == 0)
	{
		exit(-1); // or return -1;
	}

	_tprintf(_T("Operation Result : %f \n"), num1/num2);


	return 1;
}
