// IsUserAdmin.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "winapi_util.h"

BOOL IsUserInAdminGroup()
{
    BOOL bAdminGroup = FALSE;
    WinAPI_TokenHandle(hPrimaryToken);
    WAE_DeclException(wae);
    WAE_IncreaseStep(wae);
    // STEP 1. get primary token of the process
    if (!OpenProcessToken(GetCurrentProcess(),
        TOKEN_QUERY | TOKEN_DUPLICATE, &hPrimaryToken))
    {
        WAE_RetrieveErrorThrow(wae);
    }

    
    // STEP 2. get the well-known SID of the local Administrators group
    WAE_IncreaseStep(wae);
    BYTE pSidAdministrators[SECURITY_MAX_SID_SIZE];
    DWORD dwSize = SECURITY_MAX_SID_SIZE;

    if (!CreateWellKnownSid(WinBuiltinAdministratorsSid,
        NULL, pSidAdministrators, &dwSize))
    {
        WAE_RetrieveErrorThrow(wae);
    }

    // STEP 3. get the elevation type of the primary token
    WAE_IncreaseStep(wae);

    TOKEN_ELEVATION_TYPE elevation_type;
    dwSize = sizeof(TOKEN_ELEVATION_TYPE);
    if (!GetTokenInformation(hPrimaryToken, TokenElevationType, &elevation_type, dwSize, &dwSize))
    {
        WAE_RetrieveErrorThrow(wae);
    }

    // STEP 4. get the token to test
    // if the elevation_type is TokenElevationTypeLimited
    // then get the linked token.
    WAE_IncreaseStep(wae);
    WinAPI_TokenHandle(hTestToken);
    if (elevation_type == TokenElevationTypeLimited)
    {
        dwSize = hTestToken.ByteSize();
        /* Access Token Changes
        When a user logs on to a Windows Vista computer,
        Windows looks at the administrative Windows privileges
        and Relative IDs (RIDs) that the user account possesses
        to determine if the user should receive two access tokens
        (a filtered access token and a full access token).
        Windows will create two access tokens for the user

        The filtered access token will be used by default
        when the user launches application.
        The unmodified full access token, called the linked access token,
        is attached to the filtered access token and
        is used when requests are made to launch applications
        with a full administrative access token.
        */
        if (!GetTokenInformation(hPrimaryToken, TokenLinkedToken,
            &hTestToken, dwSize, &dwSize))
        {
            WAE_RetrieveErrorThrow(wae);
        }
    }
    else
    {
        /*
        DuplicationToken() makes an impersonation token
        */
        if (!DuplicateToken(hPrimaryToken, SecurityIdentification, &hTestToken))
        {
            WAE_RetrieveErrorThrow(wae);
        }
    }

    // STEP 5. check if the SID is in the Administrators group
    WAE_IncreaseStep(wae);
    if (!CheckTokenMembership(hTestToken, pSidAdministrators, &bAdminGroup))
    {
        WAE_RetrieveErrorThrow(wae);
    }

    return bAdminGroup;
}

int main()
{
    try
    {
        std::cout << std::boolalpha;
        std::cout << "Is the user in the Administrators group ? : ";
        std::cout << (IsUserInAdminGroup() == TRUE) << std::endl;
    }
    catch (tpf::WinAPIException & e)
    {
        std::cout << "\n" << e << std::endl;
    }
    return 0;
}
