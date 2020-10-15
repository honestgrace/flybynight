#include "stdafx.h"

#define ARRAY_SIZE 10
BOOL InitArray(int** pPtrArray)

void TestUninitializedMemory()
	int** pPtrArray = (int**)HeapAlloc(GetProcessHeap(),

void TestUnallocatedHeap()
{
	BYTE *pAlloc1, *pAlloc2, *pAlloc3;
	char data = 'a';
	int size = 1000000;
	hProcessHeap = GetProcessHeap();
	pAlloc1 = (BYTE*)HeapAlloc(hProcessHeap, 0, size);
	pAlloc3 = (BYTE*)new char[size];
	*((char*)pAlloc1) = data;
	*((char*)(pAlloc1 + 1)) = data;
	*((char*)(pAlloc1 + 2)) = data;
	*((char*)(pAlloc1 + 3)) = '\0';

	// Using unallocated memory
	//*((char*)pAlloc1 - 1) = data;
	printf("%s", (char*) pAlloc1);
	HeapFree(hProcessHeap, 0, pAlloc1);