/* 
    PolymorphicType2.cpp
*/

#include <tchar.h>
#include <windows.h>
#include <stdio.h>

UINT_PTR CalDistance(UINT_PTR a, UINT_PTR b)

{
	return a-b;
}


int _tmain(void)
{
	INT32 val1 = 10;
	INT32 val2 = 20;

	_tprintf(_T("distance : %d \n"), CalDistance((UINT_PTR)&val1, (UINT_PTR)&val2));

	return 0;
}
