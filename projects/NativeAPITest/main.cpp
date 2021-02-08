// NativeAPITest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
using namespace std;

#include <string>
#include <iostream>

int ProcessInformation();
int JobObject();
void TestCreateProcess();
void TestCreateProcessAsUser();
void TestThread();
void TestEvent();
void CreateThreadForStackTracking();
void TestDuplicateHandle();
void TestInterlockedCompareExchange();
void TestGetFileAttributes();
void TestReleaseSRWLockExclusive();
int TestWinSockClient(int argc, char **argv);
int TestIOCompletionPort(int argc, char **argv);
int __cdecl TestWinSockServer(int argc, char **argv);
int __cdecl TestIOCPWinSockServer(int argc, char **argv);
int TestRawSocket();
void TestDBCS();
void TestCreateThreadImpersonate(int argc, char **argv);
void TestClass(int argc, char **argv);
void TestRegEx(int argc, char **argv); 
void WinDbg();
void TestStandardLibrary(int newArgc, char **argv);
void TestHeapCorruption();
void TestHeap();
int TestCtrlHandler(void);
void TestSetup();

int __cdecl main(int argc, char **argv)
{
	string command;
    if (argc == 1)
    {
        WinDbg();
    }
    else
	{
		command = string(argv[argc - 1]);
		int newArgc = argc - 1;
        if (command == "TestStandardLibrary") TestStandardLibrary(newArgc, argv);
        if (command == "TestRegEx") TestRegEx(newArgc, argv);
        if (command == "TestDBCS") TestDBCS();
		if (command == "TestRawSocket") TestRawSocket();
		if (command == "TestIOCompletionPort") TestIOCompletionPort(newArgc, argv);
		if (command == "TestWinSockClient") TestWinSockClient(newArgc, argv);
		if (command == "TestWinSockServer") TestWinSockServer(newArgc, argv);
		if (command == "TestIOCPWinSockServer") TestIOCPWinSockServer(newArgc, argv);
		if (command == "TestGetFileAttributes") TestGetFileAttributes();
		if (command == "TestInterlockedCompareExchange") TestInterlockedCompareExchange();
		if (command == "TestDuplicateHandle") TestDuplicateHandle();
		if (command == "TestHeapCorruption") TestHeapCorruption();
		if (command == "TestHeap") TestHeap();
		if (command == "CreateThreadForStackTracking") CreateThreadForStackTracking();
		if (command == "TestCreateProcess") TestCreateProcess();
		if (command == "TestCreateProcessAsUser") TestCreateProcessAsUser();
		if (command == "TestReleaseSRWLockExclusive") TestReleaseSRWLockExclusive();
		if (command == "TestThread") TestThread();
		if (command == "TestEvent") TestEvent();
		if (command == "JobObject") JobObject();
		if (command == "ProcessInformation") ProcessInformation();
        if (command == "TestCreateThreadImpersonate") TestCreateThreadImpersonate(newArgc, argv);
        if (command == "TestClass") TestClass(newArgc, argv);
        if (command == "TestCtrlHandler") TestCtrlHandler();
		if (command == "TestSetup") TestSetup();
	}
	return 0;
}


