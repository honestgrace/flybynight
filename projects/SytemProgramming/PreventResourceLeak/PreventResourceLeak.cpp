#include "winapi_util.h"

BOOL IsProcessInAdminGroup()
{
    BOOL bAdminGroup = FALSE;

    // Macro for tpf::SmartSid pSidAdministrators (FreeSid);
    WinAPI_SmartSid(pSidAdministrators);
    
    // Macro for tpf::WinAPIException wae(__FILE__, __LINE__, __FUNCTION__);
    WAE_DeclException(wae);

    // STEP 1. make a SID for local Administrators group
    wae.increase_step(__LINE__);
    SID_IDENTIFIER_AUTHORITY AuthorityAdministrators
        = SECURITY_NT_AUTHORITY;
    if (!AllocateAndInitializeSid(&AuthorityAdministrators, 2,
        SECURITY_BUILTIN_DOMAIN_RID,
        DOMAIN_ALIAS_RID_ADMINS,
        0, 0, 0, 0, 0, 0, &pSidAdministrators))
    {
        std::cout << "Failed to retrieve SID for the Administrators group"
            << std::endl;

        // Macro for wae.retrieve_error_throw(__LINE__);
        WAE_RetrieveErrorThrow(wae);
    }

    // STEP 2. check SID's membership in the Administrators group
    if (!CheckTokenMembership(NULL, pSidAdministrators, &bAdminGroup))
    {
        std::cout << "Failed to check SID's membership"
            << std::endl;

        // Macro for wae.retrieve_error_throw(__LINE__);
        WAE_RetrieveErrorThrow(wae);
    }

    // for testing error 
    // wae.set_error_throw(12, __LINE__);

    wae.throw_if_error();

    return bAdminGroup;
}

int main()
{
    std::cout << std::boolalpha;

    std::cout << "Is Process running in the Administrators group? ";
    try
    {
        std::cout << (IsProcessInAdminGroup() == TRUE) << std::endl;
    }
    catch(tpf::WinAPIException& e)
    {
        std::cout << e << std::endl;
    }
    
    return 0;
}


