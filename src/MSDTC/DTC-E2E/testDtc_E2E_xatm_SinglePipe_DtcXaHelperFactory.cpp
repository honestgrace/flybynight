#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <TxDtc.h>
#include <xoleHlp.h>
#include <oletx2xa.h>
#include "classes.h"

#pragma comment(lib, "xolehlp.lib")
#pragma comment(lib, "xaswitch.lib")

ITransaction * pXXX2_Transaction = nullptr;
ITransactionImport * pXXX2_ITransactionImport = nullptr;
ITransactionExport * pXXX2_ITransactionExport  = nullptr;
ITransactionEnlistmentAsync * pXXX2_TransactionEnlistmentAsync_xa = nullptr;
ITransactionEnlistmentAsync * pXXX2_TransactionEnlistmentAsync_oletx = nullptr;
TransactionResourceAsync * pXXX2_transactionResourceAsync_xa = nullptr;
TransactionResourceAsync * pXXX2_transactionResourceAsync_oletx = nullptr;
    
void ReEnlist_XXX2(ITransactionEnlistmentAsync * pTransactionEnlistmentAsync, IResourceManager * pIResourceManager, XACTSTAT * pTransactionOutput)
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

int testDtc_E2E_xatm_SinglePipe_DtcXaHelperFactory()
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
    IDtcToXaHelperFactory * pIDtcToXaHelperFactory = nullptr;
	HR(pTransactionDispenser->QueryInterface(__uuidof(IDtcToXaHelperFactory), (void **) &pIDtcToXaHelperFactory) );
		
	//*******************
    // Create XA RM
    //*******************
    GUID guidXaRm;
	IDtcToXaHelper * pIDtcToXaHelper = nullptr;
	HR(pIDtcToXaHelperFactory->Create( "ignore", "XARMDll.dll", &guidXaRm, &pIDtcToXaHelper ) );
	
	//*******************
    // Create ResourceManager object using IResourceManagerFactory
    //*******************
    IResourceManagerFactory * pResourceManagerFactory_xa = nullptr;
    HR( pTransactionDispenser -> QueryInterface( __uuidof(IResourceManagerFactory), (void **) &pResourceManagerFactory_xa ) );     
    IResourceManager * pIResourceManager_xa = nullptr;
    ResourceManagerSink resourceManagerSink_xa;
    resourceManagerSink_xa.AddRef();
    HR( pResourceManagerFactory_xa -> Create( &guidXaRm, "ResourceManager_xa", &resourceManagerSink_xa, &pIResourceManager_xa ) );

    //*******************
    // Get pTransaction from XID
    //*******************      
    IXATransLookup2 * pXXX2_Lookup = nullptr;
    HR(pTransactionDispenser->QueryInterface( IID_IXATransLookup2, (void**) &pXXX2_Lookup ) );
	HR(pXXX2_Lookup->Lookup( &myXid, &pXXX2_Transaction) );

	//*******************
    // Enlist XA ResourceManager
    //*******************
	 XACTUOW transactionUUID_xa;
    LONG isolationLevel_xa;
    pXXX2_transactionResourceAsync_xa = new TransactionResourceAsync();
	pXXX2_transactionResourceAsync_xa->AddRef();

    HR( pIResourceManager_xa -> Enlist( pXXX2_Transaction, pXXX2_transactionResourceAsync_xa, &transactionUUID_xa, &isolationLevel_xa, &pXXX2_TransactionEnlistmentAsync_xa ) );
    pXXX2_transactionResourceAsync_xa->SaveContext( pXXX2_TransactionEnlistmentAsync_xa, transactionUUID_xa, isolationLevel_xa);
	//pXXX2_transactionResourceAsync_xa->EnableXA(&myXid, myXARmId);

	//*******************
    // Call TranslateTridToXid to check XID can be created correctly
    //*******************
    XID xid;
	HR(pIDtcToXaHelper->TranslateTridToXid( pXXX2_Transaction, &guidXaRm, &xid ));

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
    HR( pITransactionExportFactory -> Create( whereAboutsSizeUsed, pWhereAbouts, &pXXX2_ITransactionExport ) );

    //*******************
    // Export the transaction object
    //*******************             
    ULONG transactionCookieSize = -1; 
    HR( pXXX2_ITransactionExport -> Export( pXXX2_Transaction, &transactionCookieSize ) );

    //*******************
    // Get transaction cookie
    //*******************             
    byte * pTransactionCookie = new byte[transactionCookieSize];
    ULONG transactionCookieSize2 = -1;
    HR( pXXX2_ITransactionExport -> GetTransactionCookie( pXXX2_Transaction, transactionCookieSize, pTransactionCookie, &transactionCookieSize2 ) );

    //*******************
    // Get Import object
    //*******************             
    HR( pTransactionDispenser -> QueryInterface( __uuidof(ITransactionImport), (void **) &pXXX2_ITransactionImport ) );  

    //*******************
    // Import transaction object
    //*******************             
    ITransaction * pTransaction_Imported = nullptr;
    IID iidOfItransaction = __uuidof(ITransaction);
    HR( pXXX2_ITransactionImport -> Import( transactionCookieSize2, pTransactionCookie, &iidOfItransaction, (void **) &pTransaction_Imported ) );
      
	//*******************
	// Check the original transaction and exported transaction
	//*******************

	XACTTRANSINFO txInfo;
	HR( pXXX2_Transaction -> GetTransactionInfo(&txInfo) );	
	XACTTRANSINFO txInfo2;
	HR( pTransaction_Imported -> GetTransactionInfo(&txInfo2) );

    //*******************
    // Enlist resoure manager
    //*******************      
    XACTUOW transactionUUID;
    LONG isolationLevel;
	pXXX2_transactionResourceAsync_oletx = new TransactionResourceAsync();
    pXXX2_transactionResourceAsync_oletx->AddRef();
    HR( pIResourceManager -> Enlist( pTransaction_Imported, pXXX2_transactionResourceAsync_oletx, &transactionUUID, &isolationLevel, &pXXX2_TransactionEnlistmentAsync_oletx ) );
	pXXX2_transactionResourceAsync_oletx->SaveContext(pXXX2_TransactionEnlistmentAsync_oletx, transactionUUID, isolationLevel);
    
	//*******************
    // Call TranslateTridToXid with imported Transaction object to check XID can be created correctly
    //*******************
    XID xid2;
	HR(pIDtcToXaHelper->TranslateTridToXid( pTransaction_Imported, &guidXaRm, &xid2 ));

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
	pXXX2_transactionResourceAsync_oletx->m_donotCommit = false;

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
 	}
	else
	{
		xaerr = msqlsrvxa1.xa_rollback_entry(&myXid, myXARmId, TMONEPHASE);
        if ( XA_OK != xaerr )
		{
			printf( "Error %d from xa_rollback.\n", xaerr );
			return -1;
		}
	}

	Sleep(1000);

	//*******************
    // Reenlist the resource manager to recover the failed transaction
    //*******************
    if (committing && pXXX2_transactionResourceAsync_oletx->m_donotCommit)
	{
		XACTSTAT transactionOutput;		
		XACTSTAT * pTransactionOutput = &transactionOutput;		
		ReEnlist_XXX2(pXXX2_TransactionEnlistmentAsync_oletx, pIResourceManager, pTransactionOutput);

		XACTSTAT transactionOutput2;				
		XACTSTAT * pTransactionOutput2 = &transactionOutput2;		
		ReEnlist_XXX2(pXXX2_TransactionEnlistmentAsync_oletx, pIResourceManager, pTransactionOutput2);

		//*******************
		// Commit or abort according to the transaction output
		//*******************	
		if (transactionOutput == XACTSTAT::XACTSTAT_ABORTED)
		{
			printf("Transaction was aborted\n");
			HR( pXXX2_TransactionEnlistmentAsync_oletx -> AbortRequestDone(S_OK) );	
		} 
		else if (transactionOutput == XACTSTAT::XACTSTAT_COMMITTED)
		{
			printf("Transaction was committed...\n");
			HR( pXXX2_TransactionEnlistmentAsync_oletx -> CommitRequestDone(S_OK) );		
		}

        //*******************
		// Complete Reenlisting
		//*******************	
		HR( pIResourceManager -> ReenlistmentComplete() );
    }

	//*******************
    // Release XA Resource Manager
    //*******************
    bool doXARecovery = false;
	pIDtcToXaHelper->Close(doXARecovery);

    //*******************
    // Release COM objects and memory
    //*******************
	delete pXXX2_transactionResourceAsync_oletx; 
	delete pXXX2_transactionResourceAsync_xa;
 	
	pIDtcToXaHelper -> Release();
	pTransaction_Imported -> Release();      
    pXXX2_ITransactionImport -> Release();
    pXXX2_ITransactionExport -> Release();
    pITransactionExportFactory -> Release();
    pITransactionImportWhereabouts -> Release();
    pResourceManagerFactory -> Release();
    pTransactionDispenser -> Release();
	delete [] pTransactionCookie;
    delete [] pWhereAbouts;

	return 0;
}

