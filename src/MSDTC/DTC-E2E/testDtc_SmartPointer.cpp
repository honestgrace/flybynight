#include "stdafx.h"
#include <windows.h>
#include <wrl.h>
#include <stdio.h>
#include <TxDtc.h>
#include <xoleHlp.h>
#include "classes.h"

using namespace Microsoft::WRL;

#pragma comment(lib, "xolehlp.lib")

#define TRACE OuputDebugString

int testDtc_SmartPointer()
{
	//////////////////////
    // Get TransactionDispenser object
    //////////////////////

	ComPtr<ITransactionDispenser> pXATransactionDispenser = nullptr;
    HR( DtcGetTransactionManager( nullptr, nullptr, __uuidof(ITransactionDispenser), 0, 0, (void *) nullptr, (void **) &pXATransactionDispenser ) );
	
    //////////////////////
    // Create a new transaction object with calling BeginTranaction()
    //////////////////////      
    ComPtr<ITransaction> pXATransaction = nullptr;
    HR( pXATransactionDispenser -> BeginTransaction( nullptr, ISOLATIONLEVEL::ISOLATIONLEVEL_UNSPECIFIED, 0, nullptr, &pXATransaction ) );

    //////////////////////
    // Create ResourceManager object using IResourceManagerFactory
    //////////////////////
    ComPtr<IResourceManagerFactory> pResourceManagerFactory = nullptr;
    HR( pXATransactionDispenser.CopyTo( __uuidof(IResourceManagerFactory), (void **) &pResourceManagerFactory ) );     
    ComPtr<IResourceManager> pIResourceManager = nullptr;
    ResourceManagerSink resourceManagerSink;
    resourceManagerSink.AddRef();
    GUID guid;
    HR( CoCreateGuid(&guid));
    HR( pResourceManagerFactory -> Create( &guid, "ResourceManager1", &resourceManagerSink, &pIResourceManager ) );

    //////////////////////
    // Get WhereAboutSize
    //////////////////////             
    ComPtr<ITransactionImportWhereabouts> pITransactionImportWhereabouts = nullptr;
    HR( pXATransactionDispenser.CopyTo( __uuidof(ITransactionImportWhereabouts), (void **) &pITransactionImportWhereabouts ) );     
    ULONG whereAboutSize = -1;
    HR( pITransactionImportWhereabouts -> GetWhereaboutsSize(&whereAboutSize ) );

    //////////////////////
    // Create WhereAbout object
    //////////////////////             
    byte * pWhereAbouts = new byte[whereAboutSize];
    ULONG whereAboutsSize2 = -1;
    HR( pITransactionImportWhereabouts -> GetWhereabouts( whereAboutSize, pWhereAbouts, &whereAboutsSize2 ) );

    //////////////////////
    // Create Export object using ITransactionExportFactory
    //////////////////////             
    ComPtr<ITransactionExportFactory> pITransactionExportFactory = nullptr;
    HR( pXATransactionDispenser.CopyTo( __uuidof(ITransactionExportFactory), (void **) &pITransactionExportFactory ) );  
    ComPtr<ITransactionExport> pITransactionExport  = nullptr;
	HR( pITransactionExportFactory -> Create( whereAboutsSize2, pWhereAbouts, &pITransactionExport ) );

    //////////////////////
    // Export the transaction object
    //////////////////////             
    ULONG transactionCookieSize = -1; 
    HR( pITransactionExport -> Export( *pXATransaction.GetAddressOf(), &transactionCookieSize ) );
	
    //////////////////////
    // Get transaction cookie
    //////////////////////             
    byte * pXATransactionCookie = new byte[transactionCookieSize];
    ULONG transactionCookieSize2 = -1;
    HR( pITransactionExport -> GetTransactionCookie( *pXATransaction.GetAddressOf(), transactionCookieSize, pXATransactionCookie, &transactionCookieSize2 ) );

    //////////////////////
    // Get Import object
    //////////////////////             
    ComPtr<ITransactionImport> pITransactionImport = nullptr;
    HR( pXATransactionDispenser.CopyTo( __uuidof(ITransactionImport), (void **) &pITransactionImport ) );  

    //////////////////////
    // Import transaction object
    //////////////////////             
    ComPtr<ITransaction> pXATransaction_Imported = nullptr;
    IID iidOfItransaction = __uuidof(ITransaction);
    HR( pITransactionImport -> Import( transactionCookieSize2, pXATransactionCookie, &iidOfItransaction, (void **) &pXATransaction_Imported ) );
       
    //////////////////////
    // Enlist resoure manager
    //////////////////////      
    XACTUOW transactionUUID;
    LONG isolationLevel;
    TransactionResourceAsync transactionResourceAsync;
    ComPtr<ITransactionEnlistmentAsync> pXATransactionEnlistmentAsync;
    transactionResourceAsync.AddRef();
    HR( pIResourceManager -> Enlist( *pXATransaction_Imported.GetAddressOf(), &transactionResourceAsync, &transactionUUID, &isolationLevel, &pXATransactionEnlistmentAsync ) );
    transactionResourceAsync.SaveContext( *pXATransactionEnlistmentAsync.GetAddressOf(), transactionUUID, isolationLevel);
    
	//////////////////////
    // do not call CommitRequestDone() to simulate the accidental transaction commit failure
    //////////////////////    
	transactionResourceAsync.m_donotCommit = false;

    //////////////////////
    // Commit
    //////////////////////
    HR( pXATransaction -> Commit( false, 0, 0 ) );
	Sleep(1000);

	//////////////////////
    // Reenlist the resource manager to recover the failed transaction
    //////////////////////
	if (transactionResourceAsync.m_donotCommit)
	{
		//////////////////////
		// Create PrepareInfo object using TransactionEnlistmentAsync object
		//////////////////////	
		ComPtr<IPrepareInfo2> pIPrepareInfo2 = nullptr;
		HR( pXATransactionEnlistmentAsync.CopyTo( __uuidof(IPrepareInfo2), (void **) &pIPrepareInfo2 ) );
		ULONG prepareInfoSize = -1;
		HR( pIPrepareInfo2 -> GetPrepareInfoSize(&prepareInfoSize) );
		
		//////////////////////
		// Reenlist in order to get the transaction output
		//////////////////////	
		byte * prepareInfo = new byte[prepareInfoSize];
		HR( pIPrepareInfo2 -> GetPrepareInfo( prepareInfoSize, prepareInfo ) );
		XACTSTAT transactionOutput;
		HR( pIResourceManager -> Reenlist(prepareInfo, prepareInfoSize, XACTCONST_TIMEOUTINFINITE, &transactionOutput ) );
		
		//////////////////////
		// Commit or abort according to the transaction output
		//////////////////////	
		if (transactionOutput == XACTSTAT::XACTSTAT_ABORTED)
		{
			printf("Transaction was aborted\n");
			HR( pXATransactionEnlistmentAsync -> AbortRequestDone(S_OK) );	
		} 
		else if (transactionOutput == XACTSTAT::XACTSTAT_COMMITTED)
		{
			printf("Transaction was committed...\n");
			HR( pXATransactionEnlistmentAsync -> CommitRequestDone(S_OK) );		
		}
		
		//////////////////////
		// Complete Reenlisting
		//////////////////////	
		HR( pIResourceManager -> ReenlistmentComplete() );
		
		//////////////////////
		// Release COM object and memory
		//////////////////////
		delete [] prepareInfo;
    }

    //////////////////////
    // Release COM objects and memory
    //////////////////////
    delete [] pXATransactionCookie;
    delete [] pWhereAbouts;
	return 0;
}
