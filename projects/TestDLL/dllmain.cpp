// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        cout << "DLL_PROCESS_ATTACH: " << ul_reason_for_call
            << "," << (lpReserved == nullptr ? "not null" : "null") << endl;
        break;
    case DLL_THREAD_ATTACH:
        cout << "DLL_THREAD_ATTACH: " << ul_reason_for_call
            << "," << (lpReserved == nullptr ? "not null" : "null") << endl;
        break;
    case DLL_THREAD_DETACH:
        cout << "DLL_THREAD_DETACH: " << ul_reason_for_call
            << "," << (lpReserved == nullptr ? "not null" : "null") << endl;
        break;
    case DLL_PROCESS_DETACH:
        cout << "DLL_PROCESS_DETACH: " << ul_reason_for_call
            << "," << (lpReserved == nullptr ? "not null" : "null") << endl;
        break;
    }
    return TRUE;
}