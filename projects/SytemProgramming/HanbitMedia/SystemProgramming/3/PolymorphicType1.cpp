/* 
    PolymorphicType1.cpp
*/
#include <tchar.h>
#include <windows.h>
#include <stdio.h>

UINT CalDistance(UINT a, UINT b)
{
	return a-b;
}

int _tmain(void)
{
	INT val1 = 10;
	INT val2 = 20;

	_tprintf(_T("Position %d, %d \n"), (UINT)&val1, (UINT)&val2);
	_tprintf(_T("distance : %d \n"), CalDistance((UINT)&val1, (UINT)&val2));

	return 0;
}
