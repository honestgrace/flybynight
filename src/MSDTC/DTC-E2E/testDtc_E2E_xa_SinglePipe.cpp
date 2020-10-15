#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <TxDtc.h>
#include <xoleHlp.h>
#include <oletx2xa.h>
#include "classes.h"

#pragma comment(lib, "xolehlp.lib")

void ReEnlist_xa_SinglePipe(ITransactionEnlistmentAsync * pTransactionEnlistmentAsync, IResourceManager * pIResourceManager, XACTSTAT * pTransactionOutput)
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


int testDtc_E2E_xa_SinglePipe()
{
	//*******************
    // Get TransactionDispenser object
    //*******************
    ITransactionDispenser * pTransactionDispenser = nullptr;
    HR( DtcGetTransactionManager( nullptr, nullptr, __uuidof(ITransactionDispenser), 0, 0, (void *) nullptr, (void **) &pTransactionDispenser ) );
	
	//*******************
    // QI IDtcToXaHelperSinglePipe object
    //*******************
    IDtcToXaHelperSinglePipe * pIDtcToXaHelperSinglePipe = nullptr;
	HR(pTransactionDispenser->QueryInterface(__uuidof(IDtcToXaHelperSinglePipe), (void **) &pIDtcToXaHelperSinglePipe) );
	
	//*******************
    // Create XA RM
    //*******************
    DWORD rmCookie1 = 0; 
	HR(pIDtcToXaHelperSinglePipe->XARMCreate( "MyDSN", "XARMDll.dll", &rmCookie1 ) );
	
    //*******************
    // Create a new transaction object with calling BeginTranaction()
    //*******************      
    ITransaction * pTransaction = nullptr;
    HR( pTransactionDispenser -> BeginTransaction( nullptr, ISOLATIONLEVEL::ISOLATIONLEVEL_UNSPECIFIED, 0, nullptr, &pTransaction ) );

	//*******************
    // Enlist XA ResourceManager
    //*******************
	TransactionResourceAsync transactionResourceAsync_forXARM;
    ITransactionEnlistmentAsync * pTransactionEnlistmentAsync_forXARM;
    transactionResourceAsync_forXARM.AddRef();
    HR(pIDtcToXaHelperSinglePipe->EnlistWithRM( rmCookie1, pTransaction, &transactionResourceAsync_forXARM,  &pTransactionEnlistmentAsync_forXARM ));
    
	transactionResourceAsync_forXARM.SaveContext( pTransactionEnlistmentAsync_forXARM);

	//*******************
    // Get XID
    //*******************
    XID xid;
	HR(pIDtcToXaHelperSinglePipe->ConvertTridToXID( (DWORD*) pTransaction, rmCookie1, &xid  ));

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
    ITransactionExport * pITransactionExport  = nullptr;
	HR( pITransactionExportFactory -> Create( whereAboutsSizeUsed, pWhereAbouts, &pITransactionExport ) );

    //*******************
    // Export the transaction object
    //*******************             
    ULONG transactionCookieSize = -1; 
    HR( pITransactionExport -> Export( pTransaction, &transactionCookieSize ) );

    //*******************
    // Get transaction cookie
    //*******************             
    byte * pTransactionCookie = new byte[transactionCookieSize];
    ULONG transactionCookieSize2 = -1;
    HR( pITransactionExport -> GetTransactionCookie( pTransaction, transactionCookieSize, pTransactionCookie, &transactionCookieSize2 ) );

    //*******************
    // Get Import object
    //*******************             
    ITransactionImport * pITransactionImport = nullptr;
    HR( pTransactionDispenser -> QueryInterface( __uuidof(ITransactionImport), (void **) &pITransactionImport ) );  

    //*******************
    // Import transaction object
    //*******************             
    ITransaction * pTransaction_Imported = nullptr;
    IID iidOfItransaction = __uuidof(ITransaction);
    HR( pITransactionImport -> Import( transactionCookieSize2, pTransactionCookie, &iidOfItransaction, (void **) &pTransaction_Imported ) );
      
	//*******************
	// Check the original transaction and exported transaction
	//*******************

	XACTTRANSINFO txInfo;
	HR( pTransaction -> GetTransactionInfo(&txInfo) );	
	XACTTRANSINFO txInfo2;
	HR( pTransaction_Imported -> GetTransactionInfo(&txInfo2) );

    //*******************
    // Enlist resoure manager
    //*******************      
    XACTUOW transactionUUID;
    LONG isolationLevel;
    TransactionResourceAsync transactionResourceAsync;
    ITransactionEnlistmentAsync * pTransactionEnlistmentAsync;
    transactionResourceAsync.AddRef();
    HR( pIResourceManager -> Enlist( pTransaction_Imported, &transactionResourceAsync, &transactionUUID, &isolationLevel, &pTransactionEnlistmentAsync ) );
    transactionResourceAsync.SaveContext( pTransactionEnlistmentAsync, transactionUUID, isolationLevel);
    
	//*******************
    // Get Xid with imported Transaction object
	//*******************
    XID xid2;
	HR(pIDtcToXaHelperSinglePipe->ConvertTridToXID( (DWORD*) pTransaction_Imported, rmCookie1, &xid2  ));
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
	//transactionResourceAsync.m_donotCommit = true;

    //*******************
    // call Commit or Abort
	//*******************
	bool committing = true;
	if (committing)
	{
       HR( pTransaction -> Commit( false, 0, 0 ) );
	}
	else
	{
	   HR( pTransaction -> Abort( false, 0, 0 ) );
	}

	Sleep(1000);

	//*******************
    // Reenlist the resource manager to recover the failed transaction
    //*******************
    if (committing && transactionResourceAsync.m_donotCommit)
	{
		XACTSTAT transactionOutput;		
		XACTSTAT * pTransactionOutput = &transactionOutput;		
		ReEnlist_xa_SinglePipe(pTransactionEnlistmentAsync, pIResourceManager, pTransactionOutput);

		XACTSTAT transactionOutput2;				
		XACTSTAT * pTransactionOutput2 = &transactionOutput2;		
		ReEnlist_xa_SinglePipe(pTransactionEnlistmentAsync, pIResourceManager, pTransactionOutput2);

		//*******************
		// Commit or abort according to the transaction output
		//*******************	
		if (transactionOutput == XACTSTAT::XACTSTAT_ABORTED)
		{
			printf("Transaction was aborted\n");
			HR( pTransactionEnlistmentAsync -> AbortRequestDone(S_OK) );	
		} 
		else if (transactionOutput == XACTSTAT::XACTSTAT_COMMITTED)
		{
			printf("Transaction was committed...\n");
			HR( pTransactionEnlistmentAsync -> CommitRequestDone(S_OK) );		
		}

        //*******************
		// Complete Reenlisting
		//*******************	
		HR( pIResourceManager -> ReenlistmentComplete() );
    }

	//*******************
    // Release XA Resource Manager
    //*******************
    pIDtcToXaHelperSinglePipe->ReleaseRMCookie( rmCookie1, true );	

    //*******************
    // Release COM objects and memory
    //*******************
	pIDtcToXaHelperSinglePipe -> Release();
	pTransaction_Imported -> Release();      
    pITransactionImport -> Release();
    pITransactionExport -> Release();
    pITransactionExportFactory -> Release();
    pITransactionImportWhereabouts -> Release();
    pResourceManagerFactory -> Release();
    pTransactionDispenser -> Release();
	delete [] pTransactionCookie;
    delete [] pWhereAbouts;
	return 0;
}

