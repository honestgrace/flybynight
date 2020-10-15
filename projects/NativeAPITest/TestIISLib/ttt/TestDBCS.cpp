#include "stdafx.h"
#include <iostream>
using namespace std;

BOOL ShouldSanitizeCharacter(
	__in CHAR*          charPtr
)
{
	if ((UCHAR)*charPtr <= 0x20 || *charPtr == 0x7F)
	{
		return TRUE;
	}

	return FALSE;
}

BOOL ShouldSanitizeCharacter(
	__in WCHAR*          charPtr
)
{
	if (*charPtr <= 0x20 || *charPtr == 0x7F)
	{
		return TRUE;
	}

	return FALSE;
}


void TestDBCS()
{
	char data[10] = { 0xb1, 0xe8, 0xc1, 0xa4, 0xc8, 0xaf, 0x2e, 0x63, 0x6f, 0x6d };
	char *pData = data;
	int length = 10;
	for (int i = 0; i < length; i++)
	{
		if (ShouldSanitizeCharacter(&(data[i])))
		{
			cout << "+";
		}
		else
		{
			char temp = data[i];
			cout << temp;
		}
	}

	for (int i = 0; i < length; i++)
	{
		if (ShouldSanitizeCharacter(&(pData[i])))
		{
			cout << "+";
		}
		else
		{
			char temp = data[i];
			cout << temp;
		}
	}

	WCHAR data2[3] = { L'±è', L'Á¤', L'È¯' };
	length = 3;
	for (int i = 0; i < length; i++)
	{
		if (ShouldSanitizeCharacter(&(data2[i])))
		{
			cout << "+";
		}
		else
		{
			wcout << data2[i];
		}
	}

	length = 6;
	pData = (char*) data2;
	for (int i = 0; i < length; i++)
	{
		if (ShouldSanitizeCharacter(&(pData[i])))
		{
			cout << "+";
		}
		else
		{
			char temp = data[i];
			cout << temp;
		}
	}
}