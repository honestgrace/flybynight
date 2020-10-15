#include "stdafx.h"

#include <windows.h>
#include <iostream>
#include <conio.h>
using namespace std;

LONG interlocked_variable = 0; // 0 unlocked, 1 locked
int run = 1;

DWORD WINAPI thread(LPVOID lpParam)
{
	while (run)
	{
		while (InterlockedCompareExchange(&interlocked_variable, 1, 0) == 1)
			SwitchToThread();
		++(*((unsigned int*)lpParam));
		InterlockedExchange(&interlocked_variable, 0);
		SwitchToThread();
	}

	return 0;
}

void TestInterlockedCompareExchange()
{
	unsigned int num_threads;
	cout << "number of threads: ";
	cin >> num_threads;
	unsigned int* num_cycles = new unsigned int[num_threads];
	DWORD s_time, e_time;

	s_time = GetTickCount();
	for (unsigned int i = 0; i < num_threads; ++i)
	{
		num_cycles[i] = 0;
		HANDLE handle = CreateThread(NULL, NULL, thread, &num_cycles[i], NULL, NULL);
		CloseHandle(handle);
	}
	_getch();
	run = 0;
	e_time = GetTickCount();

	unsigned long long total = 0;
	for (unsigned int i = 0; i < num_threads; ++i)
		total += num_cycles[i];
	for (unsigned int i = 0; i < num_threads; ++i)
		cout << "\nthread " << i << ":\t" << num_cycles[i] << " cyc\t" << ((double)num_cycles[i] / (double)total) * 100 << "%";
	cout << "\n----------------\n"
		<< "cycles total:\t" << total
		<< "\ntime elapsed:\t" << e_time - s_time << " ms"
		<< "\n----------------"
		<< '\n' << (double)(e_time - s_time) / (double)(total) << " ms\\op\n";

	delete[] num_cycles;
	_getch();	
}