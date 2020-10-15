#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <TxDtc.h>
#include <xoleHlp.h>
#include "classes.h"

#pragma comment(lib, "xolehlp.lib")

void ReJoin_WithResourceManager2(ITransactionEnlistmentAsync * pXATransactionEnlistmentAsync, IResourceManager2 * pIResourceManager, XACTSTAT * pXATransactionOutput)
{
    	//*******************
		// Create PrepareInfo object using TransactionEnlistmentAsync object
		//*******************	
		IPrepareInfo2 * pIPrepareInfo2 = nullptr;
		HR( pXATransactionEnlistmentAsync -> QueryInterface( __uuidof(IPrepareInfo2), (void **) &pIPrepareInfo2 ) );
		ULONG prepareInfoSize = -1;
		HR( pIPrepareInfo2 -> GetPrepareInfoSize(&prepareInfoSize) );
		
		//*******************
		// Reenlist in order to get the transaction output
		//*******************	
		byte * prepareInfo = new byte[prepareInfoSize];
		HR( pIPrepareInfo2 -> GetPrepareInfo( prepareInfoSize, prepareInfo ) );

		HRESULT hr = pIResourceManager -> Reenlist(prepareInfo, prepareInfoSize, XACTCONST_TIMEOUTINFINITE, pXATransactionOutput );
		if (hr == XACT_E_RECOVERYALREADYDONE)
		{
				IResourceManagerRejoinable * pIResourceManagerRejoinable = nullptr;
				HR(pIResourceManager->QueryInterface(IID_IResourceManagerRejoinable, (void**)&pIResourceManagerRejoinable));
				HR(pIResourceManagerRejoinable->Rejoin ((BYTE*)prepareInfo, prepareInfoSize, XACTCONST_TIMEOUTINFINITE, pXATransactionOutput));
		}

		//*******************
		// Release COM object and memory
		//*******************
		pIPrepareInfo2 -> Release();
		delete [] prepareInfo;
}


int testDtc_E2E_Rejoin_WithResourceManager2()
{
	//*******************
	// Get TransactionDispenser object
	//*******************
	ITransactionDispenser * pXATransactionDispenser = nullptr;
	HR( DtcGetTransactionManager( nullptr, nullptr, __uuidof(ITransactionDispenser), 0, 0, (void *) nullptr, (void **) &pXATransactionDispenser ) );

	//*******************
	// Create a new transaction object with calling BeginTranaction()
	//*******************      
	ITransaction * pXATransaction = nullptr;
	HR( pXATransactionDispenser -> BeginTransaction( nullptr, ISOLATIONLEVEL::ISOLATIONLEVEL_UNSPECIFIED, 0, nullptr, &pXATransaction ) );
              
	//*******************
	// Create ResourceManager object using IResourceManagerFactory
	//*******************
	IResourceManagerFactory2 * pResourceManagerFactory = nullptr;
	HR( pXATransactionDispenser -> QueryInterface( __uuidof(IResourceManagerFactory), (void **) &pResourceManagerFactory ) );     
	IResourceManager2 * pIResourceManager = nullptr;
	ResourceManagerSink resourceManagerSink;
	resourceManagerSink.AddRef();
	GUID guid;
	HR( CoCreateGuid(&guid));
	HR( pResourceManagerFactory -> CreateEx( &guid, "ResourceManager1", &resourceManagerSink, IID_IResourceManager, (void**) &pIResourceManager ) );

	//*******************
	// ReenlistmentComplete
	//*******************			
	HR( pIResourceManager -> ReenlistmentComplete() );
	
	//*******************
	// Get WhereAboutSize
	//*******************             
	ITransactionImportWhereabouts * pITransactionImportWhereabouts = nullptr;
	HR( pXATransactionDispenser -> QueryInterface( __uuidof(ITransactionImportWhereabouts), (void **) &pITransactionImportWhereabouts ) );     
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
	HR( pXATransactionDispenser -> QueryInterface( __uuidof(ITransactionExportFactory), (void **) &pITransactionExportFactory ) );  
	ITransactionExport * pITransactionExport  = nullptr;
	HR( pITransactionExportFactory -> Create( whereAboutsSizeUsed, pWhereAbouts, &pITransactionExport ) );

	//*******************
	// Export the transaction object
	//*******************             
	ULONG transactionCookieSize = -1; 
	HR( pITransactionExport -> Export( pXATransaction, &transactionCookieSize ) );

	//*******************
	// Get transaction cookie
	//*******************             
	byte * pXATransactionCookie = new byte[transactionCookieSize];
	ULONG transactionCookieSize2 = -1;
	HR( pITransactionExport -> GetTransactionCookie( pXATransaction, transactionCookieSize, pXATransactionCookie, &transactionCookieSize2 ) );

	//*******************
	// Get Import object
	//*******************             
	ITransactionImport * pITransactionImport = nullptr;
	HR( pXATransactionDispenser -> QueryInterface( __uuidof(ITransactionImport), (void **) &pITransactionImport ) );  

	//*******************
	// Import transaction object
	//*******************             
	ITransaction * pXATransaction_Imported = nullptr;
	IID iidOfItransaction = __uuidof(ITransaction);
	HR( pITransactionImport -> Import( transactionCookieSize2, pXATransactionCookie, &iidOfItransaction, (void **) &pXATransaction_Imported ) );
      
	//*******************
	// Check the original transaction and exported transaction
	//*******************

	XACTTRANSINFO txInfo;
	HR( pXATransaction -> GetTransactionInfo(&txInfo) );	
	XACTTRANSINFO txInfo2;
	HR( pXATransaction_Imported -> GetTransactionInfo(&txInfo2) );

	//*******************
	// Enlist resoure manager
	//*******************      
	XACTUOW transactionUUID;
	LONG isolationLevel;
	TransactionResourceAsync transactionResourceAsync;
	ITransactionEnlistmentAsync * pXATransactionEnlistmentAsync;
	transactionResourceAsync.AddRef();
	HR( pIResourceManager -> Enlist( pXATransaction_Imported, &transactionResourceAsync, &transactionUUID, &isolationLevel, &pXATransactionEnlistmentAsync ) );
	transactionResourceAsync.SaveContext( pXATransactionEnlistmentAsync, transactionUUID, isolationLevel);
    
	//*******************
	// do not call CommitRequestDone() to simulate the accidental transaction commit failure
	//*******************    
	transactionResourceAsync.m_donotCommit = true;

	//*******************
	// Commit
	//*******************
	HR( pXATransaction -> Commit( false, 0, 0 ) );
	Sleep(1000);

	//*******************
	// Reenlist the resource manager to recover the failed transaction
	//*******************
	if (transactionResourceAsync.m_donotCommit)
	{
		//*******************
		// ReJoin
		//*******************	
		XACTSTAT transactionOutput;		
		XACTSTAT * pXATransactionOutput = &transactionOutput;		
		ReJoin_WithResourceManager2(pXATransactionEnlistmentAsync, pIResourceManager, pXATransactionOutput);
		
		if (transactionOutput == XACTSTAT::XACTSTAT_ABORTED)
		{
			printf("Transaction was aborted\n");
		} 
		else if (transactionOutput == XACTSTAT::XACTSTAT_COMMITTED)
		{
			printf("Transaction was committed...\n");
		} 

		//*******************
		// ReJoin
		//*******************			
		XACTSTAT transactionOutput2;				
		XACTSTAT * pXATransactionOutput2 = &transactionOutput2;		
		ReJoin_WithResourceManager2(pXATransactionEnlistmentAsync, pIResourceManager, pXATransactionOutput2);

		if (transactionOutput2 == XACTSTAT::XACTSTAT_ABORTED)
		{
			printf("Transaction was aborted\n");
		} 
		else if (transactionOutput2 == XACTSTAT::XACTSTAT_COMMITTED)
		{
			printf("Transaction was committed...\n");
		}

		//*******************
		// ReJoin
		//*******************			
		XACTSTAT transactionOutput3;
		XACTSTAT * pXATransactionOutput3 = &transactionOutput3;		
		ReJoin_WithResourceManager2(pXATransactionEnlistmentAsync, pIResourceManager, pXATransactionOutput3);

		if (transactionOutput3 == XACTSTAT::XACTSTAT_ABORTED)
		{
			printf("Transaction was aborted\n");
			HR( pXATransactionEnlistmentAsync -> AbortRequestDone(S_OK) );	
		} 
		else if (transactionOutput3 == XACTSTAT::XACTSTAT_COMMITTED)
		{
			printf("Transaction was committed...\n");
			HR( pXATransactionEnlistmentAsync -> CommitRequestDone(S_OK) );		
		}
	}

	//*******************
	// Release COM objects and memory
	//*******************
	pXATransaction_Imported -> Release();      
	pITransactionImport -> Release();
	pITransactionExport -> Release();
	pITransactionExportFactory -> Release();
	pITransactionImportWhereabouts -> Release();
	pResourceManagerFactory -> Release();
	pXATransactionDispenser -> Release();
	delete [] pXATransactionCookie;
	delete [] pWhereAbouts;
	return 0;
}
