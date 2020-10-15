#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <TxDtc.h>
#include <xoleHlp.h>
#include "classes.h"

int testDtc_MultipleResourceManager_Rejoin()
{
	//*******************
	// Get Transaction Manager
	//*******************
	ITransactionDispenser * pXATransactionDispenser = nullptr;
	HR(DtcGetTransactionManager(
		nullptr, //"localhost",
		nullptr,
		__uuidof(ITransactionDispenser),
		0,
		0,
		(void *) nullptr,
		(void **) &pXATransactionDispenser));

	//*******************
	// Begin Tranaction
	//*******************	
	ITransaction * pXATransaction = nullptr;
	HR(pXATransactionDispenser->BeginTransaction(
		nullptr,
		ISOLATIONLEVEL::ISOLATIONLEVEL_UNSPECIFIED,
		0,
		nullptr,
		&pXATransaction));

	XACTUOW transactionUUID;
	LONG isolationLevel;
	
	//*******************
	// Get ResourceManager Factory
	//*******************
	IResourceManagerFactory * pResourceManagerFactory = nullptr;
	HR(pXATransactionDispenser->QueryInterface(__uuidof(IResourceManagerFactory), (void **) &pResourceManagerFactory));	

	//*******************
	// Get first ResourceManager
	//*******************	
	IResourceManager * pIResourceManager;
	ResourceManagerSink resourceManagerSink;
	resourceManagerSink.AddRef();
	GUID guid;
	HR(CoCreateGuid(&guid));	
	HR(pResourceManagerFactory->Create(&guid, "ResourceManager1", &resourceManagerSink, &pIResourceManager));
	
	//*******************
	// Get second ResourceManager
	//*******************	
	IResourceManager * pIResourceManager2;
	ResourceManagerSink resourceManagerSink2;
	resourceManagerSink2.AddRef();
	GUID guid2;
	HR(CoCreateGuid(&guid2));	
	HR(pResourceManagerFactory->Create(&guid2, "ResourceManager2", &resourceManagerSink2, &pIResourceManager2));

	//*******************
	// Call ReenlistmentComplete
	//*******************	
	HR( pIResourceManager -> ReenlistmentComplete() );
	HR( pIResourceManager2 -> ReenlistmentComplete() );

	//*******************
	// Enlist first resoure manager
	//*******************	
	TransactionResourceAsync transactionResourceAsync;
	ITransactionEnlistmentAsync * pXATransactionEnlistmentAsync;	
	transactionResourceAsync.AddRef();
	HR(pIResourceManager->Enlist(pXATransaction, &transactionResourceAsync, &transactionUUID, &isolationLevel, &pXATransactionEnlistmentAsync));	
	transactionResourceAsync.SaveContext(pXATransactionEnlistmentAsync, transactionUUID, isolationLevel);

	//*******************
	// Enlist second resource manager to the second resource manager
	//*******************	
	TransactionResourceAsync transactionResourceAsync2;
	ITransactionEnlistmentAsync * pXATransactionEnlistmentAsync2;	
	transactionResourceAsync2.AddRef();
	HR(pIResourceManager2->Enlist(pXATransaction, &transactionResourceAsync2, &transactionUUID, &isolationLevel, &pXATransactionEnlistmentAsync2));
	transactionResourceAsync2.SaveContext(pXATransactionEnlistmentAsync2, transactionUUID, isolationLevel);

	//*******************
    // do not call CommitRequestDone() for the first RM to simulate the accidental transaction commit failure
    //*******************    
	transactionResourceAsync.m_donotCommit = true;
	transactionResourceAsync2.m_donotCommit = true;

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
		XACTSTAT transactionOutput;		
		XACTSTAT * pXATransactionOutput = &transactionOutput;		

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
    }

	//*******************
    // Reenlist the resource manager to recover the failed transaction
    //*******************
	if (transactionResourceAsync2.m_donotCommit)
	{
		XACTSTAT transactionOutput_second;		
		XACTSTAT * pXATransactionOutput_second = &transactionOutput_second;		

		//*******************
		// Create PrepareInfo object using TransactionEnlistmentAsync object
		//*******************	
		IPrepareInfo2 * pIPrepareInfo2_second = nullptr;
		HR( pXATransactionEnlistmentAsync2 -> QueryInterface( __uuidof(IPrepareInfo2), (void **) &pIPrepareInfo2_second ) );
		ULONG prepareInfoSize_second = -1;
		HR( pIPrepareInfo2_second -> GetPrepareInfoSize(&prepareInfoSize_second) );
		
		//*******************
		// Reenlist in order to get the transaction output
		//*******************	
		byte * prepareInfo_second = new byte[prepareInfoSize_second];
		HR( pIPrepareInfo2_second -> GetPrepareInfo( prepareInfoSize_second, prepareInfo_second ) );
		
		HRESULT hr = pIResourceManager2 -> Reenlist(prepareInfo_second, prepareInfoSize_second, XACTCONST_TIMEOUTINFINITE, pXATransactionOutput_second );
		if (hr == XACT_E_RECOVERYALREADYDONE)
        {
			  IResourceManagerRejoinable * pIResourceManagerRejoinable_second = nullptr;
              HR(pIResourceManager2->QueryInterface(IID_IResourceManagerRejoinable, (void**)&pIResourceManagerRejoinable_second));
              HR(pIResourceManagerRejoinable_second->Rejoin ((BYTE*)prepareInfo_second, prepareInfoSize_second, XACTCONST_TIMEOUTINFINITE, pXATransactionOutput_second));
        }

		//*******************
		// Release COM object and memory
		//*******************
		pIPrepareInfo2_second -> Release();
		delete [] prepareInfo_second;

		//*******************
		// Commit or abort according to the transaction output
		//*******************	
		if (transactionOutput_second == XACTSTAT::XACTSTAT_ABORTED)
		{
			printf("Transaction was aborted\n");
			HR( pXATransactionEnlistmentAsync2 -> AbortRequestDone(S_OK) );	
		} 
		else if (transactionOutput_second == XACTSTAT::XACTSTAT_COMMITTED)
		{
			printf("Transaction was committed...\n");
			HR( pXATransactionEnlistmentAsync2 -> CommitRequestDone(S_OK) );		
		}
    }

    return 0;
}