#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <TxDtc.h>
#include <xoleHlp.h>
#include <xa.h>

#pragma comment(lib, "xolehlp.lib")

extern "C"
{		
	extern struct xa_switch_t  msqlsrvxa1;
}

//*******************
// Utility function
//*******************
inline void HR(HRESULT hr) { if (hr != S_OK) { throw GetLastError(); } }

//*******************
// Implementing IResourceManagerSink
//*******************
class ResourceManagerSink : public IResourceManagerSink
{
	 ULONG m_cRef;

public:
	ResourceManagerSink() { 
		m_cRef = 0; 
		AddRef();
	}

	~ResourceManagerSink() {
		Release();
	}

	STDMETHODIMP_(ULONG) AddRef() {	
		return InterlockedIncrement( &m_cRef );	
	}

	STDMETHODIMP_(ULONG) Release()	{
		ULONG localRef = 0;

		localRef = InterlockedDecrement( &m_cRef );
		if ( 0 == localRef )
			delete this;

		return localRef;
	}

	STDMETHODIMP QueryInterface( REFIID i_iid, LPVOID FAR* o_ppv )	{
		HRESULT hr = S_OK;
		if ( NULL == o_ppv )
			return E_INVALIDARG;

		*o_ppv = NULL;
		if ( IID_IUnknown == i_iid )
			*o_ppv = (IUnknown*) this;
		else if ( IID_IResourceManagerSink == i_iid )
			*o_ppv = (IResourceManagerSink*) this;
		else
			hr = E_NOINTERFACE;

		if (SUCCEEDED(hr))
			((IUnknown *) *o_ppv)->AddRef ();

		return hr;
	}

	STDMETHODIMP TMDown() {
		printf("ResourceManagerSink:: TMDown()\n");
		return S_OK;
	}
};

//*******************
// Implementing ITransactionResourceAsync
//*******************
class TransactionResourceAsync : public ITransactionResourceAsync
{
	ULONG m_cRef;

public:
	ITransactionEnlistmentAsync * m_pTransactionEnlistmentAsync;
	XACTUOW m_transactionUUID;
	LONG m_isolationLevel;
	BOOL m_donotCommit;
	
	BOOL m_isXARM;
	XID * m_pMyXid;
	int m_XARmId;
	
	TransactionResourceAsync() { 
		m_cRef = 0; 
		m_donotCommit = false;
		m_isXARM = false;
		m_pMyXid = nullptr;
		m_pTransactionEnlistmentAsync = nullptr;
		AddRef();
	}

	~TransactionResourceAsync() {
		Release();
	}

	//
	// Implementing IUnknown methods
	//
	STDMETHODIMP_(ULONG) AddRef() {	
		return InterlockedIncrement( &m_cRef );	
	}

	STDMETHODIMP_(ULONG) Release()	{
		ULONG localRef = 0;

		localRef = InterlockedDecrement( &m_cRef );
		if ( 0 == localRef )
			delete this;

		return localRef;
	}

	STDMETHODIMP QueryInterface( REFIID i_iid, LPVOID FAR* o_ppv )	{
		HRESULT hr = S_OK;
		if ( NULL == o_ppv )
			return E_INVALIDARG;

		*o_ppv = NULL;
		if ( IID_IUnknown == i_iid )
			*o_ppv = (IUnknown*) this;
		else if ( IID_ITransactionResourceAsync == i_iid )
			*o_ppv = (ITransactionResourceAsync*) this;
		else
			hr = E_NOINTERFACE;

		if (SUCCEEDED(hr))
			((IUnknown *) *o_ppv)->AddRef ();

		return hr;
	}

	//
	// Implementing ITransactionResourceAsync methods
	//
	STDMETHODIMP PrepareRequest(BOOL fRetaining, DWORD grfRM, BOOL fWantMoniker, BOOL fSinglePhase) {
		printf("TransactionResourceAsync:: PrepareRequest()\n");
		if ( m_isXARM )
		{
			int xaerr = XA_OK;
			xaerr = msqlsrvxa1.xa_prepare_entry(m_pMyXid, m_XARmId, TMONEPHASE);
			if ( XA_OK != xaerr )
			{
				printf( "Error %d from xa_prepare.\n", xaerr );
				return -1;
			}
		}
		return m_pTransactionEnlistmentAsync->PrepareRequestDone(S_OK,NULL,NULL);
	}

	STDMETHODIMP CommitRequest(DWORD grfRM, XACTUOW *pNewUOW) {
		printf("TransactionResourceAsync:: CommitRequest()\n");
		if (m_donotCommit)
		{
			// skip calling CommitRequestDone() to simulate the acidential transaction commitment failure
			printf("skip calling CommitRequestDone()...\n");
			return S_OK;
		}
		if ( m_isXARM )
		{
			int xaerr = XA_OK;
			xaerr = msqlsrvxa1.xa_commit_entry(m_pMyXid, m_XARmId, TMONEPHASE);
			Sleep(1000);
			if ( XA_OK != xaerr )
			{
				printf( "Error %d from xa_commit.\n", xaerr );
				return -1;
			}
		}
		return m_pTransactionEnlistmentAsync->CommitRequestDone(S_OK);
	}

	STDMETHODIMP AbortRequest(BOID *pboidReason,BOOL fRetaining, XACTUOW *pNewUOW) {
		printf("TransactionResourceAsync:: AbortRequest()\n");
		if (m_donotCommit)
		{
			// skip calling AbortRequestDone() to simulate the acidential transaction abort failure
			printf("skip calling AbortRequestDone()...\n");
			return S_OK;
		}
		if ( m_isXARM )
		{
			int xaerr = XA_OK;
			xaerr = msqlsrvxa1.xa_rollback_entry(m_pMyXid, m_isXARM, TMONEPHASE);
			if ( XA_OK != xaerr )
			{
				printf( "Error %d from xa_rollback.\n", xaerr );
				return -1;
			}
		}
		return m_pTransactionEnlistmentAsync->AbortRequestDone(S_OK);
	}

	STDMETHODIMP TMDown() {
		printf("TransactionResourceAsync:: TMDown()\n");
		return S_OK;
	}

	//
	// Utility methods
	//
	STDMETHODIMP EnableXA(XID * pXID, int xaRMId)
	{
		m_isXARM = true;
		m_pMyXid = pXID;
		m_XARmId = xaRMId;
		return S_OK;
	}

	STDMETHODIMP SaveContext(ITransactionEnlistmentAsync * pTransactionEnlistmentAsync) {
		m_pTransactionEnlistmentAsync = pTransactionEnlistmentAsync;
		return S_OK;
	}

	STDMETHODIMP SaveContext(ITransactionEnlistmentAsync * pTransactionEnlistmentAsync, XACTUOW transactionUUID, LONG isolationLevel) {
		m_pTransactionEnlistmentAsync = pTransactionEnlistmentAsync;
		m_transactionUUID = transactionUUID;
		m_isolationLevel = isolationLevel;	
		return S_OK;
	}
};

//*******************
// Implementing ITransactionVoterNotifyAsync2
//*******************
class TransactionVoterNotifyAsync2 : public ITransactionVoterNotifyAsync2
{
	ULONG m_cRef;
	ITransactionVoterBallotAsync2 * m_pITransactionVoterBallotAsync2;

public:
	TransactionVoterNotifyAsync2() {
		m_cRef = 0;
		m_pITransactionVoterBallotAsync2 = nullptr;
		AddRef();
	}

	~TransactionVoterNotifyAsync2() {
		Release();
	}

	STDMETHODIMP_(ULONG) AddRef() {	
		return InterlockedIncrement( &m_cRef );	
	}

	STDMETHODIMP_(ULONG) Release()	{
		ULONG localRef = 0;

		localRef = InterlockedDecrement( &m_cRef );
		if ( 0 == localRef )
			delete this;

		return localRef;
	}

	STDMETHODIMP QueryInterface( REFIID i_iid, LPVOID FAR* o_ppv )	{
		HRESULT hr = S_OK;
		if ( NULL == o_ppv )
			return E_INVALIDARG;

		*o_ppv = NULL;
		if ( IID_IUnknown == i_iid )
			*o_ppv = (IUnknown*) this;
		else if ( IID_ITransactionVoterNotifyAsync2 == i_iid )
			*o_ppv = (TransactionVoterNotifyAsync2*) this;
		else
			hr = E_NOINTERFACE;

		if (SUCCEEDED(hr))
			((IUnknown *) *o_ppv)->AddRef ();

		return hr;
	}

	STDMETHODIMP VoteRequest()  {
		printf("TransactionVoterNotifyAsync2::VoteRequest()\n");
		return m_pITransactionVoterBallotAsync2->VoteRequestDone(S_OK, nullptr);
	}

	STDMETHODIMP Committed(BOOL fRetaining, XACTUOW *pNewUOW, HRESULT hr ) 	{
		printf("TransactionVoterNotifyAsync2::Commited()\n");
		return S_OK;
	}
        
    STDMETHODIMP Aborted(BOID *pboidReason, BOOL fRetaining, XACTUOW *pNewUOW, HRESULT hr )	{
		printf("TransactionVoterNotifyAsync2::Aborted()\n");
		return S_OK;
	}
        
    STDMETHODIMP HeuristicDecision(DWORD dwDecision, BOID *pboidReason, HRESULT hr)	{
		printf("TransactionVoterNotifyAsync2::Aborted()\n");
		return S_OK;
	}
        
    STDMETHODIMP Indoubt()  {
		printf("TransactionVoterNotifyAsync2::Indoubt()\n");
		return S_OK;
	}
    
	STDMETHODIMP_(ULONG) SaveContext(ITransactionVoterBallotAsync2 * pITransactionVoterBallotAsync2)	{
		m_pITransactionVoterBallotAsync2 = pITransactionVoterBallotAsync2;
		return S_OK;
	}
};