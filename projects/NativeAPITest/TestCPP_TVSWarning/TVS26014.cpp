#include "stdafx.h"
#include <windows.h>
#include <TCHAR.h>
#include <iostream>

#define Assert _ASSERT
#include <malloc.h>
#include <mbstring.h>
#include <mbctype.h>
using namespace std;

inline BOOL IsSurrogateHigh(WORD ch) {
	return (ch >= 0xd800 && ch <= 0xdbff);
}

inline BOOL IsSurrogateLow(WORD ch) {
	return (ch >= 0xdc00 && ch <= 0xdfff);
}

/*============================================================================
AspCharNextA

UTF-8 aware CharNext()
============================================================================*/

char *AspCharNextA(WORD wCodePage, const char *sz)
{
	if (wCodePage != CP_UTF8)
		return CharNextExA(wCodePage, sz, 0);
	else
	{
		// CharNextExA won't work correctly in UTF-8.

		// Add support for UTF-8 encoding for Surrogate pairs
		// 110110wwwwzzzzyyyyyyxxxxxx gets encoded as 11110uuu 10uuzzzz 10yyyyyy 10xxxxxx
		// where uuuuu = wwww + 1 (to account for addition of 10000(b16) )
		// For further information refer : Page A-7 of "The Unicode Standard 2.0" ISBN-0-201-48345-9
		if ((*sz & 0xf8) == 0xF0)
			return const_cast<char *>(sz + 4);

		//zzzzyyyyyyxxxxxx = 1110zzzz 10yyyyyy 10xxxxxx
		if ((*sz & 0xF0) == 0xE0)
			return const_cast<char *>(sz + 3);

		//00000yyyyyxxxxxx = 110yyyyy 10xxxxxx
		else if ((*sz & 0xE0) == 0xC0)
			return const_cast<char *>(sz + 2);

		//000000000xxxxxxx = 0xxxxxxx
		else
			return const_cast<char *>(sz + 1);
	}
}

char *HTMLEncode(__out_ecount(cchDest) LPSTR szDest, SIZE_T cchDest, LPCSTR szSrc, UINT uCodePage, BSTR bstrIn);

void TVS26014()
{
	const size_t MAXSIZE = 3;
	char*	        pszstrIn = "foo";
	char	        pszEncodedstr[MAXSIZE];
	char*	        pszStartEncodestr = NULL;
	char*	        output = NULL;
	int		        nbstrLen = 0;
	int		        nstrLen = MAXSIZE;
	HRESULT	        hr = S_OK;
	UINT 	        uCodePage = CP_UTF8;
	for (int i = 0; i < MAXSIZE; i++)
		pszEncodedstr[i] = -1;

	output = HTMLEncode(pszEncodedstr, nstrLen, pszstrIn, uCodePage, nullptr);
}

/*===================================================================
HTMLEncode

HTML Encode a string containing the following characters

less than           <       &lt;
greater than        >       &gt;
ampersand           &       &amp;
quote               "       &quot;
any Ascii           ?       &#xxx   (where xxx is the ascii char val)

Parameters:
szDest - Pointer to the buffer to store the HTMLEncoded string
szSrc  - Pointer to the source buffer

and '"'.
uCodePage - system code page

Returns:
A pointer to the NUL terminated string.
===================================================================*/
char *HTMLEncode(__out_ecount(cchDest) LPSTR szDest, SIZE_T cchDest, LPCSTR szSrc, UINT uCodePage, BSTR bstrIn)
{
	char *  pszDest = szDest;
	UINT    i = 0;

	if (!szDest)
	{
		return NULL;
	}

	if (!szSrc)
	{
		*szDest = '\0';
		return pszDest;
	}

	Assert(cchDest > 0);

	char * szEnd = szDest + cchDest - 1;
	*szDest = '\0';

	while (*szSrc)
	{
		//
		// The original condition is unsuitable for DBCS.
		// It is possible that new one allows to encode extended character
		// even if running system is DBCS.
		//
		// if Unicode is latin-1 area(<0x100), skip to check DBCS
		// bstrIn == NULL to handle the case were HTMLEncode is called internally
		// and bstrIn is NULL
		//
		// if bstrIn == NULL, chech DBCS
		// if bstrIn != NULL and Unicode is latin-1 area(<0x100), check DBCS
		// else skip to check DBCS
		if (!(bstrIn && bstrIn[i] < 0x100) && ::IsDBCSLeadByteEx(uCodePage, (BYTE)*szSrc))
		{
			// this is a DBCS code page do not encode the data copy 2 bytes
			// no incremnt because of using CharNextExA at the end of the loop
			if (szEnd - szDest >= 2)
			{
				*szDest++ = *szSrc;
				*szDest++ = *(szSrc + 1);
			}
		}
		//
		// Japanese only.
		// Do not encode if character is half-width katakana character.
		//
		else if ((uCodePage == 932 || uCodePage == CP_ACP && ::GetACP() == 932) && _ismbbkana(*szSrc))
		{
			if (szEnd > szDest)
			{
				*szDest++ = *szSrc;
			}
		}
		// Special case character encoding
		else if (*szSrc == '<')
		{
			if (szEnd - szDest >= 4)
			{
				CopyMemory(szDest, "&lt;", 4);
				szDest += 4;
			}
		}
		else if (*szSrc == '>')
		{
			if (szEnd - szDest >= 4)
			{
				CopyMemory(szDest, "&gt;", 4);
				szDest += 4;
			}
		}
		else if (*szSrc == '&')
		{
			if (szEnd - szDest >= 5)
			{
				CopyMemory(szDest, "&amp;", 5);
				szDest += 5;
			}
		}
		else if (*szSrc == '"')
		{
			if (szEnd - szDest >= 6)
			{
				CopyMemory(szDest, "&quot;", 6);
				szDest += 6;
			}
		}
		//
		// According RFC, if character code is greater than equal 0x80, encode it.
		//
		// BUG 153089 - WideCharToMultiByte would incorrectly convert some
		// characters above the range of 0x80 so we now use the BSTR as our source
		// to check for characters that should be encoded.
		//
		else if (bstrIn && (bstrIn[i] >= 0x80))
		{
			DWORD dwTemp;

			// Check if the bstrIn currently points to a surrogate Pair
			// Surrogate pairs would account for 2 bytes in the BSTR.
			// High Surrogate = U+D800 <==> U+DBFF
			// Low Surrogate = U+DC00 <==> U+DFFF
			if (IsSurrogateHigh(bstrIn[i]) 	// Check the higher byte.
				&& IsSurrogateLow(bstrIn[i + 1])) // Check the lower byte too.
			{
				dwTemp = (((bstrIn[i] & 0x3ff) << 10) | (bstrIn[i + 1] & 0x3ff)) + 0x10000;
				i++; // Increment bstrIn index as surrogatepair was detected.
			}
			else
			{
				dwTemp = bstrIn[i];
			}

			if (szEnd - szDest >= 9)
			{

				*szDest++ = '&';
				*szDest++ = '#';

				_ultoa(dwTemp, szDest, 10);
				szDest += strlen(szDest);

				*szDest++ = ';';
			}
		}
		else if ((unsigned char)*szSrc >= 0x80)
		{
			// Since this is unsigned char casting, the value of WORD wTemp
			// is not going to exceed 0xff(255).  So, 3 digit is sufficient here.
			WORD wTemp = (unsigned char)*szSrc;

			if (szEnd - szDest >= 6)
			{
				*szDest++ = '&';
				*szDest++ = '#';
				for (WORD Index = 100; Index > 0; Index /= 10)
				{
					*szDest++ = ((unsigned char)(wTemp / Index)) + '0';
					wTemp = wTemp % Index;
				}

				*szDest++ = ';';
			}
		}
		else
		{
			if (szEnd > szDest)
			{
				*szDest++ = *szSrc; cchDest--;
			}
		}

		// increment szSrc and i (they must be kept in sync)
		szSrc = AspCharNextA(WORD(uCodePage), szSrc);

		i++;	// Regular increment of the bstrIn index.
	}

	if (szEnd > szDest)
	{
		*szDest = '\0';
	}
	else
	{
		*szEnd = '\0';
	}
	return pszDest;
}
