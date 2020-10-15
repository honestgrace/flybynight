#include "stdafx.h"

#define ASSERT DBG_ASSERT
#include <string>
using namespace std;

HRESULT ImpersonateUser(LPCTSTR  pszUserName,
    LPCTSTR  pszDomain,
    LPCTSTR  pszPassword,
    HANDLE  *pCurImpToken,
    HANDLE  *pLoggedOnUserToken)
{
    HRESULT hr = S_OK;

    ASSERT(pCurImpToken);
    ASSERT(pLoggedOnUserToken);

    *pCurImpToken = INVALID_HANDLE_VALUE;
    *pLoggedOnUserToken = INVALID_HANDLE_VALUE;

    // logon the user.  This creates an primary impersonation
    // token.  If this fails, an error will be returned.

    if (!LogonUser((LPTSTR)pszUserName,
        (LPTSTR)pszDomain,
        (LPTSTR)pszPassword,
        LOGON32_LOGON_BATCH,
        LOGON32_PROVIDER_DEFAULT,
        pLoggedOnUserToken)) {

        hr = HRESULT_FROM_WIN32(GetLastError());
    }
    // get the current impersonation token.  If an error occurs,
    // that is OK.  This just means that no impersonation on the
    // thread is occurring, so there is no need to do the RevertToSelf.

    else if (OpenThreadToken(GetCurrentThread(),
        TOKEN_READ | TOKEN_IMPERSONATE,
        TRUE,
        pCurImpToken)) {

        RevertToSelf();
    }
    else if (GetLastError() != ERROR_NO_TOKEN)
    {
        hr = HRESULT_FROM_WIN32(GetLastError());
    }

    // if everything's been successful so far, than call 
    // ImpersonateLoggedOnUser with the token created above.

    if (SUCCEEDED(hr)) {

        if (!ImpersonateLoggedOnUser(*pLoggedOnUserToken)) {

            hr = HRESULT_FROM_WIN32(GetLastError());
        }
    }

    // if there were failures, clean up any tokens that we created
    // or hold.

    if (FAILED(hr)) {

        // cleanup the LogonUser token

        if (*pLoggedOnUserToken != INVALID_HANDLE_VALUE) {
            CloseHandle(*pLoggedOnUserToken);
            *pLoggedOnUserToken = INVALID_HANDLE_VALUE;
        }

        // cleanup the token from the OpenThreadToken call

        if (*pCurImpToken != INVALID_HANDLE_VALUE) {
            HANDLE  hThread = GetCurrentThread();
            SetThreadToken(&hThread,
                *pCurImpToken);
            CloseHandle(*pCurImpToken);
            *pCurImpToken = INVALID_HANDLE_VALUE;
        }
    }

    return hr;
}

HRESULT UnImpersonateUser(HANDLE hSavedImpToken, HANDLE hLoggedOnUser)
{
    // if there is an hSavedImpToken, then call SetThreadToken to
    // restore it.

    if (hSavedImpToken != INVALID_HANDLE_VALUE)
    {
        HANDLE  hThread = GetCurrentThread();
        SetThreadToken(&hThread, hSavedImpToken);
        CloseHandle(hSavedImpToken);
    }

    // cleanup the LogonUser token

    if (hLoggedOnUser != INVALID_HANDLE_VALUE)
    {
        CloseHandle(hLoggedOnUser);
    }

    return S_OK;
}

std::wstring s2ws(const std::string& str)
{
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
}

void TestCreateThreadImpersonate(int argc, char **argv)
{
    HANDLE m_hImpToken = S_OK;
    HANDLE m_hLogToken = S_OK;
    wstring strDomainName = s2ws(argv[1]);
    if (strDomainName == L"localhost")
    {
        strDomainName = _T("");
    }
    wstring strAdminName = s2ws(argv[2]);
    wstring strAdminPassword = s2ws(argv[3]);
    HRESULT err = ImpersonateUser(strAdminName.c_str(), strDomainName.c_str(), strAdminPassword.c_str(), &m_hImpToken, &m_hLogToken);

    if (SUCCEEDED(err))
    {
        if (m_hImpToken != INVALID_HANDLE_VALUE || m_hLogToken != INVALID_HANDLE_VALUE)
        {
            UnImpersonateUser(m_hImpToken, m_hLogToken);
        }
    }
}