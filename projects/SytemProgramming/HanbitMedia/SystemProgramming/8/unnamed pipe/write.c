/*
  write.c
  unnamed pipe를 위한 예제.
*/

#include <stdio.h>

int main(void)
{
	fputs("AAA \n", stdout);
	fputs("BBB \n", stdout);
	fputs("CCC \n", stdout);
	fputs("stop \n", stdout); 

	return 0;
}