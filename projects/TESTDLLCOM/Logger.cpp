// Logger.cpp : Implementation of CLogger

#include "pch.h"
#include "Logger.h"
#include <iostream>
using namespace std;

// CLogger
DOUBLE g_testcaseNumber = 0;

STDMETHODIMP CLogger::get_TestCaseNumber(DOUBLE* pVal)
{
    *pVal = g_testcaseNumber;
    return S_OK;
}


STDMETHODIMP CLogger::put_TestCaseNumber(DOUBLE newVal)
{
    g_testcaseNumber = newVal;
    return S_OK;
}


STDMETHODIMP CLogger::Log(BSTR message)
{
    wstring inputData(message, SysStringLen(message));
    wcout << inputData << endl;
    return S_OK;
}
