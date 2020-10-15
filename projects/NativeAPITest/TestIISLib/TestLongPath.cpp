#include "stdafx.h"

#include <iostream>
#include <string>
using namespace std;

//
// https://visualstudiomagazine.com/articles/2013/12/01/long-filenames-in-windows-8.aspx
// https://kennykerr.ca/articles/
//
#include <pathcch.h>
#pragma comment(lib, "pathcch")

struct path_exception
{
	HRESULT code;

	path_exception(HRESULT const result) :
		code{ result }
	{}
};

auto check(HRESULT const result) -> void
{
	if (S_OK != result)
	{
		throw path_exception(result);
	}
}

namespace details
{
	auto path_append(std::wstring & path,
		wchar_t const * more,
		size_t const size) -> void
	{
		path.resize(path.size() + 5 + size);

		check(PathCchAppendEx(&path[0],
			path.size() + 1,
			more,
			0));

		path.resize(wcslen(path.c_str()));
	}
}

auto path_append(std::wstring & path,
	wchar_t const * more) -> void
{
	details::path_append(path,
		more,
		wcslen(more));
}

auto path_append(std::wstring & path,
	std::wstring const & more) -> void
{
	details::path_append(path,
		more.c_str(),
		more.size());
}

void Test_path_append()
{
	try
	{
		auto path = std::wstring{ L"C:\\" };

		path_append(path, L"MorePath");
		path_append(path, std::wstring{ L"MorePath2" });

		wprintf(L"%s\n", path.c_str());
	}
	catch (path_exception const & e)
	{
		wprintf(L"%x\n", e.code);
	}
}


#define BUFSIZE 4096
#define LONG_DIR_NAME TEXT("c:\\longdirectoryname")

HRESULT
IsPathUnc(
	__in  LPCWSTR       pszPath,
	__out BOOL *        pfIsUnc
)
{
	HRESULT hr = S_OK;
	STRU strTempPath;

	if (pszPath == NULL || pfIsUnc == NULL)
	{
		hr = E_INVALIDARG;
		goto Finished;
	}

	hr = MakePathCanonicalizationProof((LPWSTR)pszPath, &strTempPath);
	if (FAILED(hr))
	{
		goto Finished;
	}

	//
	// MakePathCanonicalizationProof will map \\?\UNC, \\.\UNC and \\ to \\?\UNC
	//
	(*pfIsUnc) = (_wcsnicmp(strTempPath.QueryStr(), L"\\\\?\\UNC\\", 8 /* sizeof \\?\UNC\ */) == 0);

Finished:

	return hr;
}


void TestLongPath()
{
	STACK_STRU(ss, 5);

	ss.Resize(11);
	ss.Copy(L"0123456789");


	HRESULT hr = S_OK;
	BOOL output;
	WCHAR *p = L"test";
	STRU temp;
	hr = temp.Copy(L"Test");
	if (FAILED(hr))
	{		
		printf("failed");
	}

	temp.Append(L"Test2");
	if (FAILED(hr))
	{
		printf("failed");
	}

	temp.Copy(p);
	if (FAILED(hr))
	{
		printf("failed");
	}

	temp.Append(p);
	if (FAILED(hr))
	{
		printf("failed");
	}

	p = NULL;
	temp.Copy(p);
	if (FAILED(hr))
	{
		printf("failed");
	}

	temp.Append(p);
	if (FAILED(hr))
	{
		printf("failed");
	}

	IsPathUnc(L"c:\foo", &output);
	IsPathUnc(L"\\foo\bar", &output);

	Test_path_append();

	DWORD  retval = 0;
	BOOL   success;
	TCHAR  buffer[BUFSIZE] = TEXT("");
	TCHAR  buf[BUFSIZE] = TEXT("");
	TCHAR** lppPart = { NULL };

	// Retrieve the full path name for a file. 
	// The file does not need to exist.

	retval = GetFullPathName(L"\.",
		BUFSIZE,
		buffer,
		lppPart);

	if (retval == 0)
	{
		// Handle an error condition.
		printf("GetFullPathName failed (%d)\n", GetLastError());
		return;
	}
	else
	{
		_tprintf(TEXT("The full path name is:  %s\n"), buffer);
		if (lppPart != NULL && *lppPart != 0)
		{
			_tprintf(TEXT("The final component in the path name is:  %s\n"), *lppPart);
		}
	}


	// Create a long directory name for use with the next two examples.

	success = CreateDirectory(LONG_DIR_NAME,
		NULL);

	if (!success)
	{
		// Handle an error condition.
		printf("CreateDirectory failed (%d)\n", GetLastError());
		return;
	}


	// Retrieve the short path name.  

	retval = GetShortPathName(LONG_DIR_NAME,
		buf,
		BUFSIZE);

	if (retval == 0)
	{
		// Handle an error condition.
		printf("GetShortPathName failed (%d)\n", GetLastError());
		return;
	}
	else _tprintf(TEXT("The short name for %s is %s\n"),
		LONG_DIR_NAME, buf);


	// Retrieve the long path name.  

	retval = GetLongPathName(buf,
		buffer,
		BUFSIZE);

	if (retval == 0)
	{
		// Handle an error condition.
		printf("GetLongPathName failed (%d)\n", GetLastError());
		return;
	}
	else _tprintf(TEXT("The long name for %s is %s\n"), buf, buffer);

	// Clean up the directory.

	success = RemoveDirectory(LONG_DIR_NAME);
	if (!success)
	{
		// Handle an error condition.
		printf("RemoveDirectory failed (%d)\n", GetLastError());
		return;
	}
}

