/*
	DynamicArray.cpp
	프로그램 설명: SEH 기반의 가상메모리 컨트롤.
*/
#include <stdio.h>
#include <stdlib.h>
#include <windows.h> 

#define MAX_PAGE    10           // 할당할 최대 페이지 개수.

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
  pageSize = sSysInfo.dwPageSize;	// 페이지 사이즈 참조.

  // MAX_PAGE의 개수만큼 페이지 RESERVE!
  baseAddr = VirtualAlloc(
                NULL,           // 임의 주소 할당.    
                MAX_PAGE * pageSize, // 예약 메모리 크기.
                MEM_RESERVE,         // RESERVE!
                PAGE_NOACCESS);       // NO ACCESS.
  if (baseAddr == NULL )
    _tprintf(_T("VirtualAlloc reserve failed"));

  lpPtr = (int*) baseAddr;			// 배열의 시작번지와 같은 의미 부여.
  nextPageAddr = (int*) baseAddr;

  // page fault 발생시 예외 발생. 
  for (int i=0; i < (MAX_PAGE * pageSize) / sizeof(int); i++)
  {
	  __try
	  {
		  // Write to memory.
		  lpPtr[i] = i;
	  }
	  __except ( PageFaultExceptionFilter( GetExceptionCode() ) )
	  {
		  // 적절한 예외처리(페이지 할당)가 이뤄지지 않으면 프로세스 종료!
		  ExitProcess( GetLastError() );
	  }
  }

  //for(int i=0; i < (MAX_PAGE * pageSize) / sizeof(int); i++)
  //    _tprintf(_T("%d "), lpPtr[i]);

  // 메모리 해제
  BOOL isSuccess = VirtualFree(
                baseAddr,      // 해제할 메모리의 시작번지.
                0,            // 세번째 인자가 MEM_RELEASE 이므로 반드시 0.
                MEM_RELEASE);  // FREE 상태로 변경.

  if(isSuccess)
	  _tprintf( _T("Release succeeded!"));
  else
	  _tprintf( _T("Release failed!"));
}


int PageFaultExceptionFilter(DWORD exptCode)
{
  // 예외의 원인이 'page fault'가 아니라면.
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

  // page fault가 발생한 지점부터 실행을 계속.
  return EXCEPTION_CONTINUE_EXECUTION;
}
