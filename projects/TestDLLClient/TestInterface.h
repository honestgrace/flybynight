#pragma once

#include <Windows.h>
#include <thread>
#include <mutex>
#include "..\TestDLL\testdll.h"
#include <iostream>
using namespace std;

class ITestInterface
{
public:
    virtual void Do() = 0;
};

class ITestInterface2 : public ITestInterface
{
};

class TestInterfaceContext;

class TestInterfaceBase : public ITestInterface2
{
public:
    TestInterfaceBase()
    {
    }
    void Do();
    TestInterfaceContext* Me();
};

class TestInterfaceContext : public TestInterfaceBase
{
private:
    int _id;
public:
    TestInterfaceContext(int id) : _id(id)
    {
    }
    //void Do();
};

class TestInterfaceMain : public TestInterfaceContext
{
public:
    TestInterfaceMain() : TestInterfaceContext(555)
    {
    }
};

void TestInterfaceBase::Do()
{
    cout << "TestInterfaceBase::Do";
    Me()->Do();
}

TestInterfaceContext * TestInterfaceBase::Me()
{
    return (TestInterfaceContext * )this;
}

//void TestInterfaceContext::Do()
//{
//    cout << "TestInterfaceContext::Do";
//}

class TestInterface
{
private:
    TestInterfaceMain* _p;

public:
    TestInterface()
    {
        _p = new TestInterfaceMain();
    }

    ~TestInterface()
    {
        delete _p;
    }

    void Do()
    {
        ITestInterface* pointer;
        pointer = (ITestInterface *)_p;
        pointer->Do();

        ITestInterface2* pointer2 = (ITestInterface2 *)pointer;
        pointer2->Do();

        pointer = reinterpret_cast<TestInterfaceBase*>(_p);
        pointer = (TestInterfaceBase*)_p;
        //*ppInterface2 = reinterpret_cast<IHttpContext3*>(pInterface1);
        
        pointer->Do();

        TestInterfaceBase* pp = reinterpret_cast<TestInterfaceBase*>(_p);
        pp->Do();
    }
};