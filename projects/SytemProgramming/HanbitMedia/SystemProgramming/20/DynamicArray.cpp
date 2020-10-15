/*
	DynamicArray.cpp
	���α׷� ����: SEH ����� ����޸� ��Ʈ��.
*/
#include <stdio.h>
#include <stdlib.h>
#include <windows.h> 

#define MAX_PAGE    10           // �Ҵ��� �ִ� ������ ����.

int* nextPageAddr;
DWORD pageCnt = 0;
DWORD pageSize;

int PageFaultExceptionFilter(DWORD);

int main(int argc, TCHAR *argv[])
{
  LPVOID baseAddr; 
  int* lpPtr;      
  SYSTEM_INFO sSysInfo;

  GetSystemInfo(&sSysInfo);
  pageSize = sSysInfo.dwPageSize;	// ������ ������ ����.

  // MAX_PAGE�� ������ŭ ������ RESERVE!
  baseAddr = VirtualAlloc(
                NULL,           // ���� �ּ� �Ҵ�.    
                MAX_PAGE * pageSize, // ���� �޸� ũ��.
                MEM_RESERVE,         // RESERVE!
                PAGE_NOACCESS);       // NO ACCESS.
  if (baseAddr == NULL )
    _tprintf(_T("VirtualAlloc reserve failed"));

  lpPtr = (int*) baseAddr;			// �迭�� ���۹����� ���� �ǹ� �ο�.
  nextPageAddr = (int*) baseAddr;

  // page fault �߻��� ���� �߻�. 
  for (int i=0; i < (MAX_PAGE * pageSize) / sizeof(int); i++)
  {
	  __try
	  {
		  // Write to memory.
		  lpPtr[i] = i;
	  }
	  __except ( PageFaultExceptionFilter( GetExceptionCode() ) )
	  {
		  // ������ ����ó��(������ �Ҵ�)�� �̷����� ������ ���μ��� ����!
		  ExitProcess( GetLastError() );
	  }
  }

  //for(int i=0; i < (MAX_PAGE * pageSize) / sizeof(int); i++)
  //    _tprintf(_T("%d "), lpPtr[i]);

  // �޸� ����
  BOOL isSuccess = VirtualFree(
                baseAddr,      // ������ �޸��� ���۹���.
                0,            // ����° ���ڰ� MEM_RELEASE �̹Ƿ� �ݵ�� 0.
                MEM_RELEASE);  // FREE ���·� ����.

  if(isSuccess)
	  _tprintf( _T("Release succeeded!"));
  else
	  _tprintf( _T("Release failed!"));
}


int PageFaultExceptionFilter(DWORD exptCode)
{
  // ������ ������ 'page fault'�� �ƴ϶��.
  if (exptCode != EXCEPTION_ACCESS_VIOLATION)
  {
	  _tprintf( _T("Exception code = %d \n"), exptCode);
	  return EXCEPTION_EXECUTE_HANDLER;
  }

  _tprintf( _T("Exception is a page fault \n"));

   if (pageCnt >= MAX_PAGE)
  {
	  _tprintf( _T("Exception: out of pages \n") );
	  return EXCEPTION_EXECUTE_HANDLER;
  }

 
  LPVOID lpvResult = 
	  VirtualAlloc (
				(LPVOID) nextPageAddr, // next page to commit
				pageSize,         // page size, in bytes
				MEM_COMMIT,         // allocate a committed page
				PAGE_READWRITE    // read/write access
	  );
  if (lpvResult == NULL )
  {
	  _tprintf( _T("VirtualAlloc failed %d \n"));
	  return EXCEPTION_EXECUTE_HANDLER;
  } 
  else 
  {
	  _tprintf ( _T("Allocating another page. \n") );
  }

  pageCnt++;
  nextPageAddr += pageSize/sizeof(int);

  // page fault�� �߻��� �������� ������ ���.
  return EXCEPTION_CONTINUE_EXECUTION;
}
