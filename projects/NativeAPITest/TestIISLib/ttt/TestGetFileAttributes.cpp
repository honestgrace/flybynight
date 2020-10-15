#include "stdafx.h"

#include <windows.h>

void TestGetFileAttributes()
{
	DWORD result = GetFileAttributes(L"\\?\C:\inetpub\wwwroot\:::й\web.config");
	if (FAILED(result))
	{
		auto error = GetLastError();
		printf("%d", error);
	}

	result = GetFileAttributesW(L"\\?\C:\inetpub\wwwroot\:::й\web.config");
	if (FAILED(result))
	{
		auto error2 = GetLastError();
		printf("%d", error2);
	}
}