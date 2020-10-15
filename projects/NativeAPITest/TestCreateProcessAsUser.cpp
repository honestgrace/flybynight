#include "stdafx.h"
#include "ProcessStarter.h"
#include "userenv.h"
#include "wtsapi32.h"
#include "winnt.h"
#include <iostream>
#include <string>
using namespace std;

ProcessStarter::ProcessStarter(const std::wstring& processPath, const std::wstring& arguments)
: processPath_(processPath), arguments_(arguments)
{
}

HANDLE primaryToken = 0;
PHANDLE ProcessStarter::GetCurrentUserToken()
{
    int dwSessionId = 0;
	auto currentToken = hToken;
    BOOL bRet = DuplicateTokenEx(currentToken, TOKEN_ALL_ACCESS, 0, SecurityImpersonation, TokenPrimary, &primaryToken);
    int errorcode = GetLastError();
    if (bRet == false)
    {
        return 0;
    }

    return &primaryToken;
}

BOOL ProcessStarter::Run()
{
	GetToken();
	HANDLE token = hToken;
	token = *(GetCurrentUserToken());
	
	STARTUPINFOW startupInfo;
	PROCESS_INFORMATION processInformation;

	ZeroMemory(&startupInfo, sizeof(startupInfo));
	startupInfo.cb = sizeof(startupInfo);

	ZeroMemory(&processInformation, sizeof(processInformation));

    SECURITY_ATTRIBUTES Security1;
    SECURITY_ATTRIBUTES Security2;

    std::wstring command = L"\"" + processPath_ + L"\"";
    if (arguments_.length() != 0)
    {
        command += L" " + arguments_;
    }

    void* lpEnvironment = NULL;
    BOOL resultEnv = CreateEnvironmentBlock(&lpEnvironment, token, FALSE);
    if (resultEnv == 0)
    {                                
        long nError = GetLastError();                                
        lpEnvironment = NULL;
    }

	if (CreateProcessAsUser(
		token,             // client's access token
		NULL,              // file to execute
		(LPWSTR)(command.c_str()),    // command line
		NULL, // &Security1,              // pointer to process SECURITY_ATTRIBUTES
		NULL, // &Security2,              // pointer to thread SECURITY_ATTRIBUTES
		TRUE,             // handles are not inheritable
		CREATE_NO_WINDOW | NORMAL_PRIORITY_CLASS | CREATE_UNICODE_ENVIRONMENT,
		lpEnvironment,              // pointer to new environment block 
		NULL,              // name of current directory 
		&startupInfo,               // pointer to STARTUPINFO structure
		&processInformation                // receives information about new process
	))
	{
		WaitForSingleObject(processInformation.hProcess, INFINITE);
		CloseHandle(processInformation.hProcess);
		CloseHandle(processInformation.hThread);

		DestroyEnvironmentBlock(lpEnvironment);
		CloseHandle(token);
	}   
	else
	{
		cout << "Error : " << GetLastError() << endl;
	}

	return TRUE;
}

void TestCreateProcessAsUser()
{
	wstring processPath = L"c:\\windows\\system32\\notepad.exe";
	ProcessStarter starter(processPath, L"");
	_bstr_t strUser;
	_bstr_t strdomain;
	auto pid = GetCurrentProcessId();
	starter.GetUserFromProcess(pid, strUser, strdomain);

	starter.Run();
}