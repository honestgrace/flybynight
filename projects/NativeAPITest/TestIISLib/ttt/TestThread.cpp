#include "stdafx.h"
#include "ProcessStarter.h"
#include "string"
#include <thread>
#include <iostream>

using namespace std;

void TestThread_Task(int i)
{
	for (int j = 0; j<10; j++)
		cout << "test" << endl;
}

void TestThread()
{
	auto rambdaFunction = []() {
		for (int j = 0; j<10; j++)
			cout << "test2" << endl;
	};

	thread t(TestThread_Task, 1);
	rambdaFunction();
	
	for (int i = 0; i<5000; i++)
	cout << "main" << endl;
	t.join();
	//t2.join();
}

