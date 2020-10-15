#include <unistd.h>
#include <stdio.h>
#include <string.h>
	 
int main(int argc, char **argv)
{
	printf("-- START --\n");
	execl("/bin/ls","ls", NULL);
	printf("-- END --\n");
	return 1;
}
