#include "stdafx.h"
#include "C:\gitroot\tools\projects\NativeAPITest\IISLib\stringu.h"
#include <iostream>

HRESULT
GetRequestQueueDelegatorIdentitySid(
    IN LPCWSTR pszUserName,
    OUT BUFFER * pbuffSid
)
{
    HRESULT                 hr = S_OK;
    BUFFER                  buffDomain;
    DWORD                   cbSid;
    DWORD                   cchDomainName;
    SID_NAME_USE            sidNameUse;

    cbSid = pbuffSid->QuerySize();
    cchDomainName = buffDomain.QuerySize() / sizeof(WCHAR);
    if (!LookupAccountName(NULL,
        pszUserName,
        pbuffSid->QueryPtr(),
        &cbSid,
        (WCHAR*)buffDomain.QueryPtr(),
        &cchDomainName,
        &sidNameUse))
    {
        if (GetLastError() != ERROR_INSUFFICIENT_BUFFER ||
            !pbuffSid->Resize(cbSid) ||
            !buffDomain.Resize(cchDomainName * sizeof(WCHAR)))
        {
            return HRESULT_FROM_WIN32(GetLastError());
        }

        cbSid = pbuffSid->QuerySize();
        cchDomainName = buffDomain.QuerySize() / sizeof(WCHAR);
        if (!LookupAccountName(NULL,
            pszUserName,
            pbuffSid->QueryPtr(),
            &cbSid,
            (WCHAR*)buffDomain.QueryPtr(),
            &cchDomainName,
            &sidNameUse))
        {
            return HRESULT_FROM_WIN32(GetLastError());
        }
    }

    if (!IsValidSid(pbuffSid->QueryPtr()))
    {
        hr = HRESULT_FROM_WIN32(ERROR_INVALID_DATA);
        goto exit;
    }

exit:
    return hr;
}

HRESULT
GetRequestQueueDelegatorIdentitySid2(
    IN LPCWSTR pszRequestQueueDelegatorIdentity,
    OUT PSID& pSidRequestQueueDelegatorIdentity
)
{
    HRESULT hr = S_OK;
    SID_NAME_USE sidNameUse;
    DWORD cbSidRequestQueueDelegatorIdentity = 0;
    LPWSTR pszReferencedDomainName = NULL;
    DWORD cchReferencedDomainName = 0;

    if (LookupAccountName(NULL,                       // lpSystemName
        pszRequestQueueDelegatorIdentity,
        pSidRequestQueueDelegatorIdentity,
        &cbSidRequestQueueDelegatorIdentity,
        pszReferencedDomainName,
        &cchReferencedDomainName,
        &sidNameUse))
    {
        hr = HRESULT_FROM_WIN32(ERROR_INVALID_DATA);
        /*DPERROR((
            DBG_CONTEXT,
            hr,
            "LookupAccountName returned TRUE unexpectedly while obtaining required buffer sizes failed\n"
            ));*/

        goto exit;
    }

    hr = HRESULT_FROM_WIN32(GetLastError());
    if (hr != HRESULT_FROM_WIN32(ERROR_INSUFFICIENT_BUFFER))
    {
        /*DPERROR((
            DBG_CONTEXT,
            hr,
            "Obtaining required buffer sizes failed\n"
            ));*/
        goto exit;
    }
    hr = S_OK;

    pSidRequestQueueDelegatorIdentity = (PSID)LocalAlloc(LPTR, cbSidRequestQueueDelegatorIdentity);
    if (pSidRequestQueueDelegatorIdentity == NULL)
    {
        hr = HRESULT_FROM_WIN32(ERROR_NOT_ENOUGH_MEMORY);
        /*DPERROR((
            DBG_CONTEXT,
            hr,
            "Allocating buffer for the request queue delegator account SID failed\n"
            ));*/
        goto exit;
    }

    //
    // cchReferencedDomainName includes the NULL terminator
    //
    pszReferencedDomainName = (LPWSTR)LocalAlloc(
        LPTR,
        cchReferencedDomainName * sizeof(WCHAR)
    );
    if (pszReferencedDomainName == NULL)
    {
        hr = HRESULT_FROM_WIN32(ERROR_NOT_ENOUGH_MEMORY);
        /*DPERROR((
            DBG_CONTEXT,
            hr,
            "Allocating buffer for the reference domain name failed\n"
            ));*/
        goto exit;
    }

    if (!LookupAccountName(NULL,                      // lpSystemName
        pszRequestQueueDelegatorIdentity,
        pSidRequestQueueDelegatorIdentity,
        &cbSidRequestQueueDelegatorIdentity,
        pszReferencedDomainName,
        &cchReferencedDomainName,
        &sidNameUse))
    {
        hr = HRESULT_FROM_WIN32(GetLastError());
        /*DPERROR((
            DBG_CONTEXT,
            hr,
            "Looking up the request queue delegator account SID failed\n"
            ));*/
        goto exit;
    }

    //DBG_ASSERT(pSidRequestQueueDelegatorIdentity != NULL);
    if (!IsValidSid(pSidRequestQueueDelegatorIdentity))
    {
        hr = HRESULT_FROM_WIN32(ERROR_INVALID_DATA);
        goto exit;
    }

exit:

    if (pszReferencedDomainName != NULL)
    {
        LocalFree(pszReferencedDomainName);
        pszReferencedDomainName = NULL;
    }

    return hr;
}

void TestIisUtil()
{
    WCHAR data[256] = L"redmond\\jhkim";

    LPCWSTR pszRequestQueueDelegatorIdentity = data;
    PSID pSidRequestQueueDelegatorIdentity = NULL;
    HRESULT hr;
    BUFFER buffSidRequestQueueDelegatorIdentity;
    std::cout << "psid" << sizeof(PSID) << std::endl;

    std::cout << "buffer before" << buffSidRequestQueueDelegatorIdentity.QuerySize() << std::endl;
    buffSidRequestQueueDelegatorIdentity.Resize(1);
    std::cout << "buffer after" << buffSidRequestQueueDelegatorIdentity.QuerySize() << std::endl;

    hr = GetRequestQueueDelegatorIdentitySid2(pszRequestQueueDelegatorIdentity, pSidRequestQueueDelegatorIdentity);
    if (hr != S_OK)
    {
        goto exit;
    }

    hr = GetRequestQueueDelegatorIdentitySid(pszRequestQueueDelegatorIdentity, &buffSidRequestQueueDelegatorIdentity);
    if (hr != S_OK)
    {
        goto exit;
    }

exit:

    if (pSidRequestQueueDelegatorIdentity != NULL)
    {
        LocalFree(pSidRequestQueueDelegatorIdentity);
        pSidRequestQueueDelegatorIdentity = NULL;
    }

    return;
}