#pragma once

#include <Windows.h>
#include <thread>
#include <mutex>
#include "testdll.h"

class TestCallDll_Mutex
{
private:
    LONG64 _total;
    HANDLE lock;
    CRITICAL_SECTION cs;

public:
    typedef bool(__cdecl* MYPROC) ();

    void Do()
    {
        //initialize mutex
        lock = CreateMutex(
            NULL,  // default security attributes
            FALSE, //initialize not owned
            NULL);  //unnamed

        //initialize critical section
        if (!InitializeCriticalSectionAndSpinCount(&cs, 0x400))
        {
            Log("Failed to initialize Critical Section");
            return;
        }

        //InitializeCriticalSection(&cs);

        _total = 0;

        const int numberOfThread = 10;

        std::thread threads[numberOfThread];
        for (int i = 0; i < numberOfThread; i++)
            threads[i] = std::thread(&TestCallDll_Mutex::DoWork, this, i);

        for (int i = 0; i < numberOfThread; i++)
            threads[i].join();

        Log("TestCallDll_Mutex Total: " + std::to_string(_total));

        CloseHandle(lock);
        DeleteCriticalSection(&cs);
    }

    void DoWork(int id)
    {
        Log("TestCallDll_Mutex, Thread " + std::to_string(id));
        Log("begin of TestCallDll_Object::Do");

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
                    /*
                    // Scenario 1. using InterlockedIncrement64
                    LONG64* pointer = &_total;
                    InterlockedIncrement64(pointer);
                    if (_total % 10000 == 0)
                        Log("TestCallDll_Mutex total " + std::to_string(_total));
                    */

                    /*
                    // Scenario 2. using mutex
                    // own lock
                    DWORD dwWaitResult = WaitForSingleObject(
                        lock,       // handle to mutex
                        INFINITE);  // no time-out interval
                    switch (dwWaitResult)
                    {
                    case WAIT_OBJECT_0:
                        _total++;
                        if (!ReleaseMutex(lock))
                        {
                            Log("TestCallDll_Mutex Failed to release Mutex");
                        }

                        if (_total % 10000 == 0)
                            Log("TestCallDll_Mutex total " + std::to_string(_total));

                        break;
                    case WAIT_ABANDONED:
                        Log("TestCallDll_Mutex Failed to own Mutex");
                    }
                    */

                    // Scenario 3. using critical section
                    EnterCriticalSection(&cs);
                    _total++;
                    if (_total % 10000 == 0)
                        Log("TestCallDll_Mutex total " + std::to_string(_total));
                    LeaveCriticalSection(&cs);
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
        Log("end of TestCallDll_Mutex::Do");
    }
};