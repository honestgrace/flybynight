#include "stdafx.h"

#include <string>
#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>

using namespace std;
namespace TestClassFile
{
    class Dog
    {
    private:
        string _name;

    public:
        Dog()
        {
            _name = "";
        }

        Dog(string name)
            : _name(name)
        {
        }
        virtual ~Dog()
        {
        }

        virtual void Bark()
        {
            cout << "Dog" << endl;
        }

        string GetName()
        {
            return _name;
        }

        void SetName(string newValue)
        {
            _name = newValue;
        }
    };

    class Leg
    {
    public:
        void Run()
        {
            cout << "Running" << endl;
        }
    };

    class YellowDog : public Dog
    {
    private:
        Leg _leg;

    public:
        YellowDog(string name)
        {
            SetName(name);
        }

        ~YellowDog()
        {

        }

        void Bark()
        {
            cout << "YellowDog" << endl;
        }

        void Run()
        {
            _leg.Run();
        }
    };
}

class LogFile
{
    std::mutex m_mutex;
    ofstream f;
public:
    LogFile()
    {
        f.open("log.txt");
    }
    void Print(string msg)
    {
        std::lock_guard<std::mutex> guard(m_mutex);  // this will fix 1) issue
        f << msg << endl;
    }
    ~LogFile()
    {
        f.close();
    }

    // Never return f to the outside world
    /* ofstream& getStream() { return f; } */
    
    // Never pass f as an argument to user provided function
    /*
    void processf(void fun(ofstream&))
    {
        fun(f);
    }
    */
};

class Fctor
{
public:
    void operator()(LogFile & log)
    {
        for (int i = 0; i > -100; i--)
            log.Print("Fctor");
    }
};

auto Lambda = []()
{
    cout << "Lambda" << endl;
};

class IRequest3 {
public:
	void virtual ReadEntity() = 0;
};

class IRequest4 {
public:
	void virtual ReadEntityEx() = 0;
};

class Request : public IRequest3, public IRequest4
{
public:
	void virtual ReadEntityEx() {
		cout << "ReadEntityEx called" << endl;
	}
	void virtual ReadEntity() {
		cout << "ReadEntity called" << endl;
	}
};

void TestClass(int argc, char **argv)
{
	Request request;
	IRequest3* pRequest3 = &request;
	IRequest4* pRequest4 = &request;
	pRequest3->ReadEntity();
	pRequest4->ReadEntityEx();

	VOID* pointer = reinterpret_cast<VOID *>(pRequest4);

	IRequest4* test;
	test = reinterpret_cast<IRequest4 *>(pRequest4);
	test->ReadEntityEx();

	IRequest4* test2;
	test2 = reinterpret_cast<IRequest4*>(pRequest3);
	test2->ReadEntityEx();

	/*

    //Fctor f;
    //thread t1(f);
    // thread t1(Lambda);
    string msg = "t1";

    LogFile* pLog = new LogFile();
    thread t1((Fctor()), std::ref(*pLog));
    thread t2 = std::move(t1);
    
    try
    {
        // to do while waiting t1 thread
        // throw 111;
    }
    catch (...)
    {
        t2.join();
        throw;
    }

    for (int i = 0; i > -100; i--)
        pLog->Print("From main");

    t2.join();
    delete pLog;

    cout << msg << endl;
	*/
}