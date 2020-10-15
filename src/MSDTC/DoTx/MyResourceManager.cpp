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
#include "MyResourceManager.h"

MyResourceManager::MyResourceManager( GUID guid, char* name )
{
    m_cRef = 0;
    m_pIRm = NULL;
    m_guid = guid;
    m_name = new char[lstrlen(name) + 1];
    strcpy( m_name, name );
    InitializeCriticalSection( &m_CritSec );
}

MyResourceManager::~MyResourceManager()
{
    printf( "MyResourceManager dtor\n" );
    if ( NULL != m_pIRm )
    {
        m_pIRm->Release();
    }
    DeleteCriticalSection( &m_CritSec );
}

STDMETHODIMP MyResourceManager::QueryInterface( REFIID i_iid, LPVOID FAR* o_ppv )
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
    else if ( IID_IResourceManagerSink == i_iid )
    {
        *o_ppv = (IResourceManagerSink*) this;
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

STDMETHODIMP_ (ULONG) MyResourceManager::AddRef()
{
    return InterlockedIncrement( &m_cRef );
}

STDMETHODIMP_ (ULONG) MyResourceManager::Release()
{
    ULONG localRef = 0;

    localRef = InterlockedDecrement( &m_cRef );
    if ( 0 == localRef )
    {
        delete this;
    }

    return localRef;
}

STDMETHODIMP MyResourceManager::TMDown()
{
    HRESULT hr = S_OK;

    printf ("**** TMDown for %s ****\n", m_name );

    EnterCriticalSection( &m_CritSec );
    m_pIRm->Release();
    m_pIRm = NULL;
    LeaveCriticalSection( &m_CritSec );

    return S_OK;
}

STDMETHODIMP MyResourceManager::Enlist( IResourceManagerFactory2* pRmFactory2, ITransaction* pTxLocal, ITransactionResourceAsync* pResAsync1, ITransactionEnlistmentAsync** ppEnlistAsync )
{
    HRESULT hr = S_OK;
    XACTUOW dummyUow;
    LONG dummyIso;
    IResourceManager* localIRm = NULL;

    EnterCriticalSection( &m_CritSec );
    if ( NULL == m_pIRm )
    {
        printf( "Creating %s with DTC...\n", m_name );
        hr = pRmFactory2->CreateEx( (GUID*)&m_guid, m_name, this, IID_IResourceManager, (void**) &m_pIRm );
        if ( FAILED( hr ) )
        {
            printf( "Error 0x%x re-creating %s with DTC.\n", hr, m_name );
            goto Exit;
        }
        printf( "Calling ReenlistmentComplete for %s...\n", m_name );
        hr = m_pIRm->ReenlistmentComplete();
        if ( FAILED( hr ) )
        {
            printf( "Error 0x%x doing ReenlistmentComplete for %s.\n", hr, m_name );
            goto Exit;
        }
    }

    localIRm = m_pIRm;
    if ( NULL != localIRm )
    {
        localIRm->AddRef();
    }
Exit:
    LeaveCriticalSection( &m_CritSec );

    if ( NULL != localIRm )
    {
        printf( "Enlisting on %s...\n", m_name );
        hr = localIRm->Enlist( pTxLocal, pResAsync1, &dummyUow, &dummyIso, ppEnlistAsync );
        localIRm->Release();
    }

    return hr;

}

