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

	printf("�迭�� ũ�� : %d \n", size);
	printf("���ڿ� ���� : %d \n", len);

	return 0;
}