/*
	LibSwap.cpp
*/

#include <stdio.h>
#include <tchar.h>
		
void swap(int* v1, int* v2);      // swap 함수 선언
	
int _tmain(int argc, TCHAR * argv[])
{
    int a=10;
    int b=20;
    
    _tprintf( _T("Before: %d %d \n"), a, b);
    
    swap(&a, &b);
    
    _tprintf( _T("After: %d %d \n"), a, b);
    
    return 0;
}

void swap(int* v1, int* v2)        // swap 함수의 정의
{
    int temp=*v1;
    *v1=*v2;
    *v2=temp;
}

