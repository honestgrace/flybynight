#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <TxDtc.h>
#include <xoleHlp.h>
#include "classes.h"

#pragma comment(lib, "xolehlp.lib")

int testDtc_E2E_pull()
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
    IResourceManagerFactory * pResourceManagerFactory = nullptr;
    HR( pXATransactionDispenser -> QueryInterface( __uuidof(IResourceManagerFactory), (void **) &pResourceManagerFactory ) );     
    IResourceManager * pIResourceManager = nullptr;
    ResourceManagerSink resourceManagerSink;
    resourceManagerSink.AddRef();
    GUID guid;
    HR( CoCreateGuid(&guid));
    HR( pResourceManagerFactory -> Create( &guid, "ResourceManager1", &resourceManagerSink, &pIResourceManager ) );

	//*******************
    // Pulling trasaction object
    //*******************    
	ITransactionTransmitterFactory * pXATransactionTransmitterFactory = nullptr;
	HR( pXATransactionDispenser -> QueryInterface( IID_ITransactionTransmitterFactory, (void **) &pXATransactionTransmitterFactory ) );

	ITransactionTransmitter * pXATransactionTransmitter = nullptr;
	HR( pXATransactionTransmitterFactory -> Create( &pXATransactionTransmitter) );

	HR( pXATransactionTransmitter -> Set(pXATransaction) );

	ULONG tokenSize = 0;

	HR( pXATransactionTransmitter -> GetPropagationTokenSize( &tokenSize) );

	byte * token = new byte[tokenSize];
	ULONG tokenSizeUsed = 0;

	HR( pXATransactionTransmitter -> MarshalPropagationToken( tokenSize, token, &tokenSizeUsed ) );

	HR( pXATransactionTransmitter -> Reset() );

	ITransactionReceiverFactory * pXATransactionReceiverFactory = nullptr;
    HR( pXATransactionDispenser -> QueryInterface ( IID_ITransactionReceiverFactory, (void **) &pXATransactionReceiverFactory ) );

	ITransactionReceiver * pXATransactionReceiver = nullptr;

	HR( pXATransactionReceiverFactory -> Create(&pXATransactionReceiver) );

	ITransaction * pXATransaction_transmitted = nullptr;

	HR( pXATransactionReceiver -> UnmarshalPropagationToken( tokenSizeUsed, token, &pXATransaction_transmitted) );
	
	//*******************
	// Check the original transaction and exported transaction
	//*******************

	XACTTRANSINFO txInfo;
	HR( pXATransaction -> GetTransactionInfo(&txInfo) );	
	XACTTRANSINFO txInfo2;
	HR( pXATransaction_transmitted -> GetTransactionInfo(&txInfo2) );

    //*******************
    // Enlist resoure manager
    //*******************      
    XACTUOW transactionUUID;
    LONG isolationLevel;
    TransactionResourceAsync transactionResourceAsync;
    ITransactionEnlistmentAsync * pXATransactionEnlistmentAsync;
    transactionResourceAsync.AddRef();
    HR( pIResourceManager -> Enlist( pXATransaction_transmitted, &transactionResourceAsync, &transactionUUID, &isolationLevel, &pXATransactionEnlistmentAsync ) );
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

	delete [] token;
	pXATransactionReceiver -> Release();
	pXATransactionReceiverFactory -> Release();
	pXATransactionTransmitter -> Release();
	pXATransactionTransmitterFactory -> Release();

	pXATransaction_transmitted -> Release();
	pXATransaction -> Release();
	pResourceManagerFactory -> Release();
    pXATransactionDispenser -> Release();
	
	return 0;
}
