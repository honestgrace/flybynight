#pragma once

#include <Windows.h>
#include <thread>
#include <mutex>
#include "testdll.h"

class TestCallDll_Object
{
private:
    LONG64 _total;
    std::mutex lock;

public:
    typedef bool(__cdecl* MYPROC) ();

    void Do()
    {
        _total = 0;

        const int numberOfThread = 10;

        std::thread threads[numberOfThread];
        for (int i = 0; i < numberOfThread; i++)
            threads[i] = std::thread(&TestCallDll_Object::DoWork, this, i);

        for (int i = 0; i < numberOfThread; i++)
            threads[i].join();

        Log("TestCallDll_Object Total: " + std::to_string(_total));

    }

    void DoWork(int id)
    {
        Log("TestCallDll_Object, Thread " + std::to_string(id));
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
                    //
                    // Critical section
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
        Log("end of TestCallDll_Object::Do");
    }
};