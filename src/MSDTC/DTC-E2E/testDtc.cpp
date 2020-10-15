#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <TxDtc.h>
#include <xoleHlp.h>
#include "classes.h"

#pragma comment(lib, "xolehlp.lib")

int testDtc()
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
	// Get ResourceManager
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
	ITransactionEnlistmentAsync * pXATransactionEnlistmentAsync;
	transactionResourceAsync.AddRef();
	HR(pIResourceManager->Enlist(pXATransaction, &transactionResourceAsync, &transactionUUID, &isolationLevel, &pXATransactionEnlistmentAsync));
	transactionResourceAsync.SaveContext(pXATransactionEnlistmentAsync, transactionUUID, isolationLevel);
	
	//*******************
	// Enlist second resource manager
	//*******************	
	TransactionResourceAsync transactionResourceAsync2;
	ITransactionEnlistmentAsync * pXATransactionEnlistmentAsync2;
	transactionResourceAsync2.AddRef();
	HR(pIResourceManager->Enlist(pXATransaction, &transactionResourceAsync2, &transactionUUID, &isolationLevel, &pXATransactionEnlistmentAsync2));
	transactionResourceAsync2.SaveContext(pXATransactionEnlistmentAsync2, transactionUUID, isolationLevel);

	//*******************
	// Commit
	//*******************
	HR(pXATransaction->Commit(false, 0, 0));
	Sleep(1000);
    return 0;
}


