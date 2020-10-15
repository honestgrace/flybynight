/*
	SEH_Calculator_One.cpp
	프로그램 설명: EXCEPTION_EXECUTE_HANDER 활용 사례.
*/

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

enum{DIV=1, MUL, ADD, MIN, EXIT};

DWORD ShowMenu(void);
BOOL Calculator(void);

int _tmain(int argc, TCHAR* argv[])
{
	BOOL state;

	do{

		state = Calculator();

	}while(state == TRUE);


	return 0;
}

DWORD ShowMenu(void)
{
    DWORD sel;

    _fputts(_T("-----Menu----- \n"), stdout);
    _fputts(_T("num 1: Divide \n"), stdout);
    _fputts(_T("num 2: Multiple \n"), stdout);
    _fputts(_T("num 3: Add \n"), stdout);
    _fputts(_T("num 4: Minus \n"), stdout);
    _fputts(_T("num 5: Exit \n"), stdout);
    _fputts(_T("SELECTION >>"), stdout);
    _tscanf(_T("%d"), &sel);
		
    return sel;
}

BOOL Calculator(void)
{
	DWORD sel;
	int num1, num2, result;

	sel=ShowMenu();
	if(sel==EXIT)
		return FALSE;

    _fputts(_T("Input num1 num2: "), stdout);
    _tscanf(_T("%d %d"), &num1, &num2);
	
	__try
	{

		switch(sel)
		{
			case DIV:
				result = num1 / num2;
				_tprintf(_T("%d/%d=%d \n\n"), num1, num2, result);
				break;

			case MUL:
				result = num1 * num2;
				_tprintf(_T("%d*%d=%d \n\n"), num1, num2, result);
				break;

			case ADD:
				result = num1 + num2;
				_tprintf(_T("%d+%d=%d \n\n"), num1, num2, result);
				break;

			case MIN:
				result = num1 - num2;
				_tprintf(_T("%d-%d=%d \n\n"), num1, num2, result);
				break;
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		_tprintf( _T("Wrong number inserted. Try again! \n\n") );
	}

	return TRUE;
}
