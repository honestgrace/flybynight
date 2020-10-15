/* 
	pipe1.c
	read | write 와 같은 형태의 입력 가능!
	상속에 의한 방법!
	여러가지 방법중 하나!
*/
#define _UNICODE // for unicode
#define UNICODE	 // for unicode

#define SEPCHARS _T(" |\t\n")

#include <windows.h>
#include <tchar.h>
#include <stdio.h>

int _tmain(int argc, LPTSTR argv[])
{
	/* Process 생성에 필요한 변수의 선언 */
	STARTUPINFO si_1={0,};
	STARTUPINFO si_2={0,};
	si_1.cb = sizeof(si_1);
	si_2.cb = sizeof(si_2);

    PROCESS_INFORMATION pi_1, pi_2;

	BOOL state;
	DWORD errorState;
	TCHAR szCommand[50];

	HANDLE hReadPipe, hWritePipe; //pipe handle
	SECURITY_ATTRIBUTES pSA ={
		sizeof(SECURITY_ATTRIBUTES),
		NULL,
		TRUE
	}; //앞의 예제 코드와 비교해서 설명해야 한다. 

	TCHAR szToken1[20];  // First Command
	TCHAR szToken2[20];  // Second Command
	PTSTR pszToken;		 // Token Pointer

	/* Command 입력 */
	_fputts(_T("MY CMD>"), stdout);
	fgetws(szCommand, sizeof(szCommand)/sizeof(TCHAR), stdin);

	/* Pipe 생성 */
	CreatePipe(&hReadPipe, &hWritePipe, &pSA, 0);

	/* Command 추출 */
	pszToken=wcstok(szCommand, SEPCHARS);
	lstrcpy(szToken1, pszToken);

	pszToken=wcstok(NULL, SEPCHARS);
	lstrcpy(szToken2, pszToken);


	/* child process1 생성 */
	si_1.hStdInput=GetStdHandle(STD_INPUT_HANDLE);
	si_1.hStdError=GetStdHandle(STD_ERROR_HANDLE); //이 부분 주석처리 가능한지 확인하자!
	si_1.hStdOutput=hWritePipe;  //이 또한 리다이랙션이다.
	si_1.dwFlags=STARTF_USESTDHANDLES;

	state=CreateProcess(NULL,					
						szToken1,				
						NULL,					
						NULL,					
						TRUE,					
						0,		                
						NULL,			       
						NULL,				
						&si_1,				
						&pi_1 		   
		);		
	if(state==FALSE){
		errorState=GetLastError();
		_tprintf(_T("Error Code: %d \n"), errorState);
	}
	else{
		CloseHandle(pi_1.hThread);
		CloseHandle(hWritePipe);
	}

	/* child process2 생성 */
	si_2.hStdInput=hReadPipe;  //이 또한 리다이랙션이다.
	si_2.hStdError=GetStdHandle(STD_ERROR_HANDLE); //이 부분 주석처리 가능한지 확인하자!
	si_2.hStdOutput=GetStdHandle(STD_OUTPUT_HANDLE);
	si_2.dwFlags=STARTF_USESTDHANDLES;

	state=CreateProcess(NULL,					// No module name (use command line).
						szToken2,				// Command line. 
						NULL,					// Process handle not inheritable. 
						NULL,					// Thread handle not inheritable. 
						TRUE,					// Set handle inheritance to FALSE. 
						0,		                // No creation flags. 
						NULL,			        // Use parent's environment block. 
						NULL,					// Use parent's starting directory. 
						&si_2,					// Pointer to STARTUPINFO structure.
						&pi_2 		            // Pointer to PROCESS_INFORMATION structure.
		);
	if(state==FALSE){
		errorState=GetLastError();
		_tprintf(_T("Error Code: %d \n"), errorState);
	}
	else{
		CloseHandle(pi_2.hThread);
		CloseHandle(hReadPipe);
	}
	
	WaitForSingleObject(pi_1.hProcess, INFINITE);
	WaitForSingleObject(pi_2.hProcess, INFINITE);
	CloseHandle(pi_1.hProcess);
	CloseHandle(pi_2.hProcess);	

	return 0;
}
