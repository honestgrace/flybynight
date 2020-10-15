// This project is based on:
// https://docs.microsoft.com/en-us/cpp/build/walkthrough-creating-and-using-a-dynamic-link-library-cpp?view=vs-2019

#include "TestCallDLL.h"
#include "TestCallDLL_MultiThread.h"
#include "TestCallDLL_Object.h"
#include "TestCallDll_Mutex.h"
#include "TestCallDll_COM.h"
#include "TestInterface.h"

LONG64 TestCallDll_MultiThread::_total = 0;
std::mutex TestCallDll_MultiThread::lock;

int main()
{
    //TestCallDll::Do();    
    //TestCallDll_MultiThread::Do();
    //TestCallDll_Object().Do();
    TestCallDll_Mutex().Do();
    //TestCallDll_COM().Do();
    //TestInterface().Do();
}