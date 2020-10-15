#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <TxDtc.h>
#include <xoleHlp.h>
#include "classes.h"

#pragma comment(lib, "xolehlp.lib")

int testDtc_ChooseDTC()
{
	//*******************
    // Get TransactionDispenser object
    //*******************

	OLE_TM_CONFIG_PARAMS_V2 * pConfigParam = nullptr;
	ITransactionDispenser * pXATransactionDispenser = nullptr;	
    LPSTR pServerName = nullptr;
<<<<<<< HEAD
	LPSTR pClusterDtcGuid = nullptr;
=======
>>>>>>> bac425f0a67bcefec4eaf656b046c8d55cae79cf
    
	//
	// NOTE: pServerName should be null in order to choose specific DTC
	//

	// Alternatively, you can choose the DTC instance programatically instead of using the default DTC
	//  Choosing clustered DTC
	/*
	OLE_TM_CONFIG_PARAMS_V2 configParam;
    //GUID clusterResourceId = { 0xc9a72bba, 0x427d, 0x4df6, { 0x9b, 0x8a, 0x3a, 0xdb, 0xc7, 0x5a, 0x91, 0x1a } };   // MSDTC-DTC-7183931DTC1
    GUID clusterResourceId = { 0x495f5fd4, 0xdad9, 0x458e, { 0xbd, 0x30, 0x17, 0x26, 0x83, 0x3b, 0x87, 0xbc } };   // MSDTC-DTC-7183931DTC2
	configParam.applicationType = APPLICATIONTYPE::CLUSTERRESOURCE_APPLICATIONTYPE;
	configParam.clusterResourceId = clusterResourceId;
    configParam.dwcConcurrencyHint = 0;
    configParam.dwVersion = OLE_TM_CONFIG_VERSION_2;
	pConfigParam = &configParam;
	*/
		
	// Choosing local DTC instead of using the default clustered DTC
	OLE_TM_CONFIG_PARAMS_V2 configParam;
    configParam.applicationType = APPLICATIONTYPE::LOCAL_APPLICATIONTYPE;
    configParam.dwcConcurrencyHint = 0; 
    configParam.dwVersion = OLE_TM_CONFIG_VERSION_2;
	pConfigParam = &configParam;
	
    HR( DtcGetTransactionManagerExA( pServerName, nullptr, __uuidof(ITransactionDispenser), 0, pConfigParam, (void **) &pXATransactionDispenser ) );
	
    //*******************
    // Create a new transaction object with calling BeginTranaction()
    //*******************      
    ITransaction * pXATransaction = nullptr;
    HR( pXATransactionDispenser -> BeginTransaction( nullptr, ISOLATIONLEVEL::ISOLATIONLEVEL_UNSPECIFIED, 0, nullptr, &pXATransaction ) );
	          
    //*******************
    // Create ResourceManager object using IResourceManagerFactory
    //*******************
    IResourceManagerFactory * pResourceManagerFactory = nullptr;
    HR( pXATransactionDispenser -> QueryInterface( __uuidof(IResourceManagerFactory), (void **) &pResourceManagerFactory ) );     
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
    HR( pXATransactionDispenser -> QueryInterface( __uuidof(ITransactionImportWhereabouts), (void **) &pITransactionImportWhereabouts ) );     
    ULONG whereAboutSize = -1;
    HR( pITransactionImportWhereabouts -> GetWhereaboutsSize(&whereAboutSize ) );

    //*******************
    // Create WhereAbout object
    //*******************             
    byte * pWhereAbouts = new byte[whereAboutSize];
    ULONG whereAboutsSize2 = -1;
    HR( pITransactionImportWhereabouts -> GetWhereabouts( whereAboutSize, pWhereAbouts, &whereAboutsSize2 ) );

    //*******************
    // Create Export object using ITransactionExportFactory
    //*******************             
    ITransactionExportFactory * pITransactionExportFactory = nullptr;
    HR( pXATransactionDispenser -> QueryInterface( __uuidof(ITransactionExportFactory), (void **) &pITransactionExportFactory ) );  
    ITransactionExport * pITransactionExport  = nullptr;
	HR( pITransactionExportFactory -> Create( whereAboutsSize2, pWhereAbouts, &pITransactionExport ) );

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
	transactionResourceAsync.m_donotCommit = false;

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
		XACTSTAT transactionOutput;
		HR( pIResourceManager -> Reenlist(prepareInfo, prepareInfoSize, XACTCONST_TIMEOUTINFINITE, &transactionOutput ) );
		
		//*******************
		// Commit or abort according to the transaction output
		//*******************	
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
		
		//*******************
		// Complete Reenlisting
		//*******************	
		HR( pIResourceManager -> ReenlistmentComplete() );
		
		//*******************
		// Release COM object and memory
		//*******************
		pIPrepareInfo2 -> Release();
		delete [] prepareInfo;
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
