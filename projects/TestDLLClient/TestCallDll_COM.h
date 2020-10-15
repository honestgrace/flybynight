#pragma once

#include <Windows.h>
#include <thread>
#include <mutex>
#include "testdll.h"
#include <objbase.h>

using namespace std;

#include "\gitroot\tools\projects\TESTDLLCOM\TESTDLLCOM_i.c"
#include "\gitroot\tools\projects\TESTDLLCOM\TESTDLLCOM_i.h"

class TestCallDll_COM
{
public:

    void Do()
    {
        CoInitializeEx(nullptr, COINIT::COINIT_MULTITHREADED);
        MessageBox(0, L"test", L"test", 0);

        ILogger* pLogger;
        HRESULT hr = CoCreateInstance(
            CLSID_Logger,
            nullptr,
            CLSCTX_INPROC_SERVER, IID_ILogger,
            (LPVOID*) &pLogger);

        if (SUCCEEDED(hr))
        {
            wstring inputData(L"Hello I am testing COM object");
            BSTR inputString = SysAllocStringLen(inputData.data(), inputData.size());

            pLogger->AddRef();
            pLogger->Log(inputString);
            pLogger->Release();

            SysFreeString(inputString);
        }
        CoUninitialize();
    }
};