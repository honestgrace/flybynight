// NativeAPITest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
using namespace std;

#include <string>
#include <iostream>

void IISLibTest();
void TestLongPath();
int TestBuffer();
HRESULT TestConvertToString();
HRESULT TestUnicodeStringAPI();


int __cdecl main(int argc, char **argv)
{
	STRU* m_pData;
	m_pData = new STRU(nullptr, 255);
	auto result = wcscmp(m_pData->QueryStr(), L"test");

	string command;
	command = string(argv[argc - 1]);
	int newArgc = argc - 1;
	if (command == "IISLibTest") IISLibTest();
	if (command == "TestConvertToString") TestConvertToString();
	if (command == "TestUnicodeStringAPI") TestUnicodeStringAPI();
	if (command == "TestLongPath") TestLongPath();
	if (command == "TestBuffer") TestBuffer();
	return 0;
}


