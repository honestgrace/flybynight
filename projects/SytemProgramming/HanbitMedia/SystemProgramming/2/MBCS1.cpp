/* 
    MBCS1.cpp
*/
#include <stdio.h>
#include <string.h>

int main(void)
{
	char str[]="ABC�ѱ�";
	int size=sizeof(str);
	int len=strlen(str);

	printf("�迭�� ũ�� : %d \n", size);
	printf("���ڿ� ���� : %d \n", len);

	return 0;
}