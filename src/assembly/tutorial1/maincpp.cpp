#include <iostream>
#include <conio.h>

using namespace std;

int GetValueFromASM()
{
	__asm {
		mov eax, 39  // => return 39;
	}
}

int main()
{
	cout << "ASM said " << GetValueFromASM() << endl;
	_getch();
	return 0;
}