#define TESTDLL_EXPORTS = 1
#include "pch.h"
#include "testdll.h"

bool Foo()
{
	cout << "Foo()" << endl;
	return true;
}

bool Log(string inputstring)
{
	cout << inputstring << endl;
	return true;
}

class TestStaticClass
{
public:
    static void StaticMethod()
    {
        cout << "TestStaticClass::StaticMethod" << endl;
    }

    TestStaticClass()
    {
        StaticMethod();
        cout << "TestStaticClass constructor" << endl;
    }

    ~TestStaticClass()
    {
        cout << "TestStaticClass destructor" << endl;
    }
};

TestStaticClass singletone;