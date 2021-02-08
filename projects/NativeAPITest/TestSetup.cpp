#include "stdafx.h"
#include <iostream>
using namespace std;

HRESULT
IsServer()
{
	BOOL isServer;
	BOOL* pfIsServer = &isServer;

	HRESULT         hr = S_OK;
	OSVERSIONINFOEX osVersionInfoEx = { 0 };
	DWORDLONG       dwlConditionMask = 0;
	BOOL            fReturn = FALSE;

	*pfIsServer = FALSE;
	osVersionInfoEx.dwOSVersionInfoSize = sizeof(osVersionInfoEx);

	//
	// If we are not workstation (Client) 
	// that means that we are a server or domain controller (Server)
	//
	osVersionInfoEx.wProductType = VER_NT_WORKSTATION;
	VER_SET_CONDITION(dwlConditionMask, VER_PRODUCT_TYPE, VER_EQUAL);

	fReturn = VerifyVersionInfo(
		&osVersionInfoEx,
		VER_PRODUCT_TYPE,
		dwlConditionMask);

	//
	// If the function fails, the return value is zero 
	// and GetLastError returns an error code other than ERROR_OLD_WIN_VERSION
	//
	if (fReturn == FALSE && GetLastError() != ERROR_OLD_WIN_VERSION)
	{
		hr = HRESULT_FROM_WIN32(GetLastError());
		goto Exit;
	}

	*pfIsServer = (fReturn == FALSE);

	if (isServer)
	{
		printf("This machine is server OS\r\n");
	}
	else
	{
		printf("This machine is client OS\r\n");
	}

Exit:

	return hr;

}

void TestSetup()
{
	IsServer();
}