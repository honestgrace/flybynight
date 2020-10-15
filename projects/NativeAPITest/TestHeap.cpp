#include "stdafx.h"

#define ARRAY_SIZE 10
BOOL InitArray(int** pPtrArray)
{
	return FALSE;
}

void TestUninitializedMemory()
{
	printf("\nBegin TestUninitializedMemory\r\n");
	int iRes = 1;
	int** pPtrArray = (int**)HeapAlloc(GetProcessHeap(),
		0,
		sizeof(int* [ARRAY_SIZE]));
	if (pPtrArray != NULL)
	{
		InitArray(pPtrArray);
		__try
		{
			printf("Executing SEH test\r\n");
			// hardware error
			//*(pPtrArray[0]) = 10;			

			// software error
			throw 0;
		}
		//__except (EXCEPTION_EXECUTE_HANDLER)
		//__except (EXCEPTION_CONTINUE_EXECUTION)
		//__except (EXCEPTION_CONTINUE_SEARCH)
		//{
		//	printf("Executing SEH __except block\r\n");
		//}
		__finally {
			printf("Executing SEH __finally block\r\n");
		}
		iRes = 0;
		HeapFree(GetProcessHeap(), 0, pPtrArray);
	}
	printf("End TestUninitializedMemory\r\n");
}

void TestUninitializedMemoryWithoutErrorHandling()
{
	printf("\nBegin TestUninitializedMemoryWithoutErrorHandling\r\n");
	int iRes = 1;
	int** pPtrArray = (int**)HeapAlloc(GetProcessHeap(),
		0,
		sizeof(int* [ARRAY_SIZE]));
	if (pPtrArray != NULL)
	{
		InitArray(pPtrArray);
		// hardware error
		//*(pPtrArray[0]) = 10;			

		// software error
		throw 0;

		iRes = 0;
		HeapFree(GetProcessHeap(), 0, pPtrArray);
	}
	printf("End TestUninitializedMemoryWithoutErrorHandling\r\n");
}


void TestUnallocatedHeap()
{
	BYTE *pAlloc1, *pAlloc2, *pAlloc3;
	HANDLE hProcessHeap;
	char data = 'a';
	int size = 1000000;
	hProcessHeap = GetProcessHeap();
	pAlloc1 = (BYTE*)HeapAlloc(hProcessHeap, 0, size);
	pAlloc2 = (BYTE*)malloc(size);
	pAlloc3 = (BYTE*)new char[size];
	*((char*)pAlloc1) = data;
	*((char*)(pAlloc1 + 1)) = data;
	*((char*)(pAlloc1 + 2)) = data;
	*((char*)(pAlloc1 + 3)) = '\0';

	// Using unallocated memory
	//*((char*)pAlloc1 - 1) = data;
	printf("%s", (char*) pAlloc1);
	HeapFree(hProcessHeap, 0, pAlloc1);
	free(pAlloc2);
	delete pAlloc3;
	printf("test is completed");
}

class TestBasicClass {
public:
	int a[1024];
	int b[1024];
};

class TestBasicClassSmall {
public:
	int a;
	int b;
};


void TestBasic()
{
	TestBasicClass* test = nullptr;
	for (int j = 0; j < 1000; j++)
	{
		test = new TestBasicClass;
		for (int i = 0; i < 1024; i++)
		{
			test->a[i] = 9;
			test->b[i] = 9;
		}
	}

	TestBasicClassSmall* test2 = nullptr;
	for (int j = 0; j < 1000; j++)
	{
		test2 = new TestBasicClassSmall;
		test2->a = 8;
		test2->b = 8;
	}
}

void TestHeap()
{
	TestBasic();
	/*
	TestUnallocatedHeap();
	TestUninitializedMemory();
	TestUninitializedMemoryWithoutErrorHandling();
	*/
}