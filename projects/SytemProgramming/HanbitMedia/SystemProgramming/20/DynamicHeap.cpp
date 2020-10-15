/*
	DynamicHeap.cpp
	���α׷� ����: ���� ��Ʈ�ѿ� ���� ����.
*/
#include <stdio.h>
#include <stdlib.h>
#include <windows.h> 

int main(int argc, TCHAR *argv[])
{
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);	
	UINT pageSize = sysInfo.dwPageSize;

	// 1. ���� ����.
	HANDLE hHeap = HeapCreate(HEAP_NO_SERIALIZE, pageSize * 10, pageSize * 100);

	// 2. �޸� �Ҵ�.
	int * p = (int *)HeapAlloc(hHeap, 0, sizeof(int) * 10);

	// 3. �޸� Ȱ��.
	for(int i=0; i<10; i++)
		p[i] = i;

	// 4. �޸� ���� 
	HeapFree(hHeap, 0, p);

	// 5. �� �Ҹ�.
	HeapDestroy(hHeap);


	HANDLE hDefaultHeap = GetProcessHeap();
	TCHAR * pDefault = (TCHAR *)HeapAlloc(hDefaultHeap, HEAP_NO_SERIALIZE, sizeof(TCHAR) *10);
	_tcscpy(pDefault, _T("Default Heap!"));
	_tprintf(_T("%s \n"), pDefault);
	HeapFree(hDefaultHeap, HEAP_NO_SERIALIZE, pDefault);

	return 1;
}


