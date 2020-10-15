/* 
    MBCS_WBCS1.cpp
*/

#define UNICODE
#define _UNICODE

#include <stdio.h>
#include <tchar.h>
#include <windows.h>

int wmain(void)
{
	TCHAR str[] = _T("1234567");

	int size = sizeof(str);

	printf("string size : %d \n", size);

	return 0;
}