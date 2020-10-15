/* 
    WBCS1.cpp
*/
#include <stdio.h>
#include <string.h>

int main(void)
{
	wchar_t str[]=L"ABC";
	int size=sizeof(str);
	int len=wcslen(str);

	printf("배열의 크기 : %d \n", size);
	printf("문자열 길이 : %d \n", len);

	return 0;
}