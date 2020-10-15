/* 
    WBCS3.cpp
*/
#include <stdio.h>

int main(int argc, char* argv[])
{
	int i;
	for(i=0; i<argc; i++)
		fputws(argv[i], stdout);

	return 0;
}