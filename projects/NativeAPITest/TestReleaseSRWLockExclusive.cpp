#include "stdafx.h"
#include "ProcessStarter.h"
#include "string"
#include <thread>
#include <iostream>

using namespace std;

SRWLOCK TestReleaseSRWLockExclusive_lock;

typedef struct object_s {
	int data;
} object_t, *object_p;

void TestReleaseSRWLockShared_Task(object_p x)
{
	for (int i = 0; i < 100; i++)
	{
		AcquireSRWLockShared(&TestReleaseSRWLockExclusive_lock);
		//...do something that could probably change x->data value to 0
		if (x->data == 0)
		{
			free(x);
		}
		else
		{
			cout << "Shared" << x->data << endl;
		}
		ReleaseSRWLockShared(&TestReleaseSRWLockExclusive_lock);
	}
}

void TestReleaseSRWLockExclusive_Task(object_p x)
{
	for (int i = 0; i < 100; i++)
	{
		AcquireSRWLockExclusive(&TestReleaseSRWLockExclusive_lock);
		//...do something that could probably change x->data value to 0
		if (x->data == 0)
		{
			free(x);
		}
		else
		{
			cout << "Exclusive" << x->data << endl;
		}
		ReleaseSRWLockExclusive(&TestReleaseSRWLockExclusive_lock);
	}
}

void TestReleaseSRWLockExclusive()
{
	object_p object = (object_p)malloc(sizeof(object_t));
	object->data = 100;
	InitializeSRWLock(&TestReleaseSRWLockExclusive_lock);

	thread t(TestReleaseSRWLockExclusive_Task, object);
	thread t2(TestReleaseSRWLockShared_Task, object);
	thread t3(TestReleaseSRWLockShared_Task, object);
	cout << "t join" << endl;
	t.join();
	cout << "t2 join" << endl;
	t2.join();
	cout << "t3 join" << endl;
	t3.join();
}
