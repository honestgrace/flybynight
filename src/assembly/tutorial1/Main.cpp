#include <Windows.h>
#include <iostream>
#include <conio.h>
#include <time.h>

using namespace std;
#include "AsmFunctions.h"

extern "C" int MyCppProc(int a, int b, int c, int d, int e, int f)
{
	cout << "Hello " << f << endl;
	return 0;
}

extern "C" int MyPritnf(char *a)
{
	printf(a);
	return 0;
}

int testasm()
{
	CallCppASM();
	auto x = Multiply(3, 5);
	auto y = Divide(6, 2);
	auto z = Remainder(6, 4);

	void * test;
	test = ReturnMyArray();
	char * pt = (char *) test;
	for (int i = 0; i < 1000; i++)
	{
		*pt = 'a';
		pt++;
	}
	test = ReturnMyArray();
	
	const int SIZE = 5;
	int data[SIZE];
	for (int i = 0; i < SIZE; i++)
	{
		data[i] = 0xFFFE;
	}

	//int countInByte = SIZE * sizeof(int);
	int countInByte = 5;
	ZeroArrayASM(data, countInByte);

	auto value = GetValueFromASM();
	auto value2 = TestFunction();
	auto value3 = AddASM(3, 5);

	_putch('a');

	cout << "ASM says " << value << endl;
	_getch();
	return 0;
}

int testThread()
{
	int j = 0;
	const int loopCount = 5;
	
	HANDLE handles[loopCount];

	long startTime = clock();

	
	for (int i = 0; i<loopCount; i++)
		handles[i] = CreateThreadInASM(&j);

	for (int i = 0; i<loopCount; i++)
		WaitForSingleObject(handles[i], INFINITE);

	long finishTime = clock();

	cout << "The value of is " << j << endl;
	cout << "The time taken is " << (finishTime - startTime) << endl;

	return 0;
}

int main()
{
	//testasm();
	testThread();
    return 0;
}

