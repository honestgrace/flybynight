#include "stdafx.h"

#define ARRAY_SIZE 10
BOOL InitArray(int** pPtrArray){	return FALSE;}

void TestUninitializedMemory(){	int iRes = 1;
	int** pPtrArray = (int**)HeapAlloc(GetProcessHeap(),		0,		sizeof(int* [ARRAY_SIZE]));	if (pPtrArray != NULL)	{		InitArray(pPtrArray);		*(pPtrArray[0]) = 10;		iRes = 0;		HeapFree(GetProcessHeap(), 0, pPtrArray);	}}

void TestUnallocatedHeap()
{
	BYTE *pAlloc1, *pAlloc2, *pAlloc3;	HANDLE hProcessHeap;
	char data = 'a';
	int size = 1000000;
	hProcessHeap = GetProcessHeap();
	pAlloc1 = (BYTE*)HeapAlloc(hProcessHeap, 0, size);	pAlloc2 = (BYTE*)malloc(size);
	pAlloc3 = (BYTE*)new char[size];
	*((char*)pAlloc1) = data;
	*((char*)(pAlloc1 + 1)) = data;
	*((char*)(pAlloc1 + 2)) = data;
	*((char*)(pAlloc1 + 3)) = '\0';

	// Using unallocated memory
	//*((char*)pAlloc1 - 1) = data;
	printf("%s", (char*) pAlloc1);
	HeapFree(hProcessHeap, 0, pAlloc1);	free(pAlloc2);	delete pAlloc3;	printf("test is completed");}void TestHeap(){	TestUnallocatedHeap();	TestUninitializedMemory();}