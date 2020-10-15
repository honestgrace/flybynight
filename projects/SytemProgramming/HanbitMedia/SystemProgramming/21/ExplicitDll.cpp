/*
    ExplicitDll.cpp
    Explicit DLL È°¿ë.
*/
	 
#include <stdio.h>
#include <windows.h>
#include "SwapDll.h"

typedef void (*SWAP_FUNC)(int *, int *); 

int _tmain(int argc, TCHAR * argv[])
{
	HMODULE hinstLib; 
	SWAP_FUNC SwapFunction; 

	hinstLib = LoadLibrary( _T("SwapDll")); 
	if (hinstLib == NULL) 
	{
		_tprintf( _T("LoadLibrary fail! \n") ); 
		return -1;
	}
	
	SwapFunction = (SWAP_FUNC) GetProcAddress(hinstLib, _T("swap")); 
	if (SwapFunction == NULL) 
	{
		_tprintf( _T("GetProcAddress fail! \n") ); 
		return -1;
	}
	
	int a=10;
	int b=20;
	_tprintf( _T("Before: %d, %d \n"), a, b);
	
	SwapFunction(&a, &b);
	_tprintf( _T("After: %d, %d \n"), a, b);

    BOOL isSuccess = FreeLibrary(hinstLib);
	if(isSuccess == NULL)
	{
		_tprintf( _T("FreeLibrary fail! \n") );
		return -1;
	}

    return 0;
}
