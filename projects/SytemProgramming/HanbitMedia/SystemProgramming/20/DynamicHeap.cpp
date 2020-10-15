/*
	DynamicHeap.cpp
	프로그램 설명: 힙의 컨트롤에 대한 이해.
*/
#include <stdio.h>
#include <stdlib.h>
#include <windows.h> 

int main(int argc, TCHAR *argv[])
{
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);	
	UINT pageSize = sysInfo.dwPageSize;

	// 1. 힙의 생성.
	HANDLE hHeap = HeapCreate(HEAP_NO_SERIALIZE, pageSize * 10, pageSize * 100);

	// 2. 메모리 할당.
	int * p = (int *)HeapAlloc(hHeap, 0, sizeof(int) * 10);

	// 3. 메모리 활용.
	for(int i=0; i<10; i++)
		p[i] = i;

	// 4. 메모리 해제 
	HeapFree(hHeap, 0, p);

	// 5. 힙 소멸.
	HeapDestroy(hHeap);


	HANDLE hDefaultHeap = GetProcessHeap();
	TCHAR * pDefault = (TCHAR *)HeapAlloc(hDefaultHeap, HEAP_NO_SERIALIZE, sizeof(TCHAR) *10);
	_tcscpy(pDefault, _T("Default Heap!"));
	_tprintf(_T("%s \n"), pDefault);
	HeapFree(hDefaultHeap, HEAP_NO_SERIALIZE, pDefault);

	return 1;
}


