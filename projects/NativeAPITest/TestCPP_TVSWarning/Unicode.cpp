// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <Windows.h>
#include <iostream>
using namespace std;
//#define ARRAYSIZE(x)        ((sizeof (x)) / (sizeof ((x)[0]))) 

void Unicode()
{
	wchar_t Wide1[256] = L"𠀀";
	LONG cbWide1 = (LONG)wcslen(Wide1);
	size_t cbmbs1 = wcstombs(NULL, Wide1, 0);
	cout << "cbmbs1:" << cbmbs1 << endl;

	size_t cbmbs1_s = 0;
	char dest[256];
	auto error = wcstombs_s(&cbmbs1_s, dest, 256, Wide1, 100);
	cout << "cbmbs1_s:" << cbmbs1_s << endl;
	cout << "error:" << error << endl;

	wchar_t Wide2[256] = L"𠀁";
	LONG cbWide2 = (LONG)wcslen(Wide2);
	size_t cbmbs2 = wcstombs(NULL, Wide2, 0);
	LONG m_cbLuTransIdPrint = cbWide1 + cbWide2 + 2;
	LONG m_cbLuTransIdPrint2 = (LONG)(cbmbs1 + cbmbs2 + 2);

	cout << "m_cbLuTransIdPrint:" << m_cbLuTransIdPrint << endl;
	cout << "m_cbLuTransIdPrint2:" << m_cbLuTransIdPrint2 << endl;
}

