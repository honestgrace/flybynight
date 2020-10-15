#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <TxDtc.h>
#include <xoleHlp.h>
#include "classes.h"

int testDtc_MultipleResourceManager()
{
	//*******************
	// Get Transaction Manager
	//*******************
	ITransactionDispenser * pTransactionDispenser = nullptr;
	HR(DtcGetTransactionManager(
		nullptr, //"localhost",
		nullptr,
		__uuidof(ITransactionDispenser),
		0,
		0,
		(void *) nullptr,
		(void **) &pTransactionDispenser));

	//*******************
	// Begin Tranaction
	//*******************	
	ITransaction * pTransaction = nullptr;
	HR(pTransactionDispenser->BeginTransaction(
		nullptr,
		ISOLATIONLEVEL::ISOLATIONLEVEL_UNSPECIFIED,
		0,
		nullptr,
		&pTransaction));

	XACTUOW transactionUUID;
	LONG isolationLevel;
	
	//*******************
	// Get ResourceManager Factory
	//*******************
	IResourceManagerFactory * pResourceManagerFactory = nullptr;
	HR(pTransactionDispenser->QueryInterface(__uuidof(IResourceManagerFactory), (void **) &pResourceManagerFactory));	

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
	// Enlist first resoure manager
	//*******************	
	TransactionResourceAsync transactionResourceAsync;
	ITransactionEnlistmentAsync * pTransactionEnlistmentAsync;	
	transactionResourceAsync.AddRef();
	HR(pIResourceManager->Enlist(pTransaction, &transactionResourceAsync, &transactionUUID, &isolationLevel, &pTransactionEnlistmentAsync));	
	transactionResourceAsync.SaveContext(pTransactionEnlistmentAsync, transactionUUID, isolationLevel);
	
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
	// Enlist second resource manager to the second resource manager
	//*******************	
	TransactionResourceAsync transactionResourceAsync2;
	ITransactionEnlistmentAsync * pTransactionEnlistmentAsync2;	
	transactionResourceAsync2.AddRef();
	HR(pIResourceManager2->Enlist(pTransaction, &transactionResourceAsync2, &transactionUUID, &isolationLevel, &pTransactionEnlistmentAsync2));
	transactionResourceAsync2.SaveContext(pTransactionEnlistmentAsync2, transactionUUID, isolationLevel);

	//*******************
	// Commit
	//*******************
	HR(pTransaction->Commit(false, 0, 0));
	Sleep(1000);
    return 0;
}