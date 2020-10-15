/*
    CommandPrmpt_Six.cpp
    ���α׷� ����: ��� ������Ʈ 6�� 
    ��ɾ� 1: type --> �ؽ�Ʈ ���� ���� ���.
    ��ɾ� 2: type text.txt | sort  --> ������.
*/

#include <stdio.h>
#include <tchar.h>
#include <locale.h>
#include <windows.h> 
#include <tlhelp32.h>

#define STR_LEN    256
#define CMD_TOKEN_NUM  10

TCHAR ERROR_CMD[] 
= _T("'%s'��(��) ������ �� �ִ� ���α׷��� �ƴմϴ�. \n");

TCHAR cmdString[STR_LEN];
TCHAR cmdTokenList[CMD_TOKEN_NUM][STR_LEN];
TCHAR seps[]   = _T(" ,\t\n");

int CmdReadTokenize(void);
int CmdProcessing(int);
TCHAR * StrLower(TCHAR *);

int _tmain(int argc, TCHAR * argv[])
{
	// �ѱ� �Է��� ������ �ϱ� ����.
	_tsetlocale(LC_ALL, _T("Korean")); 

	if(argc > 2)  // �Ű����� �������ڰ� �ִ� ���.
	{
		for(int i= 1; i<argc; i++)
			_tcscpy(cmdTokenList[i-1], argv[i]);

		CmdProcessing(argc-1);
	}

	DWORD isExit = NULL;
	while(1)
	{
		int tokenNum = CmdReadTokenize();
		if(tokenNum == 0) //Enter �Է½� ó���� ����...
			continue;

		isExit = CmdProcessing(tokenNum);
		if(isExit == TRUE)
		{
			_fputts(_T("��ɾ� ó���� �����մϴ�. \n"), stdout );
			break;
		}
	}

	return 0; 
}

int CmdReadTokenize(void)
{
	TCHAR * token;

	_fputts( _T("Best command prompt>> "), stdout );
	_getts(cmdString);

	token = _tcstok(cmdString, seps);

	int tokenNum = 0;

	while(token != NULL)
	{
		_tcscpy ( cmdTokenList[tokenNum++], StrLower(token) );
		token = _tcstok(NULL, seps);
	}

	return tokenNum;
}

void ListProcessInfo(void)
{
	HANDLE hProcessSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
	if( hProcessSnap == INVALID_HANDLE_VALUE )
	{
		_tprintf( _T("CreateToolhelp32Snapshot error! \n") );
		return;
	}

	PROCESSENTRY32 pe32;	/* ���μ��� ���� ������� ����ü ���� */

	/* PROCESSENTRY32 ������ ����ϱ� ���� ũ�� ���� �ʱ�ȭ �ؾ��Ѵ� */
	pe32.dwSize = sizeof( PROCESSENTRY32 );

	/* ù ��° ���μ��� ���� ���� */
	if( !Process32First( hProcessSnap, &pe32 ) )
	{
		_tprintf( _T("Process32First error! \n") );
		CloseHandle( hProcessSnap );
		return; 
	}

	do
	{
		/* ���μ��� �̸�, ID ���� ��� */
		_tprintf(_T("%25s %5d \n"), pe32.szExeFile, pe32.th32ProcessID); 

	} while( Process32Next( hProcessSnap, &pe32 ) );

	CloseHandle( hProcessSnap );
}

void KillProcess(void)
{
	HANDLE hProcessSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
	if( hProcessSnap == INVALID_HANDLE_VALUE )
	{
		_tprintf( _T("CreateToolhelp32Snapshot (of processes)") );
		return;
	}

	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof( PROCESSENTRY32 );

	if( !Process32First( hProcessSnap, &pe32 ) )
	{
		_tprintf( _T("Process32First") );
		CloseHandle( hProcessSnap );
		return;
	}

	HANDLE hProcess;
	BOOL isKill = FALSE;
	do
	{
		if(_tcscmp(pe32.szExeFile, cmdTokenList[1]) == 0)
		{
			hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID );

			if(hProcess != NULL)
			{
				TerminateProcess(hProcess, -1);
				isKill = TRUE;
			}

			CloseHandle(hProcess);
			break;
		}

	} while( Process32Next( hProcessSnap, &pe32 ) );

	CloseHandle( hProcessSnap );

	if(isKill == FALSE)
		_tprintf(_T("Kill process fail, Try again! \n"));
}

void TypeTextFile(void)
{
	TCHAR cmdStringWithOptions[STR_LEN]={0,};
	BOOL isRun;

	if(!_tcscmp(cmdTokenList[2], _T("|")) )
	{
		/* Create unnamed pipe */
		HANDLE hReadPipe, hWritePipe;

		SECURITY_ATTRIBUTES pipeSA ={
			sizeof(SECURITY_ATTRIBUTES), NULL, TRUE
		};

		CreatePipe(&hReadPipe, &hWritePipe, &pipeSA, 0);

		/* process type�� ���� ���� */
		STARTUPINFO siType={0,};
		PROCESS_INFORMATION piType;
		siType.cb=sizeof(siType);

		siType.hStdInput=GetStdHandle(STD_INPUT_HANDLE);
		siType.hStdError=GetStdHandle(STD_ERROR_HANDLE);
		siType.hStdOutput=hWritePipe;       // ��� �����̷���!
		siType.dwFlags |=STARTF_USESTDHANDLES;

		_tcscpy(cmdStringWithOptions, cmdTokenList[0]);
		_stprintf(cmdStringWithOptions, _T("%s %s"), cmdStringWithOptions, cmdTokenList[1]);

		isRun = CreateProcess(NULL, cmdStringWithOptions, NULL, NULL, TRUE, 0, NULL, NULL, &siType, &piType);

		CloseHandle(piType.hThread);
		CloseHandle(hWritePipe);

		/* process sort�� ���� ����*/
		STARTUPINFO siSort={0,};
		PROCESS_INFORMATION piSort;
		siSort.cb=sizeof(siSort);

		siSort.hStdInput=hReadPipe;		    // �Է� �����̷���!
		siSort.hStdError=GetStdHandle(STD_ERROR_HANDLE);
		siSort.hStdOutput=GetStdHandle(STD_OUTPUT_HANDLE);
		siSort.dwFlags |=STARTF_USESTDHANDLES;

		isRun = CreateProcess(NULL, cmdTokenList[3], NULL, NULL, TRUE, 0, NULL, NULL, &siSort, &piSort);

		CloseHandle(piSort.hThread);
		CloseHandle(hReadPipe);

		WaitForSingleObject(piType.hProcess, INFINITE);
		WaitForSingleObject(piSort.hProcess, INFINITE);

		CloseHandle(piType.hProcess);
		CloseHandle(piSort.hProcess);
	}
	else
	{
		_tcscpy(cmdStringWithOptions, cmdTokenList[0]);
		_stprintf(cmdStringWithOptions, _T("%s %s"), cmdStringWithOptions, cmdTokenList[1]);

		STARTUPINFO si={0,};
		PROCESS_INFORMATION pi;
		si.cb=sizeof(si);

		isRun = CreateProcess(NULL, cmdStringWithOptions, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
		WaitForSingleObject(pi.hProcess, INFINITE);

		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
}

int CmdProcessing(int tokenNum)
{ 
	BOOL isRun;
	STARTUPINFO si={0,};
	PROCESS_INFORMATION pi; 

	TCHAR cmdStringWithOptions[STR_LEN]={0,};
	TCHAR optString[STR_LEN]={0,};

	si.cb=sizeof(si);
	if( !_tcscmp(cmdTokenList[0],_T("exit")) )
	{  
		return TRUE;
	}
	else if ( !_tcscmp(cmdTokenList[0],_T("start")) )
	{
		if(tokenNum >1)
		{
			for(int i=1; i<tokenNum; i++)
				_stprintf(optString, _T("%s %s"), optString, cmdTokenList[i]);

			_stprintf(cmdStringWithOptions, _T("%s %s"), _T("CmdProject.exe"), optString);
		}
		else
			_stprintf(cmdStringWithOptions, _T("%s"), _T("CmdProject.exe"));

		isRun = CreateProcess(NULL, cmdStringWithOptions, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);

		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
	else if ( !_tcscmp(cmdTokenList[0], _T("echo")) )
	{
		for(int i=1; i<tokenNum; i++)
			_stprintf(optString, _T("%s %s"), optString, cmdTokenList[i]);

		_tprintf( _T("echo message:%s \n"), optString);
	}
	else if( !_tcscmp(cmdTokenList[0], _T("lp")) )
	{
		ListProcessInfo();
	}
	else if( !_tcscmp(cmdTokenList[0], _T("kp")) )
	{
		if(tokenNum <2)
		{
			_tprintf(_T("usage: kp <process name> \n"));
			return 0; 
		}

		KillProcess();
	}
	else if( !_tcscmp(cmdTokenList[0], _T("sort")) )
	{
		if(!_tcscmp(cmdTokenList[1], _T(">")) )
		{
			SECURITY_ATTRIBUTES fileSec = {
				sizeof(SECURITY_ATTRIBUTES), NULL, TRUE
			};// ��� �����ؾ� ���� ����!

			HANDLE hFile = CreateFile ( 
				cmdTokenList[2], GENERIC_WRITE,	FILE_SHARE_READ,
				&fileSec, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL,	NULL
			);

			si.hStdOutput = hFile;
			si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
			si.hStdError = GetStdHandle(STD_ERROR_HANDLE);
			si.dwFlags |= STARTF_USESTDHANDLES;

			isRun = CreateProcess(NULL, cmdTokenList[0], NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);

			WaitForSingleObject(pi.hProcess, INFINITE);
			CloseHandle(hFile);
		}
		else
		{
			isRun = CreateProcess(NULL, cmdTokenList[0], NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
			WaitForSingleObject(pi.hProcess, INFINITE);
		}

		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);

	}
	else if( !_tcscmp(cmdTokenList[0], _T("type")) )
	{
		TypeTextFile();
	}
	else
	{
		_tcscpy(cmdStringWithOptions, cmdTokenList[0]);

		for(int i=1; i<tokenNum; i++)
			_stprintf(cmdStringWithOptions, _T("%s %s"), cmdStringWithOptions, cmdTokenList[i]);

		isRun = CreateProcess(NULL, cmdStringWithOptions, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);

		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);

		if(isRun == FALSE)
			_tprintf(ERROR_CMD, cmdTokenList[0]);
	}

	return 0;
}


TCHAR * StrLower(TCHAR *pStr)
{
	TCHAR *ret = pStr;

	while(*pStr)
	{
		if(_istupper(*pStr))
			*pStr = _totlower(*pStr);

		pStr++;
	}

	return ret;
}



/*************************************************************************************
sort�� ���μ����� ����....
**************************************************************************************/
#define MAX_STRING_NUM 100
#define MAX_STRING_LEN 256

void SortString()  // ������ ������ ���ؼ� ������ �������Ϸ� ������ �Ѵ�(��Ʈ).
{
	TCHAR stringArr[MAX_STRING_NUM][MAX_STRING_LEN];

	// �ַܼκ��� ���ڿ��� �о���δ�.
	int nStr;
	for(nStr=0; nStr<MAX_STRING_NUM; nStr++)
	{
		TCHAR * isEOF = _fgetts(stringArr[nStr], MAX_STRING_LEN, stdin);
		if(isEOF == NULL)    // EOF�� Ctrl+Z..
			break;
	}

	// String Bubble Sort... ���� ������� �ʰ� ���ڿ� ���� ����...
	TCHAR strTemp[MAX_STRING_LEN];

	for(int i=0; i<nStr; i++)
	{
		for(int j= nStr-1; j>i; j--)
		{
			if( _tcscmp(stringArr[j-1], stringArr[j]) >0 )
			{
				_tcscpy(strTemp, stringArr[j-1]);
				_tcscpy(stringArr[j-1], stringArr[j]);
				_tcscpy(stringArr[j], strTemp); 
			}
		}
	}

	for(int i=0; i<nStr; i++)
		_fputts(stringArr[i], stdout);
}

int _tmain_sortmain(int argc, TCHAR * argv[])
{
	SortString();

	return 0;
}

/*************************************************************************************
type�� ���μ����� ����....
**************************************************************************************/
void TYPE(TCHAR * fileName)	// FOR REDIRECTION...
{
	static TCHAR StringBuff[1024];

	FILE * filePtr = _tfopen(fileName, _T("rt"));
	while(_fgetts(StringBuff, 1024, filePtr))
		_fputts(StringBuff, stdout);
}

int _tmain_typemain(int argc, TCHAR * argv[])
{
	if(argc <2)
		return -1; 

	TYPE(argv[1]);

	return 0;
}

