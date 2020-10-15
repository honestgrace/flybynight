#include "stdafx.h"

#include <iostream>

using namespace std;

HRESULT Test(void **pEnv, LPTSTR lpPathVariable, LPTSTR lpPathValue)
{
	UCHAR _dest[256];
	UCHAR _source[256] = "abcde";
	auto _length = _mbstrnlen((const char *)_source, 256);
	//_mbsncpy(_dest, _source, _length);
 	errno_t _err = _mbsncpy_s (_dest, _length, _source, _TRUNCATE);
	if (_err)
	{
		if (_err != STRUNCATE)
		{
			cout << "unexpected error";
		}
	}
	_dest[_length] = '\0';

	strcpy_s((char *) _dest, _length + 1, (char *)_source);

	DWORD           dwEventMsg = 0;
	ULONG           cbMessageBuffer = 0;
	ULONG           cbCursor = 0;
	char            *pucMessageBuffer = NULL;
	char            *pucUuidString = NULL;

	UCHAR pucLuPair[256] = "abcde";
	int cbLuPair = _mbstrnlen((const char *)pucLuPair, 256) + 1;
	UCHAR pucLuTransId[256] = "fghi";
	int cbLuTransId = _mbstrnlen((const char *)pucLuTransId, 256 + 1);

	// The message length is the length of the LU pair, the length of the transaction identifier, and two nulls.
	cbMessageBuffer = cbLuPair + cbLuTransId + 2;
	pucMessageBuffer = new char[cbMessageBuffer];

	// Copy strings into the message buffer.
	//_mbsncpy((UCHAR *)pucMessageBuffer, (UCHAR *)pucLuPair, cbLuPair);
	memcpy((void *)pucMessageBuffer, (void *)pucLuPair, cbLuPair);
	pucMessageBuffer[cbLuPair] = '\0';
	cbCursor = cbLuPair + 1;

	//_mbsncpy((UCHAR *)pucMessageBuffer + cbCursor, (UCHAR *)pucLuTransId, cbLuTransId);
	memcpy((void *)(pucMessageBuffer + cbCursor), (void *)pucLuTransId, cbLuTransId);
	UCHAR *ppucMessageBuffer = (UCHAR *)pucMessageBuffer;
	pucMessageBuffer[cbCursor + cbLuTransId] = '\0';
	cbCursor = cbCursor + cbLuTransId + 1;



	PBYTE testtt = (PBYTE) L"abcdef\0";
	DWORD cchttt = 0;
	//PBYTE g_outttt = (PBYTE)L"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
	BYTE outttt[100];

	TCHAR szSection[8] = L"A\0B\0C\0";
	TCHAR szSection2[7] = L"A\0B\0C\0";
	TCHAR *szParent;
	do
	{
		szParent = NULL;
		TCHAR *szKeyAndValue = szSection;
		while (szKeyAndValue < (szSection + ARRAYSIZE(szSection2)) &&  *szKeyAndValue)
		{
			TCHAR *szKey = szKeyAndValue;					// save the key
			szKeyAndValue += _tcslen(szKeyAndValue) + 1;	// advance KeyAndValue to the next pair

			if (szKeyAndValue >= szSection + ARRAYSIZE(szSection2))
			{
				cout << "error";
			}
		}

		

	} while (szParent);


	char ttt[PIFMAXFILEPATH + 35] = "c:\\temp";
	void * p = ttt;
	LPSTR pszPersistTemplateDirBuf = NULL;

	pszPersistTemplateDirBuf = static_cast<LPSTR>(p);

	CHAR szDirBuffer[PIFMAXFILEPATH + 35];
	auto arrszie = ARRAYSIZE(szDirBuffer);
	INT iLen = sprintf_s(szDirBuffer, PIFMAXFILEPATH, "%s\\PID*.TMP", pszPersistTemplateDirBuf);
	if ((iLen <= 0) || (iLen >= PIFMAXFILEPATH))
	{
		cout << "Memory overflow" << endl;
	}

	DWORD cchDirOffset = strlen(pszPersistTemplateDirBuf) + 1;
	DWORD cchDirSize = sizeof szDirBuffer - cchDirOffset;
	CHAR *pDir = szDirBuffer + cchDirOffset;
	errno_t err = strcpy_s(pDir, cchDirSize, "*");
	if (err)
	{
		cout << "Error";
	}

	int ccFileOffset = strnlen_s(pDir, cchDirSize);

	BSTR strUserName = L"test2";

	_ASSERT(wcslen(strUserName) == 4);

	LPCTSTR lpString;

	NTSTATUS Status;
	WCHAR lpTemp[1025] = { 0 };
	DWORD cb;
	size_t cch = 0;
	size_t cch2 = 0;
	HRESULT hr = S_OK;

	hr = E_INVALIDARG;

	hr = StringCchLength(strUserName,
		MAX_PATH,
		&cch);
	if (cch >= MAX_PATH)
	{
		hr = E_INVALIDARG;
	}

	auto x = sizeof(lpTemp);
	auto y = (sizeof(lpTemp) / sizeof(lpTemp[0]));
	auto z = (sizeof(lpPathValue) / sizeof(lpPathValue[0]));

	if (!*pEnv) {
		return(FALSE);
	}

	hr = StringCchLength(lpTemp,
		(sizeof(lpTemp) / sizeof(lpTemp[0])),
		&cch);

	if (FAILED(hr) || cch >= (sizeof(lpTemp) / sizeof(lpTemp[0])))
	{
		*lpTemp = 0;
		return(FALSE);
	}

	hr = StringCchLength(lpPathValue,
		(sizeof(lpTemp) / sizeof(lpTemp[0])),
		&cch2);

	if (FAILED(hr) || cch2 >= (sizeof(lpTemp) / sizeof(lpTemp[0])))
	{
		*lpTemp = 0;
		return(FALSE);
	}


Finished:

	return S_OK;
}



HRESULT TestUnicodeStringAPI()
{
	WCHAR lpTemp[1025] = L"𠀀𠀀a";

	Test((void **) "𠀀𠀀a", L"𠀀𠀀a", lpTemp);

	wchar_t struTest[6] = L"0x";
	wchar_t * pStr = struTest;
	
	auto struTestLen = ARRAYSIZE(struTest) - 2;
	_ultow_s(555, pStr + 2, struTestLen, 10);

	wcout << struTest << endl;
											
	STACK_STRA(straSize, 128);
	auto kkkkk = straSize.QuerySize();

	const WCHAR * rgStrings[7];
	auto ttt = ARRAYSIZE(rgStrings);
	WCHAR               achLowValue[7];
	errno_t err22 = 0;
	err22 = _ui64tow_s(555, achLowValue, 7, 10);

	auto ttt2 = E_UNEXPECTED;
	SetLastError(E_UNEXPECTED);
	auto ttt4 = GetLastError();
	auto hr = HRESULT_FROM_WIN32(GetLastError());
	SetLastError(ERROR_NOT_ENOUGH_MEMORY);
	hr = HRESULT_FROM_WIN32(GetLastError());
	SetLastError(HRESULT_FROM_WIN32(E_UNEXPECTED));
	hr = HRESULT_FROM_WIN32(GetLastError());

	auto ttt5 = sizeof(CHAR);

	achLowValue[0] = L'0';
	achLowValue[1] = L'\0';

	for (int i = 1; i <= 3; i++)
	{
		switch (i)
		{
		case 1: 
			continue;
			break;
		case 2:
			continue;
			break;
		case 3:
			break;
		}
		printf("hit");
	}
	
	hr = S_OK;
	DWORD                       dwError = ERROR_SUCCESS;

	ULONG flags = 4294967295;
	//ULONG flags = 0;
	DWORD cb = sizeof("ffffffff"); // ULONG
	PSTR szFlags = new char(cb);
	if (szFlags == NULL)
	{
		hr = ERROR_NOT_ENOUGH_MEMORY;
		return hr;
	}
	dwError = _ultoa_s(flags, szFlags, cb, 16);
	if (dwError != ERROR_SUCCESS)
	{
		hr = HRESULT_FROM_WIN32(dwError);
		return hr;
	}
	char *ppszValue = szFlags;
	DWORD pcchValueLength = (DWORD)strlen(szFlags);

	unsigned long long _cMax = 555;
	WCHAR  achMax[10];
	WCHAR * buffer;
	buffer = achMax;
	buffer[0] = L'0';
	buffer[1] = L'x';
	//buffer = NULL;

	auto size = sizeof(achMax) / sizeof(WCHAR);
	auto size2 = ARRAYSIZE(achMax);
	auto size3 = ARRAYSIZE(achMax) - 2;

	errno_t result = _ui64tow_s(_cMax, achMax,  ARRAYSIZE(achMax) - 1, 10);
	
	result = _ui64tow_s(_cMax, buffer, 10, 10);
	result = EINVAL;
	if (result)
	{
		hr = HRESULT_FROM_WIN32(result);
		hr = E_UNEXPECTED;
		hr = HRESULT_FROM_WIN32(E_UNEXPECTED);
		if (FAILED(hr))
		{
			printf("failed");
		}
	}

	wcout << achMax << endl;

/////////////////////////////////////////////

	
	char data1[] = "A~string\tof~,,tokens\nand~some~~more~tokens";
	char data2[] = "AAA~BBB~CCC";
	LPSTR string1 = data1;
	LPSTR string2 = data2;

    #define DELIMITER	"~"
	char seps[] = "~";
	CHAR *token1 = NULL;
	CHAR *token2 = NULL;
	char *next_token1 = NULL;
	char *next_token2 = NULL;

	printf("Tokens:\n");

	// Establish string and get the first token:
	token1 = strtok_s(string1, DELIMITER, &next_token1);
	token2 = strtok_s(string2, DELIMITER, &next_token2);

	printf("%s\r\n", token1);
	printf("%s\r\n", token2);
	while (token1 != NULL || token2 != NULL)
	{
		if (token1 != NULL)
		{
			token1 = strtok_s(NULL, DELIMITER, &next_token1);
			if (token1 != NULL && next_token1 != NULL)
			{
				printf("%s\r\n", token1);
			}
		}
		if (token2 != NULL)
		{
			token2 = strtok_s(NULL, DELIMITER, &next_token2);
			if (token2 != NULL && next_token2 != NULL)
			{
				printf("%s\r\n", token2);
			}
		}
	}
	
	return S_OK;
}