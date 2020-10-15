// dllmain.cpp : Implementation of DllMain.

#include "pch.h"
#include "framework.h"
#include "resource.h"
#include "TESTDLLCOM_i.h"
#include "dllmain.h"
#include "xdlldata.h"

#include <iostream>
using namespace std;

CTESTDLLCOMModule _AtlModule;

// DLL Entry Point
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        cout << "TESTDLLCOM:DLL_PROCESS_ATTACH: " << ul_reason_for_call
            << "," << (lpReserved == nullptr ? "not null" : "null") << endl;
        break;
    case DLL_THREAD_ATTACH:
        cout << "TESTDLLCOM:DLL_THREAD_ATTACH: " << ul_reason_for_call
            << "," << (lpReserved == nullptr ? "not null" : "null") << endl;
        break;
    case DLL_THREAD_DETACH:
        cout << "TESTDLLCOM:DLL_THREAD_DETACH: " << ul_reason_for_call
            << "," << (lpReserved == nullptr ? "not null" : "null") << endl;
        break;
    case DLL_PROCESS_DETACH:
        cout << "TESTDLLCOM:DLL_PROCESS_DETACH: " << ul_reason_for_call
            << "," << (lpReserved == nullptr ? "not null" : "null") << endl;
        break;
    }

#ifdef _MERGE_PROXYSTUB
	if (!PrxDllMain(hInstance, ul_reason_for_call, lpReserved))
		return FALSE;
#endif
	hInstance;
	return _AtlModule.DllMain(ul_reason_for_call, lpReserved);
}
