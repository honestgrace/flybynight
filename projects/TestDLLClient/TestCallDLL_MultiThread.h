#pragma once

#include <Windows.h>
#include <thread>
#include <mutex>
#include "testdll.h"

class TestCallDll_MultiThread
{
private:
    static LONG64 _total;
    static std::mutex lock;

public:
    typedef bool(__cdecl* MYPROC) ();

    static void Do()
    {
        _total = 0;

        const int numberOfThread = 10;

        std::thread threads[numberOfThread];
        for (int i = 0; i < numberOfThread; i++)
            threads[i] = std::thread(DoWork, i);

        for (int i = 0; i < numberOfThread; i++)
            threads[i].join();

        Log("TestCallDll_MultiThread Total: " + std::to_string(_total));

    }

    static void DoWork(int id)
    {
        Log("TestCallDll_MultiThread, Thread " + std::to_string(id));
        Log("begin of TestCallDll_MultiThread::Do");

        HINSTANCE hinstLib;
        MYPROC procAdd;
        BOOL fFreeLib = FALSE;

        hinstLib = LoadLibrary(TEXT("testdll2.dll"));

        if (hinstLib != NULL)
        {
            procAdd = (MYPROC)GetProcAddress(hinstLib, "Foo");
            if (procAdd != NULL)
            {
                bool test = false;
                test = procAdd();

                if (test)
                    Log("Passed");
                else
                    Log("Failed");

                for (int i = 0; i < 1000000; i++)
                {
                    //
                    // Critical section
                    //
                    //_total++;

                    //
                    // Scenario 1: Using InterlockedIncrement64
                    //
                    LONG64* pointer = &_total;
                    InterlockedIncrement64(pointer);

                    //
                    // Scenario 2: Using mutex lock
                    //

                    //
                    // BugBug: there is a problem if we mix InterlockedIncrement64 and mutex
                    // with the same variable. That should be avoided.
                    //
                    lock.lock();
                    _total++;
                    lock.unlock();
                }

                fFreeLib = FreeLibrary(hinstLib);

                if (test)
                    Log("Success: FreeLibrary");
                else
                    Log("Error: FreeLibrary");
            }
            else
            {
                Log("Error: GetProcAddress");
            }
        }
        else
        {
            Log("Error: LoadLibrary");
        }
        Log("end of TestCallDll_MultiThread::Do");
    }
};