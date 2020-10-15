#include "stdafx.h"
#include <iostream>
using namespace std;

enum TestEventEventState
{
	START,
	ONGOING,
	END
};

struct TestEventEventContext
{
	HANDLE event;
	TestEventEventState state = TestEventEventState::START;
	int data = 0;
};

DWORD CALLBACK TestEventThreadFunction(LPVOID pParam)
{
	TestEventEventContext* pContext = (TestEventEventContext*)pParam;
	if (pContext->state == TestEventEventState::START)
	{
		SetEvent(pContext->event);
		ResetEvent(pContext->event);
		pContext->state = TestEventEventState::ONGOING;
	}
	WaitForSingleObject(pContext->event, INFINITE);

	while (1)
	{
		pContext->data++;
		if (pContext->state == TestEventEventState::END)
		{
			break;
		}
	}
	return 0;
}

void TestEvent()
{
	TestEventEventContext* pContext = new TestEventEventContext;
	pContext->event = CreateEvent(NULL, TRUE, TRUE, L"MyEvent");
	ResetEvent(pContext->event);
	HANDLE threadHandle = CreateThread(NULL, 0, TestEventThreadFunction, (LPVOID)pContext, NULL, NULL);
	WaitForSingleObject(pContext->event, INFINITE);
	
	cout << "Thread started" << endl;
	SetEvent(pContext->event);
	for (int i = 0; i < 500; i++)
	{
		cout << pContext->data << endl;
	}
	pContext->state = TestEventEventState::END;

	WaitForSingleObject(threadHandle, INFINITE);
	delete pContext;
}