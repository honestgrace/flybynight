#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <TxDtc.h>
#include <xoleHlp.h>
#include <oletx2xa.h>
#include "classes.h"

#pragma comment(lib, "xolehlp.lib")

ITransaction * pXATransactionWithXA = nullptr;
ITransactionImport * pXAITransactionImportWithXA = nullptr;
ITransactionExport * pXAITransactionExportWithXA  = nullptr;
ITransactionEnlistmentAsync * pXATransactionWithXAEnlistmentAsync = nullptr;
TransactionResourceAsync * pTransactionResourceAsyncWithXA = nullptr;

void ReEnlist_xatm_DoublePipeWithXA(ITransactionEnlistmentAsync * pTransactionEnlistmentAsync, IResourceManager * pIResourceManager, XACTSTAT * pTransactionOutput)
{
	//*******************
	// Create PrepareInfo object using TransactionEnlistmentAsync object
	//*******************	
	IPrepareInfo2 * pIPrepareInfo2 = nullptr;
	HR( pTransactionEnlistmentAsync -> QueryInterface( __uuidof(IPrepareInfo2), (void **) &pIPrepareInfo2 ) );
	ULONG prepareInfoSize = -1;
		
	HRESULT hr = pIPrepareInfo2 -> GetPrepareInfoSize(&prepareInfoSize);
	HR( pIPrepareInfo2 -> GetPrepareInfoSize(&prepareInfoSize) );
	byte * prepareInfo = new byte[prepareInfoSize];
	HR( pIPrepareInfo2 -> GetPrepareInfo( prepareInfoSize, prepareInfo ) );
		

	//*******************
	// Reenlist in order to get the transaction output
	//*******************	
	HR( pIResourceManager -> Reenlist(prepareInfo, prepareInfoSize, XACTCONST_TIMEOUTINFINITE, pTransactionOutput ) );
		
	//*******************
	// Release COM object and memory
	//*******************
	pIPrepareInfo2 -> Release();
	delete [] prepareInfo;
}

int testDtc_E2E_xatm_DoublePipeWithXA()
{
	//*******************
    // Create XA Transaction
    //*******************
	XID myXid;
	myXid.bqual_length = 0;
	myXid.formatID = 0;
	myXid.gtrid_length = 0;
	int myXARmId = 53;

	int xaerr = XA_OK;
    strcpy_s( myXid.data, "FAIL" );
    myXid.gtrid_length = strlen( myXid.data );
	
	//*******************
	// create transaction with calling XA functions directly
	// JHKIM debugging this is not figured out yet.
	//*******************
	/* bool commitWithXA = true;
	if (commitWithXA)
	{
		xaerr = msqlsrvxa1.xa_open_entry("TM=xaresumetx,RmRecoveryGuid=8FABAA36-FDC5-4bb7-9DFD-DF1CF312A000,Timeout=30000", myXARmId, 0);
		if (XA_OK != xaerr)
		{
			printf("Error %d from xa_open\n", xaerr);
			return -1;
		}
		
		Sleep(1000);
		
		xaerr = msqlsrvxa1.xa_start_entry(&myXid, myXARmId, 0);
		if (XA_OK != xaerr)
		{
			printf("Error %d from xa_start\n", xaerr);
			return -1;
		}

		Sleep(1000);
		
		xaerr = msqlsrvxa1.xa_prepare_entry(&myXid, myXARmId, 0);
		if (XA_OK != xaerr)
		{
			printf("Error %d from xa_prepare.\n", xaerr);
			return -1;
		}

		Sleep(1000);
		
		xaerr = msqlsrvxa1.xa_commit_entry(&myXid, myXARmId, 0);
		if (XA_OK != xaerr)
		{
			printf("Error %d from xa_commit.\n", xaerr);
			return -1;
		}

		Sleep(1000);
		
		//xaerr = msqlsrvxa1.xa_recover_entry(&myXid, myXARmId, 0, TMSTARTRSCAN | TMENDRSCAN);
		//if (XA_OK != xaerr)
		//{
		//printf("Error %d from xa_recover.\n", xaerr);
		//return -1;
		//}		

		Sleep(1000);
		
		xaerr = msqlsrvxa1.xa_close_entry("TM=xaresumetx,RmRecoveryGuid=8FABAA36-FDC5-4bb7-9DFD-DF1CF312A000,Timeout=30000", myXARmId, 0);
		if (XA_OK != xaerr)
		{
			printf("Error %d from xa_close.\n", xaerr);
			return -1;
		}

		Sleep(1000);
	}
	*/

    xaerr = msqlsrvxa1.xa_open_entry("TM=xaresumetx,RmRecoveryGuid=8FABAA36-FDC5-4bb7-9DFD-DF1CF312A000,Timeout=30000", myXARmId, 0 );
    if ( XA_OK != xaerr )
    {
        printf( "Error %d from xa_open\n", xaerr );
        return -1;
    }

    xaerr = msqlsrvxa1.xa_start_entry(&myXid, myXARmId, 0);
    if ( XA_OK != xaerr )
    {
        printf( "Error %d from xa_start\n", xaerr );
        return -1;    
	}    

	//*******************
    // Get TransactionDispenser object
    //*******************
    ITransactionDispenser * pTransactionDispenser = nullptr;
    HR( DtcGetTransactionManager( nullptr, nullptr, __uuidof(ITransactionDispenser), 0, 0, (void *) nullptr, (void **) &pTransactionDispenser ) );
	
	//*******************
    // QI IDtcToXaMapper object
    //*******************
    IDtcToXaMapper * pIDtcToXaMapper = nullptr;
	HR(pTransactionDispenser->QueryInterface(__uuidof(IDtcToXaMapper), (void **) &pIDtcToXaMapper) );
	
	//*******************
    // Create XA RM
    //*******************
    DWORD rmCookie1 = 0; 
	HR(pIDtcToXaMapper->RequestNewResourceManager( "MyDSN", "XARMDll.dll", &rmCookie1 ) );
	
    //*******************
    // Get pTransaction from XID
    //*******************      
    IXATransLookup2 * pXaLookup = nullptr;
    HR(pTransactionDispenser->QueryInterface( IID_IXATransLookup2, (void**) &pXaLookup ) );
	HR(pXaLookup->Lookup( &myXid, &pXATransactionWithXA) );

	//*******************
    // Enlist XA ResourceManager
    //*******************
    HR(pIDtcToXaMapper->EnlistResourceManager( rmCookie1, (DWORD*) pXATransactionWithXA ));
    
	//*******************
    // Call TranslateTridToXid to check XID can be created correctly
    //*******************
    XID xid;
	HR(pIDtcToXaMapper->TranslateTridToXid( (DWORD*) pXATransactionWithXA, rmCookie1, &xid  ));

    //*******************
    // Create ResourceManager object using IResourceManagerFactory
    //*******************
    IResourceManagerFactory * pResourceManagerFactory = nullptr;
    HR( pTransactionDispenser -> QueryInterface( __uuidof(IResourceManagerFactory), (void **) &pResourceManagerFactory ) );     
    IResourceManager * pIResourceManager = nullptr;
    ResourceManagerSink resourceManagerSink;
    resourceManagerSink.AddRef();
    GUID guid;
    HR( CoCreateGuid(&guid));
    HR( pResourceManagerFactory -> Create( &guid, "ResourceManager1", &resourceManagerSink, &pIResourceManager ) );

    //*******************
    // Get WhereAboutSize
    //*******************             
    ITransactionImportWhereabouts * pITransactionImportWhereabouts = nullptr;
    HR( pTransactionDispenser -> QueryInterface( __uuidof(ITransactionImportWhereabouts), (void **) &pITransactionImportWhereabouts ) );     
    ULONG whereAboutSize = -1;
    HR( pITransactionImportWhereabouts -> GetWhereaboutsSize(&whereAboutSize ) );

    //*******************
    // Create WhereAbout object
    //*******************             
    byte * pWhereAbouts = new byte[whereAboutSize];
    ULONG whereAboutsSizeUsed = -1;
    HR( pITransactionImportWhereabouts -> GetWhereabouts( whereAboutSize, pWhereAbouts, &whereAboutsSizeUsed ) );

    //*******************
    // Create Export object using ITransactionExportFactory
    //*******************             
    ITransactionExportFactory * pITransactionExportFactory = nullptr;
    HR( pTransactionDispenser -> QueryInterface( __uuidof(ITransactionExportFactory), (void **) &pITransactionExportFactory ) );  
    HR( pITransactionExportFactory -> Create( whereAboutsSizeUsed, pWhereAbouts, &pXAITransactionExportWithXA ) );

    //*******************
    // Export the transaction object
    //*******************             
    ULONG transactionCookieSize = -1; 
    HR( pXAITransactionExportWithXA -> Export( pXATransactionWithXA, &transactionCookieSize ) );

    //*******************
    // Get transaction cookie
    //*******************             
    byte * pTransactionCookie = new byte[transactionCookieSize];
    ULONG transactionCookieSize2 = -1;
    HR( pXAITransactionExportWithXA -> GetTransactionCookie( pXATransactionWithXA, transactionCookieSize, pTransactionCookie, &transactionCookieSize2 ) );

    //*******************
    // Get Import object
    //*******************             
    HR( pTransactionDispenser -> QueryInterface( __uuidof(ITransactionImport), (void **) &pXAITransactionImportWithXA ) );  

    //*******************
    // Import transaction object
    //*******************             
    ITransaction * pTransaction_Imported = nullptr;
    IID iidOfItransaction = __uuidof(ITransaction);
    HR( pXAITransactionImportWithXA -> Import( transactionCookieSize2, pTransactionCookie, &iidOfItransaction, (void **) &pTransaction_Imported ) );
      
	//*******************
	// Check the original transaction and exported transaction
	//*******************

	XACTTRANSINFO txInfo;
	HR( pXATransactionWithXA -> GetTransactionInfo(&txInfo) );	
	XACTTRANSINFO txInfo2;
	HR( pTransaction_Imported -> GetTransactionInfo(&txInfo2) );

    //*******************
    // Enlist resoure manager
    //*******************      
    XACTUOW transactionUUID;
    LONG isolationLevel;
	pTransactionResourceAsyncWithXA = new TransactionResourceAsync();
    pTransactionResourceAsyncWithXA->AddRef();
    HR( pIResourceManager -> Enlist( pTransaction_Imported, pTransactionResourceAsyncWithXA, &transactionUUID, &isolationLevel, &pXATransactionWithXAEnlistmentAsync ) );
	pTransactionResourceAsyncWithXA->SaveContext(pXATransactionWithXAEnlistmentAsync, transactionUUID, isolationLevel);
    
	//*******************
    // Call TranslateTridToXid with imported Transaction object to check XID can be created correctly
    //*******************
    XID xid2;
	HR(pIDtcToXaMapper->TranslateTridToXid( (DWORD*) pTransaction_Imported, rmCookie1, &xid2  ));
    for (int i = 0; i<14; i++)
	{
		if (xid.data[i] != xid2.data[i])
		{
			throw -1;  // xid.data is not matched 
		}
	}

	//*******************
    // do not call CommitRequestDone() to simulate the accidental transaction commit failure
    //*******************    
	pTransactionResourceAsyncWithXA->m_donotCommit = false;
	
	//*******************
    // call Commit or Abort
	//*******************
	bool committing = true;
	if (committing)
	{
		xaerr = msqlsrvxa1.xa_commit_entry(&myXid, myXARmId, TMONEPHASE);
        if ( XA_OK != xaerr )
		{
			printf( "Error %d from xa_commit.\n", xaerr );
			return -1;
		}

		//HR( pXATransactionWithXA -> Commit( false, 0, 0 ) );		
	}
	else
	{
		xaerr = msqlsrvxa1.xa_rollback_entry(&myXid, myXARmId, TMONEPHASE);
        if ( XA_OK != xaerr )
		{
			printf( "Error %d from xa_rollback.\n", xaerr );
			return -1;
		}
	    //HR( pXATransactionWithXA -> Abort( false, 0, 0 ) );
	}

	Sleep(1000);
		
	//*******************
    // Reenlist the resource manager to recover the failed transaction
    //*******************
    if (committing && pTransactionResourceAsyncWithXA->m_donotCommit)
	{
		XACTSTAT transactionOutput;		
		XACTSTAT * pTransactionOutput = &transactionOutput;		
		ReEnlist_xatm_DoublePipeWithXA(pXATransactionWithXAEnlistmentAsync, pIResourceManager, pTransactionOutput);

		XACTSTAT transactionOutput2;				
		XACTSTAT * pTransactionOutput2 = &transactionOutput2;		
		ReEnlist_xatm_DoublePipeWithXA(pXATransactionWithXAEnlistmentAsync, pIResourceManager, pTransactionOutput2);

		//*******************
		// Commit or abort according to the transaction output
		//*******************	
		if (transactionOutput == XACTSTAT::XACTSTAT_ABORTED)
		{
			printf("Transaction was aborted\n");
			HR( pXATransactionWithXAEnlistmentAsync -> AbortRequestDone(S_OK) );	
		} 
		else if (transactionOutput == XACTSTAT::XACTSTAT_COMMITTED)
		{
			printf("Transaction was committed...\n");
			HR( pXATransactionWithXAEnlistmentAsync -> CommitRequestDone(S_OK) );		
		}

        //*******************
		// Complete Reenlisting
		//*******************	
		HR( pIResourceManager -> ReenlistmentComplete() );
    }

	//*******************
    // Release XA Resource Manager
    //*******************
    HR(pIDtcToXaMapper->ReleaseResourceManager( rmCookie1 ) );	

    //*******************
    // Release COM objects and memory
    //*******************
	delete pTransactionResourceAsyncWithXA; 

	pIDtcToXaMapper -> Release();
	pTransaction_Imported -> Release();      
    pXAITransactionImportWithXA -> Release();
    pXAITransactionExportWithXA -> Release();
    pITransactionExportFactory -> Release();
    pITransactionImportWhereabouts -> Release();
    pResourceManagerFactory -> Release();
    pTransactionDispenser -> Release();
	delete [] pTransactionCookie;
    delete [] pWhereAbouts;
	
	return 0;
}

