#include "stdafx.h"
#include <windows.h>
#include <TCHAR.h>
#include <iostream>
using namespace std;

//#undef _UNICODE 
//#undef UNICODE 

#define SIZE 15
#define MYARRAYSIZE(x)        ((sizeof (x)) / (sizeof ((x)[0]))) 
#define MYARRAYSIZE2(a) \
  ((sizeof(a) / sizeof(*(a))) / \
  static_cast<size_t>(!(sizeof(a) % sizeof(*(a)))))


template<size_t size>
void Copy(PCWSTR const (&rgpszStrings)[size])
{
	PCWSTR buffer[size];
	//return AuxAppend(rgpszStrings, _countof(rgpszStrings));
	auto a = _countof(rgpszStrings);
	//strncpy(buffer, rgpszStrings, _countof(rgpszStrings));
	//wcsncpy(buffer, rgpszStrings, size);
}

void TVS6384()
{
	PCWSTR str[2] = { L"test", L"test2" };
	PCWSTR const rgAppend[] =
	{
		L"foo",
		L"bar"
	};

	Copy<2>(str);
	
	//char * dest = new char[SIZE];
	char dest[SIZE];
	char src[SIZE] = "Hello, World!!";
	auto a = sizeof dest;
	auto b = sizeof dest[0];
	auto c = MYARRAYSIZE(dest);
	auto d = MYARRAYSIZE2(dest);
	if (dest)
	{
		strncpy(dest, src, sizeof dest / sizeof dest[0]);
	}
}
