#include <iostream>
#include <tchar.h>
#include <objbase.h>
#include <txcoord.h>
#include <transact.h>
#include <stdio.h>
#include <xolehlp.h>
#include <txdtc.h>
#include <ocidl.h>
#include <xa.h>
#include "MyVoter.h"

MyVoter::MyVoter( bool bVoteYes, HANDLE phase2Event )
{
    m_cRef = 0;
    m_BallotAsync = NULL;
    m_phase2Event = phase2Event;
    printf( "Voter constructor - m_phase2event = 0x%x...\n", m_phase2Event );
    InitializeCriticalSection( &m_CritSec );
    m_bVoteYes = bVoteYes;
}

MyVoter::~MyVoter()
{
    printf( "MyVoter dtor\n" );
    if ( NULL != m_BallotAsync )
    {
        m_BallotAsync->Release();
    }
    DeleteCriticalSection( &m_CritSec );
}

STDMETHODIMP MyVoter::QueryInterface( REFIID i_iid, LPVOID FAR* o_ppv )
{
    HRESULT hr = S_OK;

    if ( NULL == o_ppv )
    {
        return E_INVALIDARG;
    }

    *o_ppv = NULL;

    if ( IID_IUnknown == i_iid )
    {
        *o_ppv = (IUnknown*) this;
    }
    else if ( IID_ITransactionVoterNotifyAsync2 == i_iid )
    {
        *o_ppv = (ITransactionVoterNotifyAsync2*) this;
    }
    else
    {
        hr = E_NOINTERFACE;
    }

    if (SUCCEEDED(hr))
    {
        ((IUnknown *) *o_ppv)->AddRef ();
    }

    return hr;
}

STDMETHODIMP_ (ULONG) MyVoter::AddRef()
{
    return InterlockedIncrement( &m_cRef );
}

STDMETHODIMP_ (ULONG) MyVoter::Release()
{
    ULONG localRef = 0;

    localRef = InterlockedDecrement( &m_cRef );
    if ( 0 == localRef )
    {
        delete this;
    }

    return localRef;
}

STDMETHODIMP MyVoter::VoteRequest(void)
{
	HRESULT hr;
    printf( "MyVoter::VoteRequest\n" );
    hr = m_BallotAsync->VoteRequestDone( (m_bVoteYes ? S_OK : E_FAIL), NULL );
	if ( FAILED(hr) )
    {
        printf( "Error!!! Failed to vote in PrePare request, 0x%x\n", hr );
    }
    return S_OK;
}

STDMETHODIMP MyVoter::Committed(
    BOOL fRetaining,
    XACTUOW * pNewUOW,
    HRESULT hr
    )
{
    printf( "MyVoter::Committed\n" );
    if ( NULL != m_BallotAsync )
    {
        m_BallotAsync->Release();
        m_BallotAsync = NULL;
    }
    SetEvent( m_phase2Event );
    return S_OK;
}

STDMETHODIMP MyVoter::Aborted(
    BOID * pboidReason,
    BOOL fRetaining,
    XACTUOW * pNewUOW,
    HRESULT hr
    )
{
    printf( "MyVoter::Aborted\n" );
    if ( NULL != m_BallotAsync )
    {
        m_BallotAsync->Release();
        m_BallotAsync = NULL;
    }
    SetEvent( m_phase2Event );
    return S_OK;
}

STDMETHODIMP MyVoter::HeuristicDecision(
    DWORD dwDecision,
    BOID * pboidReason,
    HRESULT hr
    )
{
    printf( "MyVoter::HeuristicDecision, dwDecision = %d.\n", dwDecision );
    if ( NULL != m_BallotAsync )
    {
        m_BallotAsync->Release();
        m_BallotAsync = NULL;
    }
    SetEvent( m_phase2Event );
    return S_OK;
}

STDMETHODIMP MyVoter::Indoubt(void)
{
    printf( "MyVoter::Indoubt\n" );
    if ( NULL != m_BallotAsync )
    {
        m_BallotAsync->Release();
        m_BallotAsync = NULL;
    }
    SetEvent( m_phase2Event );
    return S_OK;
}

HRESULT MyVoter::CreateVoter(ITransactionVoterFactory2* pVoterFactory, ITransaction* pTx)
{
	HRESULT hr = S_OK;
    ITransactionVoterNotifyAsync2* pVoterNotify = NULL;
    ITransactionVoterBallotAsync2* pVoterBallot = NULL;

    hr = this->QueryInterface( IID_ITransactionVoterNotifyAsync2, (void**) &pVoterNotify );
    if ( FAILED(hr) )
    {
        printf( "Failed to QI voter for ITransactionVoterNotifyAsync2, 0x%x\n", hr );
        goto Exit;
    }
    hr = pVoterFactory->Create( pTx, pVoterNotify, &pVoterBallot );
    if ( FAILED(hr) )
    {
        printf( "Failed to create voter, 0x%x\n", hr );
        goto Exit;
    }

    EnterCriticalSection( &m_CritSec );
    printf( "MyVoter::SetBallot, m_BallotAsync= 0x%x, new value = 0x%x\n", m_BallotAsync, pVoterBallot );
    if ( NULL != m_BallotAsync )
    {
        m_BallotAsync->Release();
        m_BallotAsync = NULL;
    }
    m_BallotAsync = pVoterBallot;
    LeaveCriticalSection( &m_CritSec );

Exit:
	if (pVoterNotify != NULL)
	{
		pVoterNotify->Release();
		pVoterNotify = NULL;
	}

	if (FAILED(hr))
	{
		if (m_BallotAsync != NULL)
		{
			m_BallotAsync->Release();
			m_BallotAsync = NULL;
			pVoterBallot= NULL;
		}

		if (pVoterBallot != NULL)
		{
			pVoterBallot->Release();
			pVoterBallot= NULL;
			m_BallotAsync = NULL;
		}

	}

	return hr;

}

HRESULT MyVoter::VoteNow()
{
    HRESULT hr = S_OK;
    printf( "MyVoter::VoteNow\n" );
    if ( NULL != m_BallotAsync )
    {
        hr = m_BallotAsync->VoteRequestDone( (m_bVoteYes ? S_OK : E_FAIL), NULL );
    }
    else
    {
        printf( "MyVoter::VoteNow - no ballot interface available.\n" );
        hr = E_UNEXPECTED;
    }
    return hr;
}
