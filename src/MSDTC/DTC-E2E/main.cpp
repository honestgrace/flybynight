#include "stdafx.h"
#include <windows.h>
#include <stdio.h>


int testEvent();
int testDtc();
int testDtc_MultipleResourceManager();
int testDtc_Voter();
int testDtc_Voter2();
int testDtc_E2E();
int testDtc_E2E_Rejoin();
int testDtc_E2E_Rejoin_WithResourceManager2();
int testDtc_E2E_pull();
int testDtc_ChooseDTC();
int testDtc_SmartPointer();
int testDtc_MultipleResourceManager_Rejoin();
int testDtc_E2E_xa_DoublePipe();
int testDtc_E2E_xa_SinglePipe();
int testDtc_E2E_xa_SinglePipe_DtcXaHelperFactory();
int testDtc_E2E_xatm_DoublePipe();
int testDtc_E2E_xatm_SinglePipe();
int testDtc_E2E_xatm_SinglePipe_DtcXaHelperFactory();
int testDtc_E2E_xatm_DoublePipeWithXA();

int main( void )
{
	//testEvent();
	//testDtc();
	//testDtc_MultipleResourceManager();
	//testDtc_Voter();
    //testDtc_Voter2();
	//testDtc_E2E();	
	//testDtc_E2E_pull();	
	//testDtc_ChooseDTC();
	//testDtc_SmartPointer();
	//testDtc_E2E_Rejoin();	
	//testDtc_MultipleResourceManager_Rejoin();
	//testDtc_E2E_Rejoin_WithResourceManager2();
	//testDtc_E2E_xa_DoublePipe();
	//testDtc_E2E_xa_SinglePipe();
	//testDtc_E2E_xa_SinglePipe_DtcXaHelperFactory();
	//testDtc_E2E_xatm_DoublePipe();
	//testDtc_E2E_xatm_SinglePipe();
	//testDtc_E2E_xatm_SinglePipe_DtcXaHelperFactory();
	testDtc_E2E_xatm_DoublePipeWithXA();
	return 0;
}