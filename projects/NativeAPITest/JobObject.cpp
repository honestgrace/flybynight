#include "stdafx.h"

#include <iostream>

#define _WIN32_WINNT 0x05010000

#include <windows.h>
#include <winbase.h>

void pause()
{
	std::cin.sync();
	std::cout << "\nPAUSED\n";
	std::cin.ignore();
}

int JobObject()
{
	TCHAR ProcName[] = L"Notepad.exe";
	TCHAR ProcName2[] = L"MMC.exe";

	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(pi));

	PROCESS_INFORMATION pi2;
	ZeroMemory(&pi2, sizeof(pi));

	STARTUPINFO si;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);

	STARTUPINFO si2;
	ZeroMemory(&si2, sizeof(si2));
	si2.cb = sizeof(si2);

	HANDLE Jobject = CreateJobObject(NULL, L"JHKIM Job");

	JOBOBJECT_EXTENDED_LIMIT_INFORMATION JBLI;

	try
	{
		if (Jobject == NULL)
		{
			throw "COULD NOT CREATE JOB OBJECT:";
		}

		if (!AssignProcessToJobObject(Jobject, OpenProcess(PROCESS_ALL_ACCESS, FALSE, GetCurrentProcessId())))
		{
			throw "COULD NOT ASSIGN SELF TO JOB OBJECT!:";
		}

		if (!QueryInformationJobObject(Jobject, JobObjectExtendedLimitInformation, &JBLI, sizeof(JBLI), NULL))
		{
			throw "COULD NOT QUERY JOB OBJECT INFORMATION CLASS!:";
		}

		JBLI.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_BREAKAWAY_OK;

		if (!SetInformationJobObject(Jobject, JobObjectExtendedLimitInformation, &JBLI, sizeof(JBLI)))
		{
			throw "COULD NOT SET JOB OBJECT INFORMATION CLASS!:";
		}

		if (!CreateProcess(NULL, ProcName, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
		{
			throw "COULD NOT CREATE CHILD PROCESS!";
		}		

		if (!CreateProcess(NULL, ProcName2, NULL, NULL, FALSE, CREATE_BREAKAWAY_FROM_JOB, NULL, NULL, &si2, &pi2))
		{
			throw "COULD NOT CREATE CHILD PROCESS with CREATE_BREAKAWAY_FROM_JOB!";
		}

		std::cout << "Child Process ID: " << pi.dwProcessId << "\tThread ID: " << pi.dwThreadId << "\tHandle: " << pi.hProcess << "\n";
	}
	catch (char* Message)
	{
		std::cout << Message << " " << GetLastError() << "\n";
	}

	pause();

	TerminateJobObject(Jobject, 0);
	pause();

	TerminateProcess(pi.hProcess, 1816);
	TerminateProcess(pi2.hProcess, 1816);

	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);

	CloseHandle(pi2.hThread);
	CloseHandle(pi2.hProcess);

	CloseHandle(Jobject);

	return 0;
}
