#pragma once

#include <Windows.h>
#include "testdll.h"

class TestCallDll
{
public:
	typedef bool(__cdecl* MYPROC) ();

	static void Do()
	{
        Log("begin of TestCallDll::Do");

        bool test = false;
        test = Foo();

        if (test)
            Log("Passed");
        else
            Log("Failed");


        HINSTANCE hinstLib;
        MYPROC procAdd;
        BOOL fFreeLib = FALSE;

        hinstLib = LoadLibrary(TEXT("testdll2.dll"));

        if (hinstLib != NULL)
        {
            procAdd = (MYPROC)GetProcAddress(hinstLib, "Foo");
            if (procAdd != NULL)
            {
                test = false;
                test = procAdd();

                if (test)
                    Log("Passed");
                else
                    Log("Failed");

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
        Log("end of TestCallDll::Do");
	}
};