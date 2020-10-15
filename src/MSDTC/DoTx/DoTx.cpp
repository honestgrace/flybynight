// DoTx.cpp : Defines the entry point for the console application.
//

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
#include "MyResourceManager.h"
#include "MyVoter.h"

extern "C"
{		
	struct xa_switch_t  msqlsrvxa1;
}

XID myXid;
int myRmId = 53;

IResourceManagerFactory2* pRmFactory2 = NULL;
ITransactionDispenser* pTxDisp = NULL;
ITransactionOptions* pTxOptions = NULL;
ITransactionVoterFactory2 *pVoterFactory = NULL;
ITransaction* pTx = NULL;

BOOL doOutcomeRelease = FALSE;
BOOL doSupportSPC = FALSE;


// Forward declarations.
DWORD WINAPI PrepareReqThreadRoutine( LPVOID lpParameter );
DWORD WINAPI TMDownThreadRoutine( LPVOID lpParameter );

class MyResourceAsync : public ITransactionResourceAsync
{
public:
    MyResourceAsync( int enlistNum, bool waitCommit, bool noCommitDone, bool promptPrepare, HANDLE phase2Event )
    {
        m_cRef = 0;
        m_waitCommit = waitCommit;
        m_noCommitDone = noCommitDone;
        m_EnlistAsync = NULL;
        m_enlistNum = enlistNum;
        m_promptPrepare = promptPrepare;
        m_phase2Event = phase2Event;
        printf( "Enlistment constructor - m_phase2event = 0x%x...\n", m_phase2Event );
        InitializeCriticalSection( &m_CritSec );
    }

    ~MyResourceAsync()
    {
        printf( "MyResourceAsync-%d dtor\n", m_enlistNum );
        if ( NULL != m_EnlistAsync )
        {
            m_EnlistAsync->Release();
        }
        DeleteCriticalSection( &m_CritSec );
    }


    virtual STDMETHODIMP QueryInterface( REFIID i_iid, LPVOID FAR* o_ppv )
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
        else if ( IID_ITransactionResourceAsync == i_iid )
        {
            *o_ppv = (ITransactionResourceAsync*) this;
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

    virtual STDMETHODIMP_ (ULONG) AddRef()
    {
        return InterlockedIncrement( &m_cRef );
    }

    virtual STDMETHODIMP_ (ULONG) Release()
    {
        ULONG localRef = 0;

        localRef = InterlockedDecrement( &m_cRef );
        if ( 0 == localRef )
        {
            delete this;
        }

        return localRef;
    }

    virtual STDMETHODIMP PrepareRequest(
        BOOL fRetaining,
        DWORD grfRM,
        BOOL fWantMoniker,
        BOOL fSinglePhase
    )
    {
        HRESULT promptResponse = S_OK;

        HRESULT hr = S_OK;
        HANDLE threadHandle = NULL;

        printf( "MyEnlistmentAsync-%d::PrepareRequest (singlePhase=%d) \n", m_enlistNum, fSinglePhase );

        if (m_promptPrepare)
        {
            printf( "MyEnlistmentAsync-%d::PrepareRequest - Starting another thread to do PrepareReqDone\n", m_enlistNum );
            threadHandle = CreateThread( NULL, 0, PrepareReqThreadRoutine, this, 0, NULL );
            if ( NULL == threadHandle )
            {
                printf("Error %d from CreateThread\n", GetLastError() );
            }
        }
        else
        {
            if ( fSinglePhase && doSupportSPC )
            {
                promptResponse = XACT_S_SINGLEPHASE;
            }
            EnterCriticalSection( &m_CritSec );
            if ( NULL != m_EnlistAsync )
            {
                printf( "MyEnlistmentAsync-%d::PrepareRequest - Calling PrepareRequestDone with 0x%x.\n", m_enlistNum, promptResponse );
                hr = m_EnlistAsync->PrepareRequestDone( promptResponse, NULL, NULL );
                if ( FAILED( hr ) )
                {
                    printf( "Error 0x%x returned from PrepareRequestDone.\n", hr );
                }
            }
            LeaveCriticalSection( &m_CritSec );

            if ( FAILED( promptResponse ) || ( XACT_S_SINGLEPHASE == promptResponse ) )
            {
                SetEvent( m_phase2Event );
            }
        }

        return hr;
    }

    virtual STDMETHODIMP AbortRequest(
        BOID * pboidReason,
        BOOL fRetaining,
        XACTUOW * pNewUOW
    )
    {
        HRESULT hr = S_OK;

        EnterCriticalSection( &m_CritSec );
        printf( "MyEnlistmentAsync-%d::AbortRequest\n", m_enlistNum );
        if ( NULL != m_EnlistAsync )
        {
            hr = m_EnlistAsync->AbortRequestDone( S_OK );
            if ( FAILED( hr ) )
            {
                printf( "Error 0x%x returned from AbortRequestDone.\n", hr );
                goto Exit;
            }

            m_EnlistAsync->Release();
            m_EnlistAsync = NULL;
            printf( "m_phase2event = 0x%x...\n", m_phase2Event );
            SetEvent( m_phase2Event );
        }

Exit:
        LeaveCriticalSection( &m_CritSec );
        return S_OK;
    }

    virtual STDMETHODIMP CommitRequest(
        DWORD grfRM,
        XACTUOW * pNewUOW
        )
    {
        HRESULT hr = S_OK;
        printf( "MyEnlistmentAsync-%d::CommitRequest\n", m_enlistNum );
        if ( m_waitCommit )
        {
            printf( "Hit any key to respond to CommitRequest.\n" );
            getchar();
        }

        EnterCriticalSection( &m_CritSec );

        if ( m_noCommitDone )
        {
            printf( "Not responding CommitRequestDone, as per -nocommitdone.\n" );
            printf( "m_phase2event = 0x%x...\n", m_phase2Event );
            SetEvent( m_phase2Event );
            goto Exit;
        }

        if ( NULL != m_EnlistAsync )
        {
            hr = m_EnlistAsync->CommitRequestDone( S_OK );
            if ( FAILED( hr ) )
            {
                printf( "Error 0x%x returned from CommitRequestDone.\n", hr );
                goto Exit;
            }

            m_EnlistAsync->Release();
            m_EnlistAsync = NULL;
            printf( "m_phase2event = 0x%x...\n", m_phase2Event );
            SetEvent( m_phase2Event );
        }

Exit:
        LeaveCriticalSection( &m_CritSec );

        return S_OK;
    }

    virtual STDMETHODIMP TMDown()
    {
        HANDLE threadHandle = NULL;
        if ( m_promptPrepare )
        {
            // Fire up a thread that will wait until we have finished before releasing the interface.
            printf( "MyEnlistmentAsync-%d::TMDownRequest - Starting another thread to do TMDown processing.\n", m_enlistNum );
            threadHandle = CreateThread( NULL, 0, TMDownThreadRoutine, this, 0, NULL );
            if ( NULL == threadHandle )
            {
                printf("Error %d from CreateThread\n", GetLastError() );
            }
        }
        else
        {
            EnterCriticalSection( &m_CritSec );
            printf( "MyEnlistmentAsync-%d::TMDown, m_EnlistAsync= 0x%x\n", m_enlistNum, m_EnlistAsync );
            if ( NULL != m_EnlistAsync )
            {
                m_EnlistAsync->Release();
                m_EnlistAsync = NULL;
            }
            printf( "m_phase2event = 0x%x...\n", m_phase2Event );
            SetEvent ( m_phase2Event );
            LeaveCriticalSection( &m_CritSec );
        }

        return S_OK;
    }

    void SetEnlistment( ITransactionEnlistmentAsync* enlistAsync )
    {
        EnterCriticalSection( &m_CritSec );
        printf( "MyEnlistmentAsync-%d::SetEnlistment, m_EnlistAsync= 0x%x, new value = 0x%x\n", m_enlistNum, m_EnlistAsync, enlistAsync );
        if ( NULL != m_EnlistAsync )
        {
            m_EnlistAsync->Release();
            m_EnlistAsync = NULL;
        }
        m_EnlistAsync = enlistAsync;
        LeaveCriticalSection( &m_CritSec );
    }

    ITransactionEnlistmentAsync* m_EnlistAsync;
    int m_enlistNum;
    HANDLE m_phase2Event;
    CRITICAL_SECTION m_CritSec;

private:
    LONG m_cRef;
    bool m_waitCommit;
    bool m_promptPrepare;
    bool m_noCommitDone;
};


DWORD WINAPI PrepareReqThreadRoutine( LPVOID lpParameter )
{
    MyResourceAsync* pResource = (MyResourceAsync*) lpParameter;
    HRESULT promptResponse = S_OK;
    HRESULT hr = S_OK;

    printf( "PrepareReq for Enlistment %d - Type Y to vote Yes, anything else to vote No.\n", pResource->m_enlistNum );
    if ( 'Y' != (char) getchar() )
    {
        promptResponse = E_FAIL;
    }

    EnterCriticalSection( &pResource->m_CritSec );
    if ( NULL != pResource->m_EnlistAsync )
    {
        hr = pResource->m_EnlistAsync->PrepareRequestDone( promptResponse, NULL, NULL );
        if ( FAILED( hr ) )
        {
            printf( "Error 0x%x returned from PrepareRequestDone.\n", hr );
        }
    }
    LeaveCriticalSection( &pResource->m_CritSec );

    if ( FAILED( promptResponse ) )
    {
        SetEvent( pResource->m_phase2Event );
    }

    return 0;
}

DWORD WINAPI TMDownThreadRoutine( LPVOID lpParameter )
{
    MyResourceAsync* pResource = (MyResourceAsync*) lpParameter;
    printf( "TMDownThreadRoutine - MyEnlistmentAsync-%d::TMDown, m_EnlistAsync= 0x%x\n", pResource->m_enlistNum, pResource->m_EnlistAsync );
    DWORD waitResult2 = WaitForSingleObject( pResource->m_phase2Event, 5000 );
    if ( WAIT_FAILED == waitResult2 )
    {
        printf("Wait for Phase2Event2 FAILED, error = %d.\n", GetLastError() );
        return -1;
    }
    else if ( WAIT_OBJECT_0 != waitResult2 )
    {
        printf("Wait for Phase2Event2 did not complete successfully - 0x%x, error = %d.\n", waitResult2, GetLastError() );
    }

    EnterCriticalSection( &pResource->m_CritSec );
    if ( NULL != pResource->m_EnlistAsync )
    {
        pResource->m_EnlistAsync->Release();
        pResource->m_EnlistAsync = NULL;
    }
    LeaveCriticalSection( &pResource->m_CritSec );
    return 0;
}

int rm1 = 0;
MyResourceManager* pRm1 = NULL;
// {7313B340-2020-48aa-B9D0-DD5097B13A42}
static const GUID guidRm1 = 
{ 0x7313b340, 0x2020, 0x48aa, { 0xb9, 0xd0, 0xdd, 0x50, 0x97, 0xb1, 0x3a, 0x42 } };
MyResourceAsync* pResAsync1 = NULL;


bool rm2 = FALSE;
MyResourceManager* pRm2 = NULL;
// {6A4897B3-EBC2-429c-9D08-09B9D326AF6B}
static const GUID guidRm2 = 
{ 0x6a4897b3, 0xebc2, 0x429c, { 0x9d, 0x8, 0x9, 0xb9, 0xd3, 0x26, 0xaf, 0x6b } };
MyResourceAsync* pResAsync2 = NULL;


bool waitCommit = FALSE;
bool promptPrepare = FALSE;
bool enlistTooLate = FALSE;
bool continuous = FALSE;
bool commitBeforeEnlist = FALSE;

DWORD timeout = 0;
DWORD descriptionLength = 10;
bool bRandomTimeout = FALSE;


char* pServerName = NULL;
char* pClusterDtcGuid = NULL;

// MAKEINTRESOURCE() returns an LPTSTR, but GetProcAddress()
// expects LPSTR even in UNICODE, so using MAKEINTRESOURCEA()...
#ifdef UNICODE
#define MAKEINTRESOURCEA_T(a, u) MAKEINTRESOURCEA(u)
#else
#define MAKEINTRESOURCEA_T(a, u) MAKEINTRESOURCEA(a)
#endif

BOOL myGUIDFromString(LPCTSTR psz, LPGUID pguid)
{
	BOOL bRet = FALSE;

	typedef BOOL(WINAPI *LPFN_GUIDFromString)(LPCTSTR, LPGUID);
	LPFN_GUIDFromString pGUIDFromString = NULL;

	HINSTANCE hInst = LoadLibrary(TEXT("shell32.dll"));
	if (hInst)
	{
		pGUIDFromString = (LPFN_GUIDFromString)GetProcAddress(hInst, MAKEINTRESOURCEA_T(703, 704));
		if (pGUIDFromString)
			bRet = pGUIDFromString(psz, pguid);
		FreeLibrary(hInst);
	}

	if (!pGUIDFromString)
	{
		hInst = LoadLibrary(TEXT("Shlwapi.dll"));
		if (hInst)
		{
			pGUIDFromString = (LPFN_GUIDFromString)GetProcAddress(hInst, MAKEINTRESOURCEA_T(269, 270));
			if (pGUIDFromString)
				bRet = pGUIDFromString(psz, pguid);
			FreeLibrary(hInst);
		}
	}

	return bRet;
}

HRESULT InitializeDtc()
{
    HRESULT hr = E_FAIL;

    while ( FAILED(hr) )
    {
        printf( "Getting transaction dispenser...\n" );

		// Alternatively, you can choose the DTC instance programatically instead of using the default DTC
		//  Choosing clustered DTC
		OLE_TM_CONFIG_PARAMS_V2 * pConfigParam = nullptr;
		OLE_TM_CONFIG_PARAMS_V2 configParam;
		
		GUID guid = {0};
		if (pClusterDtcGuid != NULL)
		{
			if (pServerName != NULL)
			{
				printf("Can't set both -Server and -ClusterDTCGUID together.\n");
				throw std::invalid_argument("Can't set both -Server and -ClusterDTCGUID together");
			}

			printf("Initializng configuration for Cluster DTC role...\n");
			if (!myGUIDFromString(pClusterDtcGuid, &guid))
			{
				printf("Can't validate the value of -ClusterDTCGUID.\n");
				throw std::invalid_argument("Can't validate the value of -ClusterDTCGUID");
			}
			configParam.applicationType = APPLICATIONTYPE::CLUSTERRESOURCE_APPLICATIONTYPE;
			configParam.clusterResourceId = guid;
			configParam.dwcConcurrencyHint = 0;
			configParam.dwVersion = OLE_TM_CONFIG_VERSION_2;			
			pConfigParam = &configParam;
		}
		
		hr = DtcGetTransactionManagerExA( pServerName, NULL, IID_ITransactionDispenser, 0, pConfigParam, (void **)&pTxDisp );
        if ( FAILED(hr) )
        {
            printf( "Error 0x%x Getting transaction dispenser.\n", hr );
            printf( "Hit any key to commit and loop again.  Hit CTRL-C to exit.\n" );
            getchar();
        }
    }

    hr = pTxDisp->QueryInterface( IID_ITransactionVoterFactory2, (void**) &pVoterFactory );
    if ( FAILED( hr ) )
    {
        printf( "Error 0x%x Getting ITransactionVoterFactory2.\n", hr );
        goto Exit;
    }

    hr = pTxDisp->QueryInterface( IID_IResourceManagerFactory2, (void**) &pRmFactory2 );
    if ( FAILED( hr ) )
    {
        printf( "Error 0x%x Getting IResourceManagerFactory2.\n", hr );
        goto Exit;
    }

    hr = pTxDisp->GetOptionsObject( &pTxOptions );
    if ( FAILED(hr) )
    {
        printf( "Error 0x%x Getting ITransactionOptions object.\n", hr );
        goto Exit;
    }

Exit:

    return hr;
}

int _tmain(int argc, _TCHAR* argv[])
{
    HRESULT hr = S_OK;
    XACTTRANSINFO txInfo;
    WCHAR szTxId[50];
    BOOL releaseEnlistBeforeCommit = FALSE;
    HANDLE phase2Event1 = NULL;
    HANDLE phase2Event2 = NULL;
    HANDLE voterPhase2Event = NULL;
    HANDLE voterBeforePromotePhase2Event = NULL;
    BOOL doVoter = FALSE;
	BOOL voteAbort = FALSE;
    BOOL doVoterBeforePromote = FALSE;
    BOOL voteEarly = FALSE;
    MyVoter* pVoter = NULL;
    MyVoter* pVoterBeforePromote = NULL;
    BOOL noCommit = FALSE;
    BOOL naConfig = FALSE;
    BOOL bModifyAllowInbound = FALSE;
    BOOL allowInboundValue = FALSE;
    BOOL once = FALSE;
    BOOL nocommitdone = FALSE;
    BOOL doXaSuperior = FALSE;
    BOOL doXaSPC = FALSE;
    DWORD loopCount = 0;
    BOOL displayTipId = FALSE;
	BOOL useRandomRmId = FALSE;

    phase2Event1 = CreateEvent(
        NULL,
        TRUE,
        TRUE,
        NULL
        );
    if ( NULL == phase2Event1 )
    {
        printf("Error from CreateEvent\n");
        return -1;
    }
    printf( "phase2event1 = 0x%x...\n", phase2Event1 );

    phase2Event2 = CreateEvent(
        NULL,
        TRUE,
        TRUE,
        NULL
        );
    if ( NULL == phase2Event2 )
    {
        printf("Error from CreateEvent\n");
        return -1;
    }
    printf( "phase2event2 = 0x%x...\n", phase2Event2 );

    voterPhase2Event = CreateEvent(
        NULL,
        TRUE,
        TRUE,
        NULL
        );
    if ( NULL == voterPhase2Event )
    {
        printf("Error from CreateEvent\n");
        return -1;
    }
    printf( "voterPhase2event = 0x%x...\n", voterPhase2Event );

    voterBeforePromotePhase2Event = CreateEvent(
        NULL,
        TRUE,
        TRUE,
        NULL
        );
    if ( NULL == voterBeforePromotePhase2Event )
    {
        printf("Error from CreateEvent\n");
        return -1;
    }
    printf( "voterBeforePromotePhase2event = 0x%x...\n", voterBeforePromotePhase2Event );


    if ( argc <= 1 )
    {
        goto Usage;
    }

    for ( int i = 0; i < argc; i++ )
    {
        if ( 0 == _mbsicmp( (const unsigned char*) argv[i], (const unsigned char*) "-?" ) )
        {
            goto Usage;
        }

        else if ( 0 == _mbsicmp( (const unsigned char*) argv[i], (const unsigned char*) "-rm1" ) )
        {
            i++;
            rm1 = _ttoi( argv[i] );
            // limit to one because I only create one phase2 event.
            rm1 = 1;
        }
        else if ( 0 == _mbsicmp( (const unsigned char*) argv[i], (const unsigned char*) "-rm2" ) )
        {
            rm2 = TRUE;
        }
        else if ( 0 == _mbsicmp( (const unsigned char*) argv[i], (const unsigned char*) "-waitcommit" ) )
        {
            waitCommit = TRUE;
        }
        else if ( 0 == _mbsicmp( (const unsigned char*) argv[i], (const unsigned char*) "-promptprepare" ) )
        {
            promptPrepare = TRUE;
        }
        else if ( 0 == _mbsicmp( (const unsigned char*) argv[i], (const unsigned char*) "-toolate" ) )
        {
            enlistTooLate = TRUE;
        }
        else if ( 0 == _mbsicmp( (const unsigned char*) argv[i], (const unsigned char*) "-continuous" ) )
        {
            continuous = TRUE;
        }
        else if ( 0 == _mbsicmp( (const unsigned char*) argv[i], (const unsigned char*) "-commitbeforeenlist" ) )
        {
            commitBeforeEnlist = TRUE;
        }
        else if ( 0 == _mbsicmp( (const unsigned char*) argv[i], (const unsigned char*) "-releaseenlistbeforecommit" ) )
        {
            releaseEnlistBeforeCommit = TRUE;
        }
        else if ( 0 == _mbsicmp( (const unsigned char*) argv[i], (const unsigned char*) "-server" ) )
        {
            i++;
            int size = strlen( argv[i] );
            pServerName = new char [size+1];
            if ( NULL == pServerName )
            {
                printf ("Unable to allocate space for server name\n" );
                return -1;
            }
            strcpy( pServerName, argv[i] );
            printf( "Using TM on node %s\n", pServerName );
        }
		else if (0 == _mbsicmp((const unsigned char*)argv[i], (const unsigned char*) "-clusterDtcGuid"))
		{
			i++;
			int size = strlen(argv[i]);
			pClusterDtcGuid = new char[size + 1];
			if (NULL == pClusterDtcGuid)
			{
				printf("Unable to allocate space for clusterDtcGuid\n");
				return -1;
			}
			strcpy(pClusterDtcGuid, argv[i]);
			printf("Using Cluster DTC role of %s\n", pClusterDtcGuid);
		}
        else if ( 0 == _mbsicmp( (const unsigned char*) argv[i], (const unsigned char*) "-timeout" ) )
        {
            i++;
            timeout = _ttoi( argv[i] ) * 1000;
        }
        else if ( 0 == _mbsicmp( (const unsigned char*) argv[i], (const unsigned char*) "-randomtimeout" ) )
        {
            bRandomTimeout = TRUE;
            srand( 54 );
        }
        else if ( 0 == _mbsicmp( (const unsigned char*) argv[i], (const unsigned char*) "-voter" ) )
        {
            doVoter = TRUE;
        }
		else if ( 0 == _mbsicmp( (const unsigned char*) argv[i], (const unsigned char*) "-voteabort" ) )
        {
            voteAbort = TRUE;
        }
        else if ( 0 == _mbsicmp( (const unsigned char*) argv[i], (const unsigned char*) "-voterbeforepromote" ) )
        {
            doVoterBeforePromote = TRUE;
        }
        else if ( 0 == _mbsicmp( (const unsigned char*) argv[i], (const unsigned char*) "-voteearly" ) )
        {
            voteEarly = TRUE;
        }
        else if ( 0 == _mbsicmp( (const unsigned char*) argv[i], (const unsigned char*) "-nocommit" ) )
        {
            noCommit = TRUE;
        }
        else if ( 0 == _mbsicmp( (const unsigned char*) argv[i], (const unsigned char*) "-nullserver" ) )
        {
            pServerName = new char [1];
            if ( NULL == pServerName )
            {
                printf ("Unable to allocate space for server name\n" );
                return -1;
            }
            pServerName[0] = 0;
            printf( "Using null string for server name.\n" );
        }
        else if ( 0 == _mbsicmp( (const unsigned char*) argv[i], (const unsigned char*) "-descriptionlength" ) )
        {
            i++;
            descriptionLength = _ttoi( argv[i] );
        }
        else if ( 0 == _mbsicmp( (const unsigned char*) argv[i], (const unsigned char*) "-naconfig" ) )
        {
            naConfig = TRUE;
        }

        else if ( 0 == _mbsicmp( (const unsigned char*) argv[i], (const unsigned char*) "-allowinbound" ) )
        {
            bModifyAllowInbound = TRUE;
            i++;
            if ( 0 != _ttoi( argv[i] ) )
            {
                allowInboundValue = TRUE;
            }
            else
            {
                allowInboundValue = FALSE;
            }
        }
        else if ( 0 == _mbsicmp( (const unsigned char*) argv[i], (const unsigned char*) "-once" ) )
        {
            once = TRUE;
        }

        else if ( 0 == _mbsicmp( (const unsigned char*) argv[i], (const unsigned char*) "-nocommitdone" ) )
        {
            nocommitdone = TRUE;
        }
        else if ( 0 == _mbsicmp( (const unsigned char*) argv[i], (const unsigned char*) "-outcomerelease" ) )
        {
            doOutcomeRelease = TRUE;
        }
        else if ( 0 == _mbsicmp( (const unsigned char*) argv[i], (const unsigned char*) "-xasuperior" ) )
        {
            doXaSuperior = TRUE;
        }
        else if ( 0 == _mbsicmp( (const unsigned char*) argv[i], (const unsigned char*) "-xaspc" ) )
        {
            doXaSPC = TRUE;
        }
        else if ( 0 == _mbsicmp( (const unsigned char*) argv[i], (const unsigned char*) "-supportspc" ) )
        {
            doSupportSPC = TRUE;
        }
        else if ( 0 == _mbsicmp( (const unsigned char*) argv[i], (const unsigned char*) "-displaytipid" ) )
        {
            displayTipId = TRUE;
        }
        else if ( 0 == _mbsicmp( (const unsigned char*) argv[i], (const unsigned char*) "-userandomrmid" ) )
        {
            useRandomRmId = TRUE;
        }
    }

    hr = CoInitializeEx(NULL,0);
    if ( FAILED(hr) )
    {
        printf( "Error 0x%x from CoInitializeEx.\n", hr );
        return -1;
    }

    hr = InitializeDtc();
    if ( FAILED( hr ) )
    {
        printf( "Error 0x%x Initializing DTC.\n", hr );
        return -1;
    }

    if ( 0 < rm1 )
    {
		GUID rmid1 = guidRm1;
		if (useRandomRmId)
		{
			hr = CoCreateGuid(&rmid1);
			if (FAILED(hr))
			{
				printf("Error 0x%x from CoCreateGuid.\n", hr);
				return -1;
			}
		}

        pRm1 = new MyResourceManager(rmid1, "DoTxRm1");
        if ( NULL == pRm1 )
        {
            printf( "Unable to allocate RM1 object.\n" );
            return -1;
        }
        pRm1->AddRef();
    }

    if ( rm2 )
    {
		GUID rmid2 = guidRm2;
		if (useRandomRmId)
		{
			hr = CoCreateGuid(&rmid2);
			if (FAILED(hr))
			{
				printf("Error 0x%x from CoCreateGuid.\n", hr);
				return -1;
			}
		}
        pRm2 = new MyResourceManager(rmid2, "DoTxRm2");
        if ( NULL == pRm2 )
        {
            printf( "Unable to allocate RM2 object.\n" );
            return -1;
        }
        pRm2->AddRef();
    }

    while (TRUE)
    {
        if ( naConfig )
        {
            // Retrieve the NetworkAccess stuff and display it, then prompt for input
            IDtcNetworkAccessConfig2* pDtcNaConfig = NULL;

            hr = pTxDisp->QueryInterface( IID_IDtcNetworkAccessConfig2, (void**) &pDtcNaConfig );
            if ( FAILED( hr ) )
            {
                printf( "Error 0x%x from QI for IDtcNetworkAccessConfig2.\n", hr );
                goto tryAgain;
            }

            BOOL bAnyNetworkAccess = FALSE;
            hr = pDtcNaConfig->GetAnyNetworkAccess( &bAnyNetworkAccess );
            if ( FAILED( hr ) )
            {
                printf( "Error 0x%x from GetAnyNetworkAccess.\n", hr );
                goto tryAgain;
            }

            BOOL bNetworkAdministrationAccess = FALSE;
            hr = pDtcNaConfig->GetNetworkAdministrationAccess( &bNetworkAdministrationAccess );
            if ( FAILED( hr ) )
            {
                printf( "Error 0x%x from GetNetworkAdministrationAccess.\n", hr );
                goto tryAgain;
            }

            BOOL bNetworkTxAccess = FALSE;
            hr = pDtcNaConfig->GetNetworkTransactionAccess( &bNetworkTxAccess );
            if ( FAILED( hr ) )
            {
                printf( "Error 0x%x from GetNetworkTransactionAccess.\n", hr );
                goto tryAgain;
            }

            BOOL bNetworkClientAccess = FALSE;
            hr = pDtcNaConfig->GetNetworkClientAccess( &bNetworkClientAccess );
            if ( FAILED( hr ) )
            {
                printf( "Error 0x%x from GetNetworkClientAccess.\n", hr );
                goto tryAgain;
            }

            BOOL bNetworkTipAccess = FALSE;
            hr = pDtcNaConfig->GetNetworkTIPAccess( &bNetworkTipAccess );
            if ( FAILED( hr ) )
            {
                printf( "Error 0x%x from GetNetworkTIPAccess.\n", hr );
                goto tryAgain;
            }

            BOOL bXAAccess = FALSE;
            hr = pDtcNaConfig->GetXAAccess( &bXAAccess );
            if ( FAILED( hr ) )
            {
                printf( "Error 0x%x from GetXAAccess.\n", hr );
                goto tryAgain;
            }

            BOOL bNetworkInboundAccess = FALSE;
            hr = pDtcNaConfig->GetNetworkInboundAccess( &bNetworkInboundAccess );
            if ( FAILED( hr ) )
            {
                printf( "Error 0x%x from GetNetworkInboundAccess.\n", hr );
                goto tryAgain;
            }

            BOOL bNetworkOutboundAccess = FALSE;
            hr = pDtcNaConfig->GetNetworkOutboundAccess( &bNetworkOutboundAccess );
            if ( FAILED( hr ) )
            {
                printf( "Error 0x%x from GetNetworkOutboundAccess.\n", hr );
                goto tryAgain;
            }

            AUTHENTICATION_LEVEL authLevel = NO_AUTHENTICATION_REQUIRED;
            hr = pDtcNaConfig->GetAuthenticationLevel( &authLevel );
            if ( FAILED( hr ) )
            {
                printf( "Error 0x%x from GetAuthenticationLevel.\n", hr );
                goto tryAgain;
            }

            printf( "AnyNetworkAccess = %d\n", bAnyNetworkAccess );
            printf( "NetworkAdminAccess = %d\n", bNetworkAdministrationAccess );
            printf( "NetworkTxAccess = %d\n", bNetworkTxAccess );
            printf( "NetworkClientAccess = %d\n", bNetworkClientAccess );
            printf( "XaAccess = %d\n", bNetworkTipAccess );
            printf( "NetworkTipAccess = %d\n", bXAAccess );
            printf( "NetworkInboundAccess = %d\n", bNetworkInboundAccess );
            printf( "NetworkOutboundAccess = %d\n", bNetworkOutboundAccess );
            printf( "AuthLevel = %d\n", authLevel );

            if ( bModifyAllowInbound )
            {
                hr = pDtcNaConfig->SetNetworkInboundAccess( allowInboundValue );
                if ( FAILED( hr ) )
                {
                    printf( "Error 0x%x from SetNetworkInboundAccess.\n", hr );
                    goto tryAgain;
                }

                hr = pDtcNaConfig->RestartDtcService();
                if ( FAILED( hr ) )
                {
                    printf( "Error 0x%x from RestartDtcService.\n", hr );
                    goto tryAgain;
                }
            }

tryAgain:
        printf( "Hit any key to loop again.  Hit CTRL-C to exit.\n" );
        getchar();
        continue;
        }

        XACTOPT options;


        if ( bRandomTimeout )
        {
            // This will give us a timeout value somewhere between 1 second and 17 minutes plus
            // a little.
            options.ulTimeout = (rand() + 1000) & 0xFFFFF;

        }
        else
        {
            options.ulTimeout = timeout;
        }
        printf( "Beginning transaction with timeout %d milliseconds...\n", options.ulTimeout );
        memset( (void*) &options.szDescription, 0, 40 );
        memset( (void*) &options.szDescription, 0x78, descriptionLength );  // 0x78 = "x"

        hr = pTxOptions->SetOptions( &options );
        if ( FAILED(hr) )
        {
            if ( ( XACT_E_TMNOTAVAILABLE == hr ) || ( XACT_E_CONNECTION_DOWN == hr ) )
            {
                HRESULT hr2 = S_OK;
                pTxDisp->Release();
                pTxDisp = NULL;
                pVoterFactory->Release();
                pVoterFactory = NULL;
                pRmFactory2->Release();
                pRmFactory2 = NULL;
                pTxOptions->Release();
                pTxOptions = NULL;

                hr2 = InitializeDtc();
                if ( FAILED( hr2 ) )
                {
                    printf( "Error 0x%x Initializing DTC.\n", hr2 );
                    return -1;
                }

            }
        }

        if ( doXaSuperior )
        {
            int xaerr = XA_OK;
            loopCount++;

            char szLoopCount[20] = "";
            strcpy( myXid.data, "JCARLEYXIDGTRID" );
            sprintf( szLoopCount, "%d", loopCount );
            strcat( myXid.data, szLoopCount );
            myXid.gtrid_length = strlen( myXid.data );

            printf( "Doing XA Superior for XID %s\n", myXid.data );

            xaerr = msqlsrvxa1.xa_open_entry("TM=xaresumetx,RmRecoveryGuid=8FABAA36-FDC5-4bb7-9DFD-DF1CF312A000,Timeout=30000", myRmId, 0 );
            if ( XA_OK != xaerr )
            {
                printf( "Error %d from xa_open\n", xaerr );
                return -1;
            }
            printf( "XaOpen succeeded.\n" );

            xaerr = msqlsrvxa1.xa_start_entry(&myXid, myRmId, 0);
            if ( XA_OK != xaerr )
            {
                printf( "Error %d from xa_start\n", xaerr );
                return -1;
            }
            printf( "XaStart succeeded.\n" );


            printf( "Getting the ITransaction from the XID.\n" );
            IXATransLookup2 * pXaLookup = NULL;
            hr = pTxDisp->QueryInterface( IID_IXATransLookup2, (void**) &pXaLookup );
            if ( FAILED(hr) )
            {
                printf( "Error %x from QI for IID_IXATransLookup\n", hr );
                return -1;
            }

            hr = pXaLookup->Lookup( &myXid, &pTx );
            if ( FAILED(hr) )
            {
                printf( "Error %x from QI for IXATransLookup::LookUp\n", hr );
                return -1;
            }

            for ( int j = 0; j < rm1 ; j++ )
            {
                ResetEvent( phase2Event1 );
                printf( "Enlisting with RM1...\n" );
                pResAsync1 = new MyResourceAsync( 1, FALSE, FALSE, FALSE, phase2Event1 );
                pResAsync1->AddRef();
                ITransactionEnlistmentAsync* enlistAsync = NULL;
                hr = pRm1->Enlist( pRmFactory2, pTx, pResAsync1, &enlistAsync );
                if ( FAILED(hr) )
                {
                    printf( "Error 0x%x doing enlistment for RM1.\n", hr );
                    continue;
                }
                pResAsync1->SetEnlistment( enlistAsync );
            }


            if ( doXaSPC )
            {
                printf( "Trying XA SinglePhaseCommit.\n" );
                xaerr = msqlsrvxa1.xa_commit_entry(&myXid, myRmId, TMONEPHASE);
                if ( XA_OK != xaerr )
                {
                    printf( "Error %d from xa_commit.\n", xaerr );
                    return -1;
                }
                printf( "XaCommit (SPC) succeeded.\n" );

            }
            else
            {
                xaerr = msqlsrvxa1.xa_prepare_entry(&myXid, myRmId, 0);
                if ( XA_OK != xaerr )
                {
                    printf( "Error %d from xa_prepare.\n", xaerr );
                    return -1;
                }
                printf( "XaPrepare succeeded.\n" );

                xaerr = msqlsrvxa1.xa_commit_entry(&myXid, myRmId, 0);
                if ( XA_OK != xaerr )
                {
                    printf( "Error %d from xa_commit.\n", xaerr );
                    return -1;
                }
                printf( "XaCommit succeeded.\n" );
            }

            DWORD waitResult1 = WaitForSingleObject( phase2Event1, 5000 );
            if ( WAIT_FAILED == waitResult1 )
            {
                printf("Wait for Phase2Event1 FAILED, error = %d.\n", GetLastError() );
                printf( "phase2event1 = 0x%x...\n", phase2Event1 );
                return -1;
            }
            else if ( WAIT_OBJECT_0 != waitResult1 )
            {
                printf("Wait for Phase2Event1 did not complete successfully - 0x%x, error = %d.\n", waitResult1, GetLastError() );
                printf( "phase2event1 = 0x%x...\n", phase2Event1 );
            }


            pTx->Release();
            pTx = NULL;

tryAgainXa:
            printf( "Hit any key to loop again.  Hit CTRL-C to exit.\n" );
            getchar();
            continue;
        }

        hr = pTxDisp->BeginTransaction( NULL, ISOLATIONLEVEL_SERIALIZABLE, 0, pTxOptions, &pTx );
        if ( FAILED(hr) )
        {
            if ( ( XACT_E_TMNOTAVAILABLE == hr ) || ( XACT_E_CONNECTION_DOWN == hr ) )
            {
                HRESULT hr2 = S_OK;
                pTxDisp->Release();
                pTxDisp = NULL;
                pVoterFactory->Release();
                pVoterFactory = NULL;
                pRmFactory2->Release();
                pRmFactory2 = NULL;

                hr2 = InitializeDtc();
                if ( FAILED( hr2 ) )
                {
                    printf( "Error 0x%x Initializing DTC.\n", hr2 );
                    return -1;
                }

            }

            printf( "Error 0x%x beginning transaction.\n", hr );
            continue;
        }

        if ( displayTipId )
        {
            ITipTransaction* pTipTransaction = NULL;
            char* pTipId;

            hr = pTx->QueryInterface(IID_ITipTransaction, (void**) &pTipTransaction);
            if (FAILED(hr))
            {
                printf( "Error 0x%x from QI for ITipTransaction.\n", hr );
                return -1;
            }

            hr = pTipTransaction->GetTransactionUrl( &pTipId );
            if (FAILED(hr))
            {
                printf( "Error 0x%x from GetTransactionUrl.\n", hr );
                return -1;
            }
            printf("TIP URL = %s\n", pTipId);

            pTipTransaction->Release();
            pTipTransaction = NULL;
            CoTaskMemFree(pTipId);
        }

        if ( doVoterBeforePromote )
        {
            ResetEvent( voterBeforePromotePhase2Event );
            pVoterBeforePromote = (MyVoter*) new MyVoter( !voteAbort, voterBeforePromotePhase2Event );
            pVoterBeforePromote->AddRef();

			hr = pVoterBeforePromote->CreateVoter(pVoterFactory, pTx);
			if (FAILED(hr))
			{
				printf("Failed to create voter before promote - 0x%x\n", hr);
				continue;
			}

            if ( voteEarly )
            {
                hr = pVoterBeforePromote->VoteNow();
                printf( "HRESULT from voting early - 0x%x\n", hr );
            }
        }

        hr = pTx->GetTransactionInfo( &txInfo );
        if ( FAILED(hr) )
        {
            printf( "Error 0x%x getting transaction info.\n", hr );
            pTx->Abort( NULL, FALSE, FALSE );
            pTx = NULL;
            continue;
        }
        if ( 0 == StringFromGUID2( (REFGUID) txInfo.uow, (LPOLESTR) szTxId, 50 ) )
        {
            printf( "Unable to convert TXID to string.\n" );
            pTx->Abort( NULL, FALSE, FALSE );
            pTx = NULL;
            continue;
        }
        wprintf( L"TransactionId = %s\n", szTxId );

        if ( commitBeforeEnlist )
        {
            hr = pTx->Commit( FALSE, 0, 0 );
            if ( FAILED(hr) )
            {
                printf( "Error 0x%x committing transaction.\n", hr );
                continue;
            }
        }

        if ( doVoter )
        {
            ResetEvent( voterPhase2Event );
            pVoter = (MyVoter*) new MyVoter( !voteAbort, voterPhase2Event );
            pVoter->AddRef();

			hr = pVoter->CreateVoter(pVoterFactory, pTx);
			if (FAILED(hr))
			{
				printf("Failed to create voter - 0x%x\n", hr);
				continue;
			}

            if ( voteEarly )
            {
                hr = pVoter->VoteNow();
                printf( "HRESULT from voting early - 0x%x\n", hr );
            }
        }

        for ( int j = 0; j < rm1 ; j++ )
        {
            ResetEvent( phase2Event1 );
            printf( "Enlisting with RM1...\n" );
            pResAsync1 = new MyResourceAsync( 1, FALSE, FALSE, FALSE, phase2Event1 );
            pResAsync1->AddRef();
            ITransactionEnlistmentAsync* enlistAsync = NULL;
            hr = pRm1->Enlist( pRmFactory2, pTx, pResAsync1, &enlistAsync );
            if ( FAILED(hr) )
            {
                printf( "Error 0x%x doing enlistment for RM1.\n", hr );
                continue;
            }
            pResAsync1->SetEnlistment( enlistAsync );
        }

        if ( rm2 )
        {
            ResetEvent( phase2Event2 );
            printf( "Enlisting with RM2...\n" );
            pResAsync2 = new MyResourceAsync( 2, waitCommit, nocommitdone, promptPrepare, phase2Event2 );
            pResAsync2->AddRef();
            ITransactionEnlistmentAsync* enlistAsync = NULL;
            hr = pRm2->Enlist( pRmFactory2, pTx, pResAsync2, &enlistAsync );
            if ( FAILED(hr) )
            {
                printf( "Error 0x%x doing enlistment for RM2.\n", hr );
                continue;
            }
            pResAsync2->SetEnlistment( enlistAsync );
        }

        if ( doOutcomeRelease )
        {
            Outcome* pOutcome = new Outcome();
            if ( NULL == pOutcome )
            {
                printf( "Unable to allocate Outcome object.\n" );
                return -1;
            }

			hr = pOutcome->CreateOutcomeConnectionPoint(pTx);
			if (FAILED(hr))
			{
				printf("Failed to create outcome connection point - 0x%x.\n", hr);
			}
        }

        if ( !continuous )
        {
            printf( "Hit any key to commit and loop again.  Hit CTRL-C to exit.\n" );
            getchar();
        }

        if ( releaseEnlistBeforeCommit )
        {
			if ( NULL != pResAsync1 )
            {
                pResAsync1->SetEnlistment( NULL );
            }
        }

        if ( !commitBeforeEnlist && !noCommit )
        {
			hr = pTx->Commit( FALSE, 0, 0 );
            if ( FAILED(hr) )
            {
                printf( "Error 0x%x committing transaction.\n", hr );
            }
        }

        if ( !noCommit )
        {
            if ( enlistTooLate && rm1 )
            {
                printf( "Enlisting too late with RM1...\n" );
                pResAsync1 = new MyResourceAsync( 1, FALSE, FALSE, FALSE, NULL );
                pResAsync1->AddRef();
                ITransactionEnlistmentAsync* enlistAsync = NULL;
                hr = pRm1->Enlist( pRmFactory2, pTx, pResAsync1, &enlistAsync );
                if ( SUCCEEDED(hr) )
                {
                    printf( "UNEXPECTED - No error 0x%x doing enlistment too late for RM1.\n", hr );
                    enlistAsync->Release();
                    continue;
                }
                else
                {
                    printf( "EXPECTED - Error 0x%x doing enlistment too late for RM1.\n", hr );
                }
            }

            DWORD waitResult1 = WaitForSingleObject( phase2Event1, 5000 );
            if ( WAIT_FAILED == waitResult1 )
            {
                printf("Wait for Phase2Event1 FAILED, error = %d.\n", GetLastError() );
                printf( "phase2event1 = 0x%x...\n", phase2Event1 );
                return -1;
            }
            else if ( WAIT_OBJECT_0 != waitResult1 )
            {
                printf("Wait for Phase2Event1 did not complete successfully - 0x%x, error = %d.\n", waitResult1, GetLastError() );
                printf( "phase2event1 = 0x%x...\n", phase2Event1 );
            }

            // If we are prompting for prepare, we don't want this thread to continue until we process
            // things.  I am trying to test a tmdown while prepare is outstanding, and if I don't do this,
            // this thread would timeout on the wait for phase2event2 and move on to the next transaction.
            DWORD timeout = 5000;
            if ( promptPrepare )
            {
                timeout = INFINITE;
            }

            DWORD waitResult2 = WaitForSingleObject( phase2Event2, timeout );
            if ( WAIT_FAILED == waitResult2 )
            {
                printf("Wait for Phase2Event2 FAILED, error = %d.\n", GetLastError() );
                printf( "phase2event2 = 0x%x...\n", phase2Event2 );
                return -1;
            }
            else if ( WAIT_OBJECT_0 != waitResult2 )
            {
                printf("Wait for Phase2Event2 did not complete successfully - 0x%x, error = %d.\n", waitResult2, GetLastError() );
                printf( "phase2event2 = 0x%x...\n", phase2Event2 );
            }

            DWORD waitResultVoter = WaitForSingleObject( voterPhase2Event, 5000 );
            if ( WAIT_FAILED == waitResultVoter )
            {
                printf("Wait for voterPhase2Event FAILED, error = %d.\n", GetLastError() );
                printf( "voterPhase2Event = 0x%x...\n", voterPhase2Event );
                return -1;
            }
            else if ( WAIT_OBJECT_0 != waitResultVoter )
            {
                printf("Wait for voterPhase2Event did not complete successfully - 0x%x, error = %d.\n", waitResultVoter, GetLastError() );
                printf( "voterPhase2Event = 0x%x...\n", voterPhase2Event );
            }

            waitResultVoter = WaitForSingleObject( voterBeforePromotePhase2Event, 5000 );
            if ( WAIT_FAILED == waitResultVoter )
            {
                printf("Wait for voterBeforePromotePhase2Event FAILED, error = %d.\n", GetLastError() );
                printf( "voterBeforePromotePhase2Event = 0x%x...\n", voterPhase2Event );
                return -1;
            }
            else if ( WAIT_OBJECT_0 != waitResultVoter )
            {
                printf("Wait for voterBeforePromotePhase2Event did not complete successfully - 0x%x, error = %d.\n", waitResultVoter, GetLastError() );
                printf( "voterBeforePromotePhase2Event = 0x%x...\n", voterPhase2Event );
            }

            if ( NULL != pVoterBeforePromote )
            {
                pVoterBeforePromote->Release();
                pVoterBeforePromote = NULL;
            }

            if ( NULL != pVoter )
            {
                pVoter->Release();
                pVoter = NULL;
            }
            
            pTx->Release();
			pTx = NULL;

            if ( NULL != pResAsync1 )
            {
                pResAsync1->Release();
                pResAsync1 = NULL;
            }

            if ( NULL != pResAsync2 )
            {
                pResAsync2->Release();
                pResAsync2 = NULL;
            }
        }

        if ( once )
        {
            break;
        }
    }

    if ( NULL != pRm1 )
    {
        pRm1->Release();
    }
    if ( NULL != pRm2 )
    {
        pRm2->Release();
    }
Usage:

    printf( "USAGE:\n");
    printf( "  dotx [arguments]\n");
    printf( "     where arguments are:\n");
    printf( "\n");
    printf( "     -rm1 n - create n resource managers and enlist the tx on them (currently limited to 1)\n");
    printf( "     -rm2 - create a second rm.  This one uses -waitcommit, -promptprepare, and -nocommitdone below.\n");
    printf( "     -waitcommit - if you have an rm2, prompt user when CommitRequest is called.\n" );
    printf( "     -promptprepare - if you have an rm2, prompt user when PrepareRequest is called.\n" );
    printf( "     -nocommitdone - if you have an rm2, do not respond to CommitRequest with CommitRequestDone.\n" );
    printf( "     -toolate - try to create an enlistment AFTER commit.\n" );
    printf( "     -continuous - do not prompt before calling Commit, just do it and loop again.\n" );
    printf( "     -commitbeforeenlist - commit the tx before ANY enlistments - slightly different than toolate.\n " );
    printf( "     -releaseenlistbeforecommit - release the enlistment object before calling commit - early no vote.\n" );
    printf( "     -server hostname - use the TM on hosthame - programmatic remote proxy.\n" );
	printf( "     -clusterDtcGuid {GUID} - use the GUID value of the DTC cluster on GUID.\n");
    printf( "     -timeout n - number of seconds for tx timeout - default is no timeout.\n" );
    printf( "     -randomtimeout - generate a random number for timeout.\n" );
    printf( "     -voter - create a voter AFTER RM enlistment (after promotion on Vista/Longhorn.\n" );
    printf( "     -voterbeforepromote - create a voter BEFORE RM enlistment (before promotion on Vista/Longhorn.\n" );
    printf( "     -voteearly - call VoteRequestDone BEFORE Commit.  Noop if you don’t have a voter.\n" );
    printf( "     -nocommit - do not commit the tx, just release the ITransaction.\n" );
    printf( "     -nullserver - do DtcGetTransactionManagerEx with an empty string for hostname.\n" );
    printf( "     -descriptionlength n - the number of characters to put in tx description (n * the character x).  Default is 10.\n" );
    printf( "     -once - do not loop - do only one transaction.\n" );
    printf( "     -outcomerelease - registers for transaction outcome and releases the ITransaction inline with the outcome notification.\n");
    printf( "     -xasuperior - do xa superior.\n");
    printf( "     -xaspc - try to do single phase commit thru XA interfaces.\n");
    printf( "     -supportspc - enlistments support single phase commit.\n");
    printf( "     -userandomrmid - generate a new unique GUID for the resource manager identifiers.\n" );

	return 0;
}

