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
#include "Outcome.h"

    Outcome::Outcome()
    {
        m_cRef = 0;
		m_dwOutcomeCookie = 0;
		m_pConnPt = NULL;
		m_pTx = NULL;
    }

    Outcome::~Outcome()
    {
		if (m_pConnPt != NULL)
		{
			m_pConnPt->Release();
			m_pConnPt = NULL;
		}

		if (m_pTx != NULL)
		{
			printf("Outcome destructor releasing pTx\n");
			m_pTx->Release();
			m_pTx = NULL;
		}
	}

    STDMETHODIMP Outcome::QueryInterface( REFIID i_iid, LPVOID FAR* o_ppv )
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
        else if ( IID_ITransactionOutcomeEvents == i_iid )
        {
            *o_ppv = (ITransactionOutcomeEvents*) this;
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

    STDMETHODIMP_ (ULONG) Outcome::AddRef()
    {
        return InterlockedIncrement( &this->m_cRef );
    }

    STDMETHODIMP_ (ULONG) Outcome::Release()
    {
        ULONG localRef = 0;

        localRef = InterlockedDecrement( &this->m_cRef );
        if ( 0 == localRef )
        {
            delete this;
        }

        return localRef;
    }

    STDMETHODIMP Outcome::Aborted(
                            BOID * pboidReason,
                            BOOL fRetaining,
                            XACTUOW * pNewUOW,
                            HRESULT hr
                            )
    {
        printf("Outcome::Aborted.\n");

        printf("Unadvising...\n");
        HRESULT localHr = m_pConnPt->Unadvise( m_dwOutcomeCookie );
        if ( FAILED( localHr ) )
        {
            printf("Error from IConnectionPoint::Unadvise - 0x%x.\n", localHr );
        }


        printf("Releasing pConnPt...\n");
        m_pConnPt->Release();
        m_pConnPt = NULL;

        printf("Release pTx...\n");
        m_pTx->Release();
        m_pTx = NULL;

        return S_OK;
    }

    STDMETHODIMP Outcome::Committed(
                            BOOL fRetaining,
                            XACTUOW * pNewUOW,
                            HRESULT hr
                            )
    {
        printf("Outcome::Committed.\n");

        printf("Unadvising...\n");
        HRESULT localHr = m_pConnPt->Unadvise( m_dwOutcomeCookie );
        if ( FAILED( localHr ) )
        {
            printf("Error from IConnectionPoint::Unadvise - 0x%x.\n", localHr );
        }


        printf("Releasing pConnPt...\n");
        m_pConnPt->Release();
        m_pConnPt = NULL;

        printf("Release pTx...\n");
        m_pTx->Release();
        m_pTx = NULL;

        return S_OK;
    }

    STDMETHODIMP Outcome::HeuristicDecision(
                            DWORD dwDecision,
                            BOID * pboidReason,
                            HRESULT hr
                            )
    {
        printf("Outcome::HeuristicDecision.\n");

        printf("Unadvising...\n");
        HRESULT localHr = m_pConnPt->Unadvise( m_dwOutcomeCookie );
        if ( FAILED( localHr ) )
        {
            printf("Error from IConnectionPoint::Unadvise - 0x%x.\n", localHr );
        }


        printf("Releasing pConnPt...\n");
        m_pConnPt->Release();
        m_pConnPt = NULL;

        printf("Release pTx...\n");
        m_pTx->Release();
        m_pTx = NULL;

        return S_OK;
    }

    STDMETHODIMP Outcome::Indoubt(void)
    {
        printf("Outcome::InDoubt.\n");

        printf("Unadvising...\n");
        HRESULT localHr = m_pConnPt->Unadvise( m_dwOutcomeCookie );
        if ( FAILED( localHr ) )
        {
            printf("Error from IConnectionPoint::Unadvise - 0x%x.\n", localHr );
        }


        printf("Releasing pConnPt...\n");
        m_pConnPt->Release();
        m_pConnPt = NULL;

        printf("Release pTx...\n");
        m_pTx->Release();
        m_pTx = NULL;

        return S_OK;
    }

    STDMETHODIMP Outcome::CreateOutcomeConnectionPoint(ITransaction* pTransaction)
	{
		HRESULT hr = S_OK;
        IConnectionPointContainer* pConnPtContainer = NULL;
        Outcome* pOutcome = NULL;
        IUnknown* pOutcomeUnk = NULL;

		m_pTx = pTransaction;
		m_pTx->AddRef();

        hr = m_pTx->QueryInterface( IID_IConnectionPointContainer, (void**) &pConnPtContainer );
        if ( FAILED( hr ) )
        {
            printf( "QI for IConnectionPointContainer on pTx failed - 0x%x.\n", hr );
            goto ErrorExit;
        }

        hr = pConnPtContainer->FindConnectionPoint( IID_ITransactionOutcomeEvents, &m_pConnPt );
        if ( FAILED( hr ) )
        {
            printf( "IConnectionPointContainer::FindConnectionPoint failed - 0x%x.\n", hr );
            goto ErrorExit;
        }

        hr = this->QueryInterface( IID_IUnknown, (void**) &pOutcomeUnk );
        if ( FAILED( hr ) )
        {
            printf( "QI for IUnknown on Outcome object failed - 0x%x.\n", hr );
            goto ErrorExit;
        }

        hr = m_pConnPt->Advise( pOutcomeUnk, &m_dwOutcomeCookie );
        if ( FAILED( hr ) )
        {
            printf( "IConnectionPoint::Advise failed - 0x%x.\n", hr );
            goto ErrorExit;
        }

        printf ("Successfully created outcome connection point.\n");

ErrorExit:	
		if (pOutcomeUnk != NULL)
		{
			pOutcomeUnk->Release();
			pOutcomeUnk = NULL;
		}

		if (pConnPtContainer != NULL)
		{
			pConnPtContainer->Release();
			pConnPtContainer = NULL;
		}

		if (FAILED(hr))
		{
			if (this->m_pConnPt != NULL)
			{
				this->m_pConnPt->Release();
				this->m_pConnPt = NULL;
			}
		}

		return hr;
	}


