#include <Windows.h>
#include <iostream>
#pragma comment(lib,"advapi32.lib")

BOOL IsProcessInAdminGroup()
{
    BOOL bAdminGroup = FALSE;
    PSID pSidAdministrators = NULL;

    while (1)
    {
        // STEP 1. make a SID for local Administrators group
        SID_IDENTIFIER_AUTHORITY AuthorityAdministrators
            = SECURITY_NT_AUTHORITY;
        if (!AllocateAndInitializeSid(&AuthorityAdministrators, 2,
            SECURITY_BUILTIN_DOMAIN_RID,
            DOMAIN_ALIAS_RID_ADMINS,
            0, 0, 0, 0, 0, 0, &pSidAdministrators))
        {
            std::cout << "Failed to retrieve SID for the Administrators group"
                << std::endl;
            break;
        }

        // STEP 2. check SID's membership in the Administrators group
        if (!CheckTokenMembership(NULL, pSidAdministrators, &bAdminGroup))
        {
            std::cout << "Failed to check SID's membership"
                << std::endl;
            break;
        }

        break;
    }

    if (pSidAdministrators)
        FreeSid(pSidAdministrators);

    return bAdminGroup;
}

int main()
{
    std::cout << std::boolalpha;

    std::cout << "Is Process running in the Administrators group? ";
    std::cout << (IsProcessInAdminGroup() == TRUE) << std::endl;

    return 0;
}


