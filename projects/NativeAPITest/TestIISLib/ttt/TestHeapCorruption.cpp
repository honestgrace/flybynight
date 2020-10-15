#include "stdafx.h"

#include <windows.h> 
#include <stdio.h> 
#include <conio.h>
#include <iostream>
using namespace std;

class TestHeapCorruption02_CAppInfo
{
public:
	TestHeapCorruption02_CAppInfo(LPWSTR wszAppName, LPWSTR wszVersion)
	{
		m_wszAppName = wszAppName;
		m_wszVersion = wszVersion;
	}

	VOID PrintAppInfo()
	{
		wprintf(L"\nFull application Name: %s\n", m_wszAppName);
		wprintf(L"Version: %s\n", m_wszVersion);
	}

private:
	LPWSTR m_wszAppName;
	LPWSTR m_wszVersion;
};

static TestHeapCorruption02_CAppInfo* g_AppInfo;
class TestHeapCorruption02
{
public:

	static int Test()
	{
		wint_t iChar = 0;
		g_AppInfo = new TestHeapCorruption02_CAppInfo(L"Memory Corruption Sample", L"1.0");
		if (!g_AppInfo)
		{
			return 1;
		}

		wprintf(L"Press: \n");
		wprintf(L"    1    To display application information\n");
		wprintf(L"    2    To simulated memory corruption\n");
		wprintf(L"    3    To exit\n\n\n>");

		while ((iChar = _getwche()) != '3')
		{
			switch (iChar)
			{
			case '1':
				g_AppInfo->PrintAppInfo();
				break;

			case '2':
				SimulateMemoryCorruption();
				wprintf(L"\nMemory Corruption completed\n");
				break;

			default:
				wprintf(L"\nInvalid option\n");
			}
			wprintf(L"\n\n> ");
		}
		return 0;
	}

private:
	static VOID SimulateMemoryCorruption()
	{
		char* pszWrite = "Corrupt";
		BYTE* p = (BYTE*)g_AppInfo;
		CopyMemory(p, pszWrite, strlen(pszWrite));
	}
};

class TestHeapCorruption01
{
public:
	static void Test()
	{
		int const ARRAY_SIZE = 10;
		cout << "Press any key to start..." << endl;
		_getch();
		auto tempSize = sizeof(int*[ARRAY_SIZE]);
		int** pPtrArray = (int**)HeapAlloc(GetProcessHeap(), 0, tempSize);
		if (pPtrArray != NULL)
		{
			InitArray(pPtrArray);
			*(pPtrArray[0]) = 10;
			HeapFree(GetProcessHeap(), 0, pPtrArray);
		}
	}

private:
	static BOOL InitArray(int** pPtrArray)
	{
		return FALSE;
	}
};

class TestStackCorruption01
{
public:
	static void Test()
	{
		auto BAD_ADDRESS = 0xBAADF00D;
		char* p = (char*)BAD_ADDRESS;
		*p = 'A';
	}

private:
	
};

class TestStackOverrun
{
public:
	static const int MAX_CONN_LEN = 30;
	static wstring TestStackOverrunData;
	static void Test()
	{
		WCHAR *TestStackOverrunData = L"ThisIsMyVeryExtremelySuperMagnificantConnectionStringForMyDataSource";
		HelperFunction(TestStackOverrunData);
		wprintf(L"Connection to %s established\n", TestStackOverrunData);
	}

private:
	static VOID HelperFunction(const WCHAR* pszConnectionString)
	{
		WCHAR pszCopy[MAX_CONN_LEN];

		// wcscpy is vulnerable api
		// wcscpy(pszCopy, pszConnectionString);

		//auto size = lstrlenW(pszConnectionString);
		//wcscpy_s(pszCopy, size, pszConnectionString);
		//
		// ...
		// Establish connection
		// ...
		//
	}
};

void simpleOne() {
	void *p;
	p = malloc(4);
	char *pszString = (char *)p;
	strcpy(pszString, "abc");
	*(pszString + 3) = 'd';
	*(pszString + 4) = '\0';
	cout << pszString << endl;
	free(p);
}

void TestHeapCorruption()
{
	simpleOne();
	/*
	TestStackOverrun::Test();
	TestHeapCorruption02::Test();
	TestStackCorruption01::Test();
	TestHeapCorruption01::Test();
	*/
}
