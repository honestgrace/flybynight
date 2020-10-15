#include "stdafx.h"
#include <iostream>

using namespace std;

#define TCHAR CHAR

typedef PROPID QUEUEPROPID;
typedef struct tagPROPVARIANT tagMQPROPVARIANT;
typedef tagMQPROPVARIANT MQPROPVARIANT;
typedef struct tagMQQUEUEPROPS
{
    DWORD           cProp;
    __field_ecount(cProp) QUEUEPROPID*    aPropID;
    __field_ecount(cProp) MQPROPVARIANT*  aPropVar;
    __field_ecount_opt(cProp) HRESULT*        aStatus;
} MQQUEUEPROPS;

static
void
RemovePropWarnings(
    __in  MQQUEUEPROPS*  pqp,
    __in_ecount(pqp->cProp)  HRESULT*       aStatus,
    __out MQQUEUEPROPS** ppGoodProps,
    __out char**         ppTmpBuff)
{
    DWORD i;
    DWORD cGoodProps;
    char *pTmpBuff;
    MQQUEUEPROPS *pGoodProps;
    HRESULT *pStatus;
    QUEUEPROPID *pPropId;
    MQPROPVARIANT *pPropVar;

    // See how many good properties do we have.
    for (i = 0, cGoodProps = 0, pStatus = aStatus;
        i < pqp->cProp;
        i++, pStatus++)
    {
        if (*pStatus != S_OK)
        {
            return; // ASSERT(!FAILED(*pStatus));
        }
        else
        {
            cGoodProps++;
        }
    }

    // Allocate the temporary buffer, the buffer contains everything in it.
    // It contains the MQQUEUEPROPS structure, the QUEUEPROPID and
    // MQPROPVARIANT arrays.
    pTmpBuff = new char[sizeof(MQQUEUEPROPS) +
        cGoodProps * sizeof(QUEUEPROPID) +
        cGoodProps * sizeof(MQPROPVARIANT)];
    *ppTmpBuff = pTmpBuff;

    pGoodProps = (MQQUEUEPROPS*)pTmpBuff;
    *ppGoodProps = pGoodProps;

    //
    // Initialize the MQQUEUEPROPS structure.
    //
    // N.B. To avoid alignment fault the MQPROPVARIANT array is alocated before
    //      The QUEUEPROPID.
    //
    pGoodProps->cProp = cGoodProps;
    pGoodProps->aPropID = (QUEUEPROPID*)(pTmpBuff + sizeof(MQQUEUEPROPS) + cGoodProps * sizeof(MQPROPVARIANT));
    pGoodProps->aPropVar = (MQPROPVARIANT*)(pTmpBuff + sizeof(MQQUEUEPROPS));

    // Copy the array entries of the good properties to the arrays in the
    // temporary buffer.
    for (i = 0, cGoodProps = 0, pStatus = aStatus,
        pPropId = pqp->aPropID, pPropVar = pqp->aPropVar;
        i < pqp->cProp;
        i++, pStatus++, pPropId++, pPropVar++)
    {
        if (*pStatus == S_OK)
        {
            pGoodProps->aPropID[cGoodProps] = *pPropId;
            pGoodProps->aPropVar[cGoodProps] = *pPropVar;
            cGoodProps++;
        }
    }
}

HRESULT TestConvertToString()
{
    const int cProperties = 1;
    QUEUEPROPID   rgPropId[cProperties] = { 113 };
    MQPROPVARIANT rgPropVariant[cProperties];
    rgPropVariant[0].vt = VT_UI1;
    MQQUEUEPROPS  prop = { cProperties, rgPropId, rgPropVariant, NULL };

    MQQUEUEPROPS * pQMProps = &prop;

    MQQUEUEPROPS out_prop;
    MQQUEUEPROPS * p_out_prop = &out_prop;

    MQQUEUEPROPS** ppGoodQMP = &p_out_prop;
    HRESULT ttt = S_OK;

    char TmpBuff[256];
    char *pTmpQPBuff = TmpBuff;
    char **ppTmpBuff = &pTmpQPBuff;

    RemovePropWarnings(
        pQMProps,
        &ttt,
        ppGoodQMP,
        ppTmpBuff);

	ULONG                 Length = 0;
	ULONG *               pLength = &Length;

    BYTE pBuffer[256] = { 0, };

    LPWSTR  pServer = reinterpret_cast<LPWSTR>(pBuffer);


	WCHAR szBuf[1024];
	size_t size = ARRAYSIZE(szBuf) * sizeof(WCHAR);

	for (int i = 0; i < 3; i++)
	{
		if (i == 0)
		{
			ZeroMemory(szBuf, size);
			szBuf[0] = L'A';
			szBuf[1] = L'\0';
			szBuf[2] = L'A';
			szBuf[3] = L'A';


			wcout << szBuf << endl;
		}

		if (i == 1)
		{
			ZeroMemory(szBuf, size);
			szBuf[0] = L'B';
			szBuf[1] = L'B';
			szBuf[2] = L'\0';
			
			wcout << szBuf << endl;
		}

		if (i == 2)
		{
			ZeroMemory(szBuf, size);
			szBuf[0] = L'C';
			szBuf[1] = L'\0';
			
			wcout << szBuf << endl;
		}
	}

	#define DTC_PROXY_FILE_NAME            "MSDTCPRX.DLL"
    #define DTC_PROXY_FILE_NAME_W            L"MSDTCPRX.DLL"

	wchar_t test[11] = L"0123456789";

	HRESULT hr = S_OK;
	size_t cch = 0;
	hr = StringCchLength(test, ARRAYSIZE(test), &cch);
	//hr = StringCchLength(test, STRSAFE_MAX_CCH, &cch);

	std::cout << cch;
	std::cout << "Hello World!\n";


	LPSTR       szAppId = NULL;
	WCHAR       wszAppId[255];
	CHAR       wszAppId2[255];
	auto _ll = ARRAYSIZE(wszAppId);
	auto _ll2 = ARRAYSIZE(wszAppId2);
	
	auto _lll = sizeof DTC_PROXY_FILE_NAME;
	auto _lll2 = ARRAYSIZE(DTC_PROXY_FILE_NAME);

	_lll = sizeof DTC_PROXY_FILE_NAME_W;
	_lll2 = ARRAYSIZE(DTC_PROXY_FILE_NAME_W);

	auto _lll3 = sizeof(TCHAR);
	auto _result = (DWORD)(lstrlenA(DTC_PROXY_FILE_NAME) + 1) * sizeof(TCHAR);

	size_t _length = 0;
	
	hr = StringCchLengthA(DTC_PROXY_FILE_NAME,
		ARRAYSIZE(DTC_PROXY_FILE_NAME),
		&cch);

	if (SUCCEEDED(hr))
	{
		cout << "Successful";
	}
	size_t      cch_w = 0;
	HRESULT hr_w = StringCchLength(DTC_PROXY_FILE_NAME_W,
		ARRAYSIZE(DTC_PROXY_FILE_NAME_W),
		&cch_w);

	auto _result2 = (DWORD)(cch + 1) * sizeof(TCHAR);


	WCHAR _wszHostName[256] = L"localhost";
	TCHAR _szUuid[256] = "uuid";

	const WCHAR * wszHostName = _wszHostName;
	TCHAR * szUuid = _szUuid;

	int     sLength1, sLength2;       //Length of the szHostName and szUuid

	sLength1 = lstrlenW(wszHostName);
	sLength2 = lstrlenA(szUuid);

	size_t     sLength1_t, sLength2_t;       //Length of the szHostName and szUuid

	hr = StringCchLength(wszHostName,
		MAX_COMPUTERNAME_LENGTH,
		&sLength1_t);


	return S_OK;
}