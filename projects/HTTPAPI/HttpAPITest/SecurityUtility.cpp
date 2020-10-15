#include <Windows.h>
#include <iostream>
#include <AclAPI.h>

namespace SecurityUtility
{
    template<typename T, size_t N>
    constexpr size_t ElementCount(T(&)[N])
    {
        return N;
    }

    BOOL CreateSecurityAttributes(LPWSTR userName, SECURITY_ATTRIBUTES *psa)
    {
        BOOL result = FALSE;

        PSID pSidAdministrators = NULL;
        PSID pSidEveryone = NULL;
        PSID pSidLocalSystem = NULL;

        PACL pDacl = NULL;
        PSECURITY_DESCRIPTOR pSecurityDescriptor = NULL;

        SID_IDENTIFIER_AUTHORITY AuthorityAdministrators{ SECURITY_NT_AUTHORITY };
        SID_IDENTIFIER_AUTHORITY AuthorityEveryone{ SECURITY_WORLD_SID_AUTHORITY };

        EXPLICIT_ACCESS ea[] = { {}, {}, {}, {} };
        ULONG ea_count = (ULONG)ElementCount(ea);

        DWORD dwUserName = 0;
        DWORD dwDomainName = 0;
        SID_NAME_USE SidUse = SidTypeUnknown;

        // STEP 0. Create SID for the given username
        PSID pSidUser = NULL;
        LPWSTR referenceDomainName = NULL;

        DWORD dwSidUser = SECURITY_MAX_SID_SIZE;
        DWORD dwReferenceDomainUser = 1024;
        
        pSidUser = (PSID) LocalAlloc(LPTR, SECURITY_MAX_SID_SIZE);
        referenceDomainName = (LPWSTR) LocalAlloc(LPTR, sizeof(TCHAR) * 1024);
        
        if (!LookupAccountName(
            NULL,
            userName,
            pSidUser,
            &dwSidUser,
            referenceDomainName,
            &dwReferenceDomainUser,
            &SidUse))
        {
            std::cout << "Failed to get buffer size for SID, Error: " << GetLastError() << std::endl;
            goto CleanUp;
        }

        
        // STEP 1. Create SID for Everyone
        if (!AllocateAndInitializeSid(&AuthorityEveryone, 1,
            SECURITY_WORLD_RID,
            0, 0, 0, 0, 0, 0, 0, &pSidEveryone))
        {
            std::cout << "Failed to allocate SID for group \"Everyone\"" << std::endl;
            goto CleanUp;
        }
        
        // STEP 2. Create SID for Administrators
        if (!AllocateAndInitializeSid(
            &AuthorityAdministrators,
            2,
            SECURITY_BUILTIN_DOMAIN_RID,
            DOMAIN_ALIAS_RID_ADMINS,
            0, 0, 0, 0, 0, 0,
            &pSidAdministrators))
        {
            std::cout << "Failed to allocate SID for group \"Administrators\"" << std::endl;
            goto CleanUp;
        }

        if (!AllocateAndInitializeSid(
            &AuthorityAdministrators,
            2,
            SECURITY_BUILTIN_DOMAIN_RID,
            DOMAIN_ALIAS_RID_ADMINS,
            0, 0, 0, 0, 0, 0,
            &pSidAdministrators))
        {
            std::cout << "Failed to allocate SID for group \"Administrators\"" << std::endl;
            goto CleanUp;
        }

        if (!AllocateAndInitializeSid(
            &AuthorityAdministrators,
            1,
            SECURITY_LOCAL_SYSTEM_RID,            
            0, 0, 0, 0, 0, 0, 0,
            &pSidLocalSystem))
        {
            std::cout << "Failed to allocate SID for group \"Administrators\"" << std::endl;
            goto CleanUp;
        }

        // STEP 3. Initialize ea with two SIDs
        size_t arrayIndex;        

        arrayIndex = 0;
        ea[arrayIndex].grfAccessPermissions = KEY_READ;      // give permission to read registry key
        ea[arrayIndex].grfAccessMode = SET_ACCESS;           // Indicates an ACCESS_ALLOWED_ACE structure that allows the specified rights        
        ea[arrayIndex].grfInheritance = NO_INHERITANCE;      // do not allow inheritance for our ACE
        ea[arrayIndex].Trustee.TrusteeForm = TRUSTEE_IS_SID;
        ea[arrayIndex].Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
        ea[arrayIndex].Trustee.ptstrName = (LPWSTR)pSidEveryone;

        arrayIndex = 1;
        ea[arrayIndex].grfAccessPermissions =  KEY_ALL_ACCESS;
        ea[arrayIndex].grfAccessMode = SET_ACCESS;
        ea[arrayIndex].grfInheritance = NO_INHERITANCE;
        ea[arrayIndex].Trustee.TrusteeForm = TRUSTEE_IS_SID;
        ea[arrayIndex].Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP; // TRUSTEE_IS_GROUP;
        ea[arrayIndex].Trustee.ptstrName = (LPWSTR)pSidAdministrators;
        
        arrayIndex = 2;
        ea[arrayIndex].grfAccessPermissions = GENERIC_ALL;
        ea[arrayIndex].grfAccessMode = GRANT_ACCESS;
        ea[arrayIndex].grfInheritance = NO_INHERITANCE;
        ea[arrayIndex].Trustee.TrusteeForm = TRUSTEE_IS_SID;
        ea[arrayIndex].Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
        ea[arrayIndex].Trustee.ptstrName = (LPTSTR)pSidLocalSystem;

        arrayIndex = 3;
        ea[arrayIndex].grfAccessPermissions = GENERIC_ALL;
        ea[arrayIndex].grfAccessMode = GRANT_ACCESS;
        ea[arrayIndex].grfInheritance = NO_INHERITANCE;
        ea[arrayIndex].Trustee.TrusteeForm = TRUSTEE_IS_SID;
        ea[arrayIndex].Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
        ea[arrayIndex].Trustee.ptstrName = (LPTSTR)pSidUser;

        // STEP 4. Initialize DACL
        if (SetEntriesInAcl(ea_count, ea, NULL, &pDacl)
            != ERROR_SUCCESS)
        {
            std::cout << "Failed to create a DACL with ACEs" << std::endl;
            goto CleanUp;
        }

        // STEP 5. Create Security Descriptor
        pSecurityDescriptor = (PSECURITY_DESCRIPTOR)GlobalAlloc(GPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);
        if (!pSecurityDescriptor)
        {
            std::cout << "Failed to allocate memory for a Security Descriptor" << std::endl;
            goto CleanUp;
        }

        if (!InitializeSecurityDescriptor(pSecurityDescriptor, SECURITY_DESCRIPTOR_REVISION))
        {
            std::cout << "Failed to initialize the Security Descriptor" << std::endl;
            goto CleanUp;
        }

        // STEP 6. Add DACL to the security descriptor
        if (!SetSecurityDescriptorDacl(pSecurityDescriptor, TRUE, pDacl, FALSE))
        {
            std::cout << "Failed to add DACL to the Security Descriptor" << std::endl;
            goto CleanUp;
        }

        // STEP 7. Assign Security Descriptor to the Security Attribute output
        psa->nLength = sizeof(SECURITY_ATTRIBUTES);
        psa->lpSecurityDescriptor = pSecurityDescriptor;
        psa->bInheritHandle = FALSE;

        result = TRUE;
        goto CleanUp;

CleanUp:
        if (pSecurityDescriptor)
            GlobalFree(pSecurityDescriptor);

        if (pSidAdministrators)
            FreeSid(pSidAdministrators);

        if (pSidEveryone)
            FreeSid(pSidEveryone);

        if (pSidLocalSystem)
            FreeSid(pSidLocalSystem);

        if (pSidUser)
            LocalFree(pSidUser);
        
        if (referenceDomainName)
            LocalFree(referenceDomainName);

        if (pDacl)
            LocalFree(pDacl);

        return result;
    }
}