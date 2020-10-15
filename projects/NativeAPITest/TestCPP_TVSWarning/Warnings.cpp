#include "stdafx.h"
#include <windows.h>
/**********************************************************************/
/**                Copyright(c) Microsoft Corp., 2007                **/
/**********************************************************************/

/*
stringu.h

class STRU:

A light-weight string class, useful for creating Win32 wide API args

String encoding varies depending on how STRU is populated, thinks in WCHAR
arrays

FILE HISTORY:
Johnl       15-Aug-1994 Created
MuraliK     09-Jul-1996 Rewrote for efficiency with no unicode support
MCourage    12-Feb-1999 Another rewrite. All unicode of course.
RickJ       17-Aug-2007 Reduced sprawl, more CCH/HRESULT consistency
*/

#ifndef _STRINGU_H_
#define _STRINGU_H_

// #include "D:\os\src\inetsrv\iis\admin\xpath\common\buffer.h"
#include "buffer.h"
#include <strsafe.h>

//
// BUGBUG: TODO:
// - pageheap support
// - CopyToBufferA
//

class STRU
{

public:

	template<size_t size>
	static
		HRESULT
		PathCchAddBackslashEx(
			__inout WCHAR(&pszPath)[size],
			__deref_out LPWSTR *    ppszEnd
		)
	{

		HRESULT hr;
		PWSTR   pszEnd;
		size_t  cch;
		size_t  cchRemaining;

		*ppszEnd = NULL;

		hr = StringCchLength(pszPath,
			size,
			&cch);
		if (FAILED(hr))
		{
			return hr;
		}

		if (cch >= size)
		{
			return  STRSAFE_E_INSUFFICIENT_BUFFER;
		}

		pszEnd = pszPath + cch;
		cchRemaining = size - cch;
		if (cch && (*(pszEnd - 1) != L'\\'))
		{
			hr = StringCchCopyExW(pszEnd, cchRemaining, L"\\", &pszEnd, &cchRemaining, 0);
		}
		else
		{
			// The string is either empty or already ends in a backslash.
			// Treat as success. Don't add backslash to empty strings.
			hr = S_FALSE;
		}

		if (SUCCEEDED(hr))
		{
			*ppszEnd = pszEnd;
		}

		return hr;
	}

	STRU(
		VOID
	) : m_cchLen(0)
	{
		*(QueryStr()) = L'\0';
	}

	STRU(
		__in_ecount(cchInit) WCHAR* pbInit,
		DWORD cchInit
	) : m_Buff((PBYTE)pbInit, cchInit * sizeof(WCHAR)),
		m_cchLen(0)
		/*++
		Description:

		Used by STACK_STRU. Initially populates underlying buffer with pbInit.

		pbInit is not freed.

		Arguments:

		pbInit - initial memory to use
		cchInit - count, in characters, of pbInit

		Returns:

		None.

		--*/
	{
		_ASSERTE(cchInit <= (MAXDWORD / sizeof(WCHAR)));
		_ASSERTE(NULL != pbInit);
		_ASSERTE(cchInit > 0);
		*pbInit = L'\0';
	}

	BOOL
		IsEmpty(
			VOID
		) const
	{
		return (m_cchLen == 0);
	}

	BOOL
		Equals(
			__in    PCWSTR  pszRhs,
			BOOL    fIgnoreCase = FALSE
		) const
	{
		if (CSTR_EQUAL == CompareStringOrdinal(
			QueryStr(),
			QueryCCH(),
			pszRhs,
			-1,             // -1 means pszRhs is NULL terminated
			fIgnoreCase
		)
			)
		{
			return TRUE;
		}

		return FALSE;
	}

	BOOL
		Equals(
			__in const STRU *   pstrRhs,
			BOOL                fIgnoreCase = FALSE
		) const
	{
		_ASSERTE(pstrRhs != NULL);
		if (CSTR_EQUAL == CompareStringOrdinal(
			QueryStr(),
			QueryCCH(),
			pstrRhs->QueryStr(),
			pstrRhs->QueryCCH(),
			fIgnoreCase
		)
			)
		{
			return TRUE;
		}
		return FALSE;
	}

	BOOL
		Equals(
			__in const STRU &   strRhs,
			BOOL                fIgnoreCase = FALSE
		) const
	{
		if (CSTR_EQUAL == CompareStringOrdinal(
			QueryStr(),
			QueryCCH(),
			strRhs.QueryStr(),
			strRhs.QueryCCH(),
			fIgnoreCase
		)
			)
		{
			return TRUE;
		}
		return FALSE;
	}

	DWORD
		QueryCB(
			VOID
		) const
		//
		// Returns the number of bytes in the string excluding the terminating NULL
		//
	{
		return m_cchLen * sizeof(WCHAR);
	}

	DWORD
		QueryCCH(
			VOID
		) const
		//
		//  Returns the number of characters in the string excluding the terminating NULL
		//
	{
		return m_cchLen;
	}

	DWORD
		QuerySizeCCH(
			VOID
		) const
		//
		// Returns size of the underlying storage buffer, in characters
		//
	{
		return m_Buff.QuerySize() / sizeof(WCHAR);
	}

	WCHAR*
		QueryStr(
			VOID
		) const
		//
		//  Return the string buffer
		//
	{
		return static_cast<WCHAR*>(m_Buff.QueryPtr());
	}

	VOID
		Reset(
			VOID
		)
		//
		// Resets the internal string to be NULL string. Buffer remains cached.
		//
	{
		_ASSERTE(QueryStr() != NULL);
		*(QueryStr()) = L'\0';
		m_cchLen = 0;
	}

	HRESULT
		Resize(
			DWORD cchSize
		)
	{
		SIZE_T cbSize = cchSize * sizeof(WCHAR);
		if (cbSize > MAXDWORD)
		{
			return HRESULT_FROM_WIN32(ERROR_ARITHMETIC_OVERFLOW);
		}
		if (!m_Buff.Resize(static_cast<DWORD>(cbSize)))
		{
			return E_OUTOFMEMORY;
		}

		return S_OK;
	}

	VOID
		SyncWithBuffer(
			VOID
		)
		//
		// Recalculate the length of the string, etc. because we've modified
		// the buffer directly.
		//
	{
		//
		// TODO: Should we change the signature to support HRESULTs
		// or just blindly trust in the recent buffer changes?
		//
		m_cchLen = static_cast<DWORD>(wcslen(QueryStr()));
	}

	template<size_t size>
	HRESULT
		Copy(
			PCWSTR const (&rgpszStrings)[size]
		)
	{
		Reset();
		printf("%d", size);
		auto a = _countof(rgpszStrings);
		printf("%d", a);
		return AuxAppend(rgpszStrings, _countof(rgpszStrings));
	}

	template<size_t size>
	HRESULT
		Copy2(
			PCWSTR const (&rgpszStrings)[size],
			SIZE_T    cNumStrings
		)
	{
		Reset();
		printf("%d", size);
		auto a = _countof(rgpszStrings);
		printf("%d", a);
		return AuxAppend(rgpszStrings, cNumStrings);
		//return AuxAppend( rgpszStrings, size );
		//return AuxAppend( rgpszStrings, _countof( rgpszStrings ) );
	}

	HRESULT
		Copy(
			PCWSTR  pszCopy,
			DWORD   cchLen = 0,
			const DWORD cchMaxLen = STRSAFE_MAX_CCH
		)
		//
		// Copy the contents of another string to this one
		//
	{
		HRESULT hr;
		size_t  cbStr;

		hr = GetStringCbLengthW(pszCopy,
			cchLen,
			cchMaxLen,
			&cbStr);
		if (FAILED(hr))
		{
			return hr;
		}

		_ASSERTE(cbStr <= MAXDWORD);

		return AuxAppend(
			pszCopy,
			static_cast<DWORD>(cbStr),
			0
		);
	}

	HRESULT
		Copy(
			__in const STRU * pstrRhs
		)
	{
		_ASSERTE(NULL != pstrRhs);
		return Copy(pstrRhs->QueryStr(), pstrRhs->QueryCCH());
	}

	HRESULT
		Copy(
			__in const STRU & str
		)
	{
		return Copy(str.QueryStr(), str.QueryCCH());
	}

	HRESULT
		CopyAndExpandEnvironmentStrings(
			__in PCWSTR             pszSource
		)
	{
		HRESULT                     hr = S_OK;
		DWORD                       cchDestReqBuff = 0;

		Reset();

		cchDestReqBuff = ExpandEnvironmentStringsW(pszSource,
			QueryStr(),
			QuerySizeCCH());
		if (cchDestReqBuff == 0)
		{
			hr = HRESULT_FROM_WIN32(GetLastError());
			goto Finished;
		}
		else if (cchDestReqBuff > QuerySizeCCH())
		{
			hr = Resize(cchDestReqBuff);
			if (FAILED(hr))
			{
				goto Finished;
			}

			cchDestReqBuff = ExpandEnvironmentStringsW(pszSource,
				QueryStr(),
				QuerySizeCCH());

			if (cchDestReqBuff == 0 || cchDestReqBuff > QuerySizeCCH())
			{
				_ASSERTE(FALSE);
				hr = HRESULT_FROM_WIN32(GetLastError());
				goto Finished;
			}
		}

		SyncWithBuffer();

	Finished:

		return hr;

	}

	HRESULT
		CopyA(
			PCSTR   pszCopyA,
			//
			// BUGBUG: were there ever APIs depending on size-zero, non-NULL terminated copies?
			//
			DWORD   cchLen = 0,
			UINT    CodePage = CP_UTF8,
			const DWORD cchMaxLen = STRSAFE_MAX_CCH
		)
	{
		_ASSERTE(cchMaxLen <= STRSAFE_MAX_CCH);

		HRESULT hr;
		size_t  cbStr;

		hr = GetStringCbLengthA(pszCopyA,
			cchLen,
			cchMaxLen,
			&cbStr);
		if (FAILED(hr))
		{
			return hr;
		}

		_ASSERTE(cbStr <= MAXDWORD);

		return AuxAppendA(
			pszCopyA,
			static_cast<DWORD>(cbStr),
			0,
			CodePage
		);
	}

	HRESULT
		Append(
			PCWSTR  pszAppend,
			DWORD   cchLen = 0,
			const DWORD cchMaxLen = STRSAFE_MAX_CCH
		)
		//
		// Append something to the end of the string
		//
	{
		_ASSERTE(NULL != pszAppend);

		HRESULT hr;
		size_t  cbStr;

		hr = GetStringCbLengthW(pszAppend,
			cchLen,
			cchMaxLen,
			&cbStr);
		if (FAILED(hr))
		{
			return hr;
		}

		_ASSERTE(cbStr <= MAXDWORD);

		return AuxAppend(
			pszAppend,
			static_cast<DWORD>(cbStr),
			QueryCB()
		);
	}

	HRESULT
		Append(
			__in const STRU * pstrRhs
		)
	{
		_ASSERTE(NULL != pstrRhs);
		return Append(pstrRhs->QueryStr(), pstrRhs->QueryCCH());
	}

	HRESULT
		Append(
			__in const STRU & strRhs
		)
	{
		return Append(strRhs.QueryStr(), strRhs.QueryCCH());
	}

	template<size_t size>
	HRESULT
		Append(
			PCWSTR const (&rgpszStrings)[size]
		)
	{
		return AuxAppend(rgpszStrings, _countof(rgpszStrings));
	}

	template<size_t size>
	HRESULT
		Append2(
			PCWSTR const (&rgpszStrings)[size],
			SIZE_T    cNumStrings
		)
	{
		return AuxAppend(rgpszStrings, cNumStrings);
		//return AuxAppend( rgpszStrings, size );
		//return AuxAppend( rgpszStrings, _countof( rgpszStrings ) );
	}

	HRESULT
		AppendA(
			PCSTR   pszAppendA,
			DWORD   cchLen = 0,
			UINT    CodePage = CP_UTF8,
			const DWORD cchMaxLen = STRSAFE_MAX_CCH
		)
	{
		HRESULT hr;
		size_t  cbStr;

		hr = GetStringCbLengthA(pszAppendA,
			cchLen,
			cchMaxLen,
			&cbStr);
		if (FAILED(hr))
		{
			return hr;
		}

		_ASSERTE(cbStr <= MAXDWORD);

		return AuxAppendA(
			pszAppendA,
			static_cast<DWORD>(cbStr),
			QueryCB(),
			CodePage
		);
	}

	HRESULT
		CopyToBuffer(
			__out_bcount(*pcb) WCHAR*   pszBuffer,
			PDWORD                      pcb
		) const
		//
		// Makes a copy of the stored string into the given buffer
		//
	{
		_ASSERTE(NULL != pszBuffer);
		_ASSERTE(NULL != pcb);

		HRESULT hr = S_OK;
		DWORD   cbNeeded = QueryCB() + sizeof(WCHAR);

		if (*pcb < cbNeeded)
		{
			hr = HRESULT_FROM_WIN32(ERROR_INSUFFICIENT_BUFFER);
			goto Finished;
		}

		//
		// BUGBUG: StringCchCopy?
		//
		memcpy(pszBuffer, QueryStr(), cbNeeded);

	Finished:

		*pcb = cbNeeded;

		return hr;
	}

	HRESULT
		SetLen(
			DWORD cchLen
		)
		/*++
		*
		Routine Description:

		Set the length of the string and null terminate, if there
		is sufficient buffer already allocated. Will not reallocate.

		Arguments:

		cchLen - The number of characters in the new string.

		Return Value:

		HRESULT

		--*/
	{
		if (cchLen >= QuerySizeCCH())
		{
			return HRESULT_FROM_WIN32(ERROR_INVALID_PARAMETER);
		}

		*(QueryStr() + cchLen) = L'\0';
		m_cchLen = cchLen;

		return S_OK;
	}

	HRESULT
		SafeSnwprintf(
			__in PCWSTR pwszFormatString,
			...
		)
		/*++

		Routine Description:

		Writes to a STRU, growing it as needed. It arbitrarily caps growth at 64k chars.

		Arguments:

		pwszFormatString    - printf format
		...                 - printf args

		Return Value:

		HRESULT

		--*/
	{
		HRESULT     hr = S_OK;
		va_list     argsList;
		va_start(argsList, pwszFormatString);

		hr = SafeVsnwprintf(pwszFormatString, argsList);

		va_end(argsList);
		return hr;
	}

	HRESULT SafeVsnwprintf(
		__in PCWSTR pwszFormatString,
		va_list     argsList
	)
		/*++

		Routine Description:

		Writes to a STRU, growing it as needed. It arbitrarily caps growth at 64k chars.

		Arguments:

		pwszFormatString    - printf format
		argsList            - printf va_list

		Return Value:

		HRESULT

		--*/
	{
		HRESULT     hr = S_OK;
		int         cchOutput;
		int         cchNeeded;

		//
		// Format the incoming message using vsnprintf()
		// so that the overflows are captured
		//
		cchOutput = _vsnwprintf_s(
			QueryStr(),
			QuerySizeCCH(),
			QuerySizeCCH() - 1,
			pwszFormatString,
			argsList
		);

		if (cchOutput == -1)
		{
			//
			// Couldn't fit this in the original STRU size.
			//
			cchNeeded = _vscwprintf(pwszFormatString, argsList);
			if (cchNeeded > 64 * 1024)
			{
				//
				// If we're trying to produce a string > 64k chars, then
				// there is probably a problem
				//
				hr = HRESULT_FROM_WIN32(ERROR_INVALID_DATA);
				goto Finished;
			}

			//
			// _vscprintf doesn't include terminating null character
			//
			cchNeeded++;

			hr = Resize(cchNeeded);
			if (FAILED(hr))
			{
				goto Finished;
			}

			cchOutput = _vsnwprintf_s(
				QueryStr(),
				QuerySizeCCH(),
				QuerySizeCCH() - 1,
				pwszFormatString,
				argsList
			);
			if (-1 == cchOutput)
			{
				//
				// This should never happen, cause we should already have correctly sized memory
				//
				_ASSERTE(FALSE);

				hr = HRESULT_FROM_WIN32(ERROR_INVALID_DATA);
				goto Finished;
			}
		}

		//
		// always null terminate at the last WCHAR
		//
		QueryStr()[QuerySizeCCH() - 1] = L'\0';

		//
		// we directly touched the buffer - therefore:
		//
		SyncWithBuffer();

	Finished:

		if (FAILED(hr))
		{
			Reset();
		}

		return hr;
	}

private:

	//
	// Avoid C++ errors. This object should never go through a copy
	// constructor, unintended cast or assignment.
	//
	STRU(const STRU &);
	STRU & operator = (const STRU &);

	HRESULT
		AuxAppend(
			PCWSTR const    rgpszStrings[],
			SIZE_T          cNumStrings
		)
		/*++

		Routine Description:

		Appends an array of strings of length cNumStrings

		Arguments:

		rgStrings   - The array of strings to be appened
		cNumStrings - The count of String

		Return Value:

		HRESULT

		--*/
	{
		HRESULT         hr = S_OK;
		size_t          cbStringsTotal = sizeof(WCHAR);   // Account for null-terminator

														  /*
														  Compute total size of the string.
														  Resize internal buffer
														  Copy each array element one by one to backing buffer
														  Update backing buffer string length
														  */

		for (SIZE_T i = 0; i < cNumStrings; i++)
		{
			_ASSERTE(rgpszStrings[i] != NULL);

			size_t      cbString = 0;

			hr = StringCbLengthW(rgpszStrings[i],
				STRSAFE_MAX_CCH * sizeof(WCHAR),
				&cbString);
			if (FAILED(hr))
			{
				return hr;
			}

			cbStringsTotal += cbString;

			if (cbStringsTotal > MAXDWORD)
			{
				return HRESULT_FROM_WIN32(ERROR_ARITHMETIC_OVERFLOW);
			}
		}

		ULONGLONG cbBufSizeRequired = (ULONGLONG)QueryCCH() * sizeof(WCHAR) + cbStringsTotal;
		if (cbBufSizeRequired > MAXDWORD)
		{
			return HRESULT_FROM_WIN32(ERROR_ARITHMETIC_OVERFLOW);
		}

		if (m_Buff.QuerySize() < cbBufSizeRequired)
		{
			if (!m_Buff.Resize(static_cast< DWORD >(cbBufSizeRequired)))
			{
				return E_OUTOFMEMORY;
			}
		}

		STRSAFE_LPWSTR pszStringEnd = QueryStr() + QueryCCH();
		size_t cchRemaining = QuerySizeCCH() - QueryCCH();
		for (SIZE_T i = 0; i < cNumStrings; i++)
		{
			hr = StringCchCopyExW(pszStringEnd,        //  pszDest
				cchRemaining,        //  cchDest
				rgpszStrings[i],   //  pszSrc
				&pszStringEnd,       //  ppszDestEnd
				&cchRemaining,       //  pcchRemaining
				0);                 //  dwFlags
			if (FAILED(hr))
			{
				_ASSERTE(FALSE);
				SyncWithBuffer();
				return hr;
			}
		}

		m_cchLen = static_cast< DWORD >(cbBufSizeRequired) / sizeof(WCHAR) - 1;

		return S_OK;
	}

	HRESULT
		AuxAppend(
			const WCHAR*    pStr,
			DWORD           cbStr,
			DWORD           cbOffset
		)
		/*++

		Routine Description:

		Appends to the string starting at the (byte) offset cbOffset.

		Arguments:

		pStr     - A unicode string to be appended
		cbStr    - Length, in bytes, of pStr
		cbOffset - Offset, in bytes, at which to begin the append

		Return Value:

		HRESULT

		--*/
	{
		_ASSERTE(NULL != pStr);
		_ASSERTE(0 == cbStr % sizeof(WCHAR));
		_ASSERTE(cbOffset <= QueryCB());
		_ASSERTE(0 == cbOffset % sizeof(WCHAR));

		ULONGLONG cb64NewSize = (ULONGLONG)cbOffset + cbStr + sizeof(WCHAR);
		if (cb64NewSize > MAXDWORD)
		{
			return HRESULT_FROM_WIN32(ERROR_ARITHMETIC_OVERFLOW);
		}

		if (m_Buff.QuerySize() < cb64NewSize)
		{
			if (!m_Buff.Resize(static_cast<DWORD>(cb64NewSize)))
			{
				return E_OUTOFMEMORY;
			}
		}

		memcpy((BYTE*)m_Buff.QueryPtr() + cbOffset, pStr, cbStr);

		m_cchLen = (cbStr + cbOffset) / sizeof(WCHAR);

		*(QueryStr() + m_cchLen) = L'\0';

		return S_OK;
	}

	HRESULT
		AuxAppendA(
			const CHAR*     pStr,
			DWORD           cbStr,
			DWORD           cbOffset,
			UINT            CodePage
		)
		/*++

		Routine Description:

		Convert and append an ANSI string to the string starting at
		the (byte) offset cbOffset

		Arguments:

		pStr     - An ANSI string to be appended
		cbStr    - Length, in bytes, of pStr
		cbOffset - Offset, in bytes, at which to begin the append
		CodePage - code page to use for conversion

		Return Value:

		HRESULT

		--*/
	{
		WCHAR*  pszBuffer;
		DWORD   cchBuffer;
		DWORD   cchCharsCopied = 0;

		_ASSERTE(NULL != pStr);
		_ASSERTE(cbOffset <= QueryCB());
		_ASSERTE(0 == cbOffset % sizeof(WCHAR));

		if (0 == cbStr)
		{
			return S_OK;
		}

		//
		//  Only resize when we have to.  When we do resize, we tack on
		//  some extra space to avoid extra reallocations.
		//
		if (m_Buff.QuerySize() < (ULONGLONG)cbOffset + ((ULONGLONG)cbStr * sizeof(WCHAR)) + sizeof(WCHAR))
		{
			ULONGLONG cb64NewSize = (ULONGLONG)cbOffset + (ULONGLONG)cbStr * sizeof(WCHAR) + sizeof(WCHAR);

			//
			// Check for the arithmetic overflow
			//
			if (cb64NewSize > MAXDWORD)
			{
				return HRESULT_FROM_WIN32(ERROR_ARITHMETIC_OVERFLOW);
			}

			if (!m_Buff.Resize(static_cast<DWORD>(cb64NewSize)))
			{
				return E_OUTOFMEMORY;
			}
		}

		pszBuffer = (WCHAR*)((BYTE*)m_Buff.QueryPtr() + cbOffset);
		cchBuffer = (m_Buff.QuerySize() - cbOffset - sizeof(WCHAR)) / sizeof(WCHAR);

		cchCharsCopied = MultiByteToWideChar(
			CodePage,
			MB_ERR_INVALID_CHARS,
			pStr,
			static_cast<int>(cbStr),
			pszBuffer,
			cchBuffer
		);
		if (0 == cchCharsCopied)
		{
			return HRESULT_FROM_WIN32(GetLastError());
		}

		//
		// set the new length
		//
		m_cchLen = cchCharsCopied + cbOffset / sizeof(WCHAR);

		//
		// Must be less than, cause still need to add NULL
		//
		_ASSERTE(m_cchLen < QuerySizeCCH());

		//
		// append NULL character
		//
		*(QueryStr() + m_cchLen) = L'\0';

		return S_OK;
	}

	static
		HRESULT
		GetStringCbLengthW(
			PCWSTR psz,
			DWORD cchLen,
			const DWORD cchMaxLen,
			size_t* pcbStr
		)
	{
		_ASSERTE(cchMaxLen <= STRSAFE_MAX_CCH);
		if (cchLen != 0)
		{
			*pcbStr = cchLen * sizeof(WCHAR);
			if (*pcbStr > MAXDWORD)
			{
				return HRESULT_FROM_WIN32(ERROR_ARITHMETIC_OVERFLOW);
			}
		}
		else
		{
			return StringCbLengthW(psz,
				cchMaxLen * sizeof(WCHAR),
				pcbStr);
		}
		return S_OK;
	}

	static
		HRESULT
		GetStringCbLengthA(
			PCSTR psz,
			DWORD cchLen,
			const DWORD cchMaxLen,
			size_t* pcbStr
		)
	{
		_ASSERTE(cchMaxLen <= STRSAFE_MAX_CCH);
		if (cchLen != 0)
		{
			*pcbStr = cchLen;
		}
		else
		{
			return StringCbLengthA(psz,
				cchMaxLen,
				pcbStr);
		}
		return S_OK;
	}

	BUFFER  m_Buff;
	DWORD   m_cchLen;
};

//
// Heap operation reduction macros
//
#define STACK_STRU( name, size )  WCHAR __ach##name[size]; \
                                  STRU name( __ach##name, sizeof( __ach##name ) / sizeof( *__ach##name ) )

#define INLINE_STRU(name, size)  WCHAR  __ach##name[size]; \
                                 STRU  name;

#define INLINE_STRU_INIT(name) name( __ach##name, sizeof( __ach##name ) / sizeof( *__ach##name ) )

#endif

void WarningTest()
{
	
	STRU test;
	STRU test2;
	PCWSTR const data[] =
	{
		L"/foo",
		L"/bar"
	};
	const size_t arraySize = sizeof(data) / sizeof(*data);
	auto hr = test.Copy<arraySize>(data);
	hr = test.Append<arraySize>(data);
	hr = test2.Copy2(data, _countof(data));
	hr = test2.Append2(data, _countof(data));

	PCWSTR const data2[] =
	{
		L"/zzz"
	};

	const size_t arraySize2 = sizeof(data2) / sizeof(*data2);
	hr = test.Copy<arraySize2>(data2);
	hr = test.Append<arraySize2>(data2);
	hr = test2.Copy2(data2, _countof(data2));
	hr = test2.Append2(data2, _countof(data2));

	TCHAR   szTemp[MAX_PATH] = {L"c:\\foo\\bar"};
	PWSTR   pEnd = NULL;
	STRU::PathCchAddBackslashEx(szTemp, &pEnd);
}

