/*
	TerminationHandlerEx2.cpp
	프로그램 설명: 뮤텍스 반환에 관련 종료 핸들러 구성.
*/

#include <stdio.h>
#include <windows.h>
#include <process.h>

#define NUM_OF_GATE		7

LONG gTotalCount = 0;

HANDLE hMutex;

void IncreaseCount()
{
	__try
	{
		WaitForSingleObject(hMutex, INFINITE);
		gTotalCount++;
	}
	__finally
	{
		ReleaseMutex(hMutex);
	}
}


unsigned int WINAPI ThreadProc( LPVOID lpParam ) 
{ 
	for(DWORD i=0; i<1000; i++)
	{
		IncreaseCount();
	}

	return 0;
} 


int _tmain(int argc, TCHAR* argv[])
{
    DWORD dwThreadIDs[NUM_OF_GATE];
    HANDLE hThreads[NUM_OF_GATE];

    hMutex = CreateMutex (NULL, FALSE, NULL);

    if (hMutex == NULL) 
    {
        _tprintf(_T("CreateMutex error: %d\n"), GetLastError());
    }

	for(DWORD i=0; i<NUM_OF_GATE; i++)
	{
        hThreads[i] = (HANDLE)
		    _beginthreadex ( 
			    NULL,
			    0,				        
			    ThreadProc,				  
			    NULL,                    
			    CREATE_SUSPENDED,		   
			    (unsigned *)&dwThreadIDs[i]   
	        );

	    if(hThreads[i] == NULL)
	    {
	        _tprintf(_T("Thread creation fault! \n"));
	        return -1;
	    }
	}

	for(DWORD i=0; i<NUM_OF_GATE; i++)
	{
		ResumeThread(hThreads[i]);
	}


	WaitForMultipleObjects(NUM_OF_GATE, hThreads, TRUE, INFINITE);

	_tprintf(_T("total count: %d \n"), gTotalCount);

	for(DWORD i=0; i<NUM_OF_GATE; i++)
	{
		CloseHandle(hThreads[i]);
	}
  
	CloseHandle(hMutex);

	return 0;
}

