// DtcEtwTraceMessagesFromGatewayUsingTip.cpp : Defines the entry point for the console application.
//

#define WIN32_LEAN_AND_MEAN
#include "stdafx.h"
#include <stdio.h>
#include <tchar.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "xolehlp.h"
#include "dtchelp.h"
#include "transact.h"
#include "txcoord.h"
#include "txdtc.h"

class VoterNotify : public ITransactionVoterNotifyAsync2
{
public:

        virtual HRESULT __stdcall Committed( 
            /* [in] */ BOOL fRetaining,
            /* [unique][in] */ XACTUOW *pNewUOW,
            /* [in] */ HRESULT hr)
		{
			wprintf(L"VoterNotify Committed: %X\n", hr);
			return S_OK;
		}
        
        virtual HRESULT __stdcall Aborted( 
            /* [unique][in] */ BOID *pboidReason,
            /* [in] */ BOOL fRetaining,
            /* [unique][in] */ XACTUOW *pNewUOW,
            /* [in] */ HRESULT hr)
		{
			wprintf(L"VoterNotify Aborted: %X\n", hr);
			return S_OK;
		}
        
        virtual HRESULT __stdcall HeuristicDecision( 
            /* [in] */ DWORD dwDecision,
            /* [unique][in] */ BOID *pboidReason,
            /* [in] */ HRESULT hr)
		{
			wprintf(L"VoterNotify Heuristic: %X\n", hr);
			return S_OK;
		}
        
        virtual HRESULT __stdcall Indoubt( void)
		{
			wprintf(L"VoterNotify InDoubt: \n");
			return S_OK;
		}

	virtual 	HRESULT	__stdcall	QueryInterface (REFIID i_iid,
												 	LPVOID FAR* o_ppv)
		{
			return S_OK;
		}

	//@cmember .
	virtual  ULONG	__stdcall AddRef (void)
		{
			return S_OK;
		}

	//@cmember .
	virtual  ULONG	__stdcall Release (void)
				{
			return S_OK;
		}


	HRESULT __stdcall VoteRequest(void)
	{
		wprintf(L"VoterNotify VoteRequest\n");
		return S_OK;
	}
};


int testDtc_Voter2()
{
	DTC_GET_TRANSACTION_MANAGER_EX	pfnDtcGetTmEx = NULL;
	HMODULE hXolehlp = NULL;
	ITransactionDispenser * pTxDisp = NULL;
	HRESULT hr = S_OK;
	ITransaction * tx = NULL;
	ITransactionVoterFactory2 * pTxVoterFactory = NULL;
	VoterNotify voterNotify;
	ITransactionVoterBallotAsync2 * pVoterBallot = NULL;
	//ITipXATransaction * pTipTx = NULL;
	ITransactionTransmitterFactory * pTxTransFac = NULL;
	ITransactionTransmitter * pTxTrans = NULL;
	ULONG cbToken = 0;
	ULONG cbTokenUsed = 0;
	byte * marshalToken = NULL;
	ITransactionReceiverFactory * pTxRecvFac = NULL;
	ITransactionReceiver * pTxRecv = NULL;
	ITransaction * pUnMarshalTx = NULL;

	hXolehlp = LoadLibraryW(L"Xolehlp.dll");
	if(hXolehlp == NULL)
	{
		wprintf(L"LoadLibrary failed: %u\n", GetLastError());
		goto Cleanup;
	}

	pfnDtcGetTmEx = (DTC_GET_TRANSACTION_MANAGER_EX)GetProcAddress(hXolehlp, "DtcGetTransactionManagerEx");
	if (pfnDtcGetTmEx == NULL)
	{
		wprintf(L"GetProcAddress failed: %u\n", GetLastError());
		goto Cleanup;
	}

	hr = pfnDtcGetTmEx(NULL, NULL, IID_ITransactionDispenser, OLE_TM_FLAG_NONE, NULL, (void **)&pTxDisp);
	if(FAILED(hr))
	{
		wprintf(L"DtcGetTransactionManagerExW failed: %X\n", hr);
		goto Cleanup;
	}

	hr = pfnDtcGetTmEx(NULL, NULL, IID_ITransactionVoterFactory2, OLE_TM_FLAG_NONE, NULL, (void **)&pTxVoterFactory);
	if(FAILED(hr))
	{
		wprintf(L"DtcGetTransactionManagerExW failed: %X\n", hr);
		goto Cleanup;
	}

	hr = pTxDisp->BeginTransaction(NULL, ISOLATIONLEVEL_UNSPECIFIED, ISOFLAG_RETAIN_DONTCARE, NULL, &tx);
	if(FAILED(hr))
	{
		wprintf(L"BeginTransaction failed: %X\n", hr);
		goto Cleanup;
	}

	hr = pTxDisp->QueryInterface(IID_ITransactionTransmitterFactory, (void **)&pTxTransFac);
	if(FAILED(hr))
	{
		wprintf(L"QueryInterface failed: %X\n", hr);
		goto Cleanup;
	}

	hr = pTxTransFac->Create(&pTxTrans);
	if(FAILED(hr))
	{
		wprintf(L"ITransactionTransmitterFactory::Create failed: %X\n", hr);
		goto Cleanup;
	}

	hr = pTxVoterFactory->Create(tx, &voterNotify, &pVoterBallot);
	if(FAILED(hr))
	{
		wprintf(L"pTxVoterFactory->Create failed: %X\n", hr);
		goto Cleanup;
	}

	hr = pTxTrans->Set(tx);
	if(FAILED(hr))
	{
		wprintf(L"ITransactionTransmitter:Set failed: %X\n", hr);
		goto Cleanup;
	}

	hr = pTxTrans->GetPropagationTokenSize(&cbToken);
	if(FAILED(hr))
	{
		wprintf(L"ITransactionTransmitter:GetPropagationTokenSize failed: %X\n", hr);
		goto Cleanup;
	}

	marshalToken = new byte[cbToken];

	hr = pTxTrans->MarshalPropagationToken(cbToken, marshalToken, &cbTokenUsed);
	if(FAILED(hr))
	{
		wprintf(L"ITransactionTransmitter:MarshalPropagationTokenSize failed: %X\n", hr);
		goto Cleanup;
	}

	hr = pfnDtcGetTmEx(NULL, NULL, IID_ITransactionReceiverFactory, OLE_TM_FLAG_NONE, NULL, (void **)&pTxRecvFac);
	if(FAILED(hr))
	{
		wprintf(L"DtcGetTransactionManagerExW failed: %X\n", hr);
		goto Cleanup;
	}

	hr = pTxRecvFac->Create(&pTxRecv);
	if(FAILED(hr))
	{
		wprintf(L"ITransactionReceiverFactory::Create failed: %X\n", hr);
		goto Cleanup;
	}

	hr = pTxRecv->UnmarshalPropagationToken(cbToken, marshalToken, &pUnMarshalTx);
	if(FAILED(hr))
	{
		wprintf(L"pTxRecv->UnmarshalPropagationToken failed: %X\n", hr);
		goto Cleanup;
	}

	hr = tx->Commit(FALSE, XACTTC_ASYNC, 0);
	if(FAILED(hr))
	{
		wprintf(L"tx->Commit failed: %X\n", hr);
		goto Cleanup;
	}

	Sleep(500);

    hr = pUnMarshalTx->Abort(NULL, FALSE, FALSE);
	if(FAILED(hr))
	{
		wprintf(L"pUnMarshalTx->Abort failed: %X\n", hr);
		goto Cleanup;
	}
	
    hr = pVoterBallot->VoteRequestDone(S_OK , NULL);
	if(FAILED(hr))
	{
		wprintf(L"pVoterBallot->VoteRequestDone failed: %X\n", hr);
		goto Cleanup;
	}
		
	Sleep(3000);

Cleanup:

	if(pVoterBallot != NULL)
	{
		pVoterBallot->Release();
	}

	if(marshalToken != NULL)
	{
		delete[] marshalToken;
	}

	if(pUnMarshalTx != NULL)
	{
		pUnMarshalTx->Release();
	}

	if(tx != NULL)
	{
		tx->Release();
	}

	if(pTxRecv != NULL)
	{
		pTxRecv->Release();
	}

	if(pTxRecvFac != NULL)
	{
		pTxRecvFac->Release();
	}

	if(pTxVoterFactory != NULL)
	{
		pTxVoterFactory->Release();
	}

	if(pTxTrans != NULL)
	{
		pTxTrans->Release();
	}

	if(pTxTransFac != NULL)
	{
		pTxTransFac->Release();
	}

	if(pTxDisp != NULL)
	{
		pTxDisp->Release();
	}

	if(hXolehlp != NULL)
	{
		FreeLibrary(hXolehlp);
	}

	return 0;
}

