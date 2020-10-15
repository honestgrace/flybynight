#include <Windows.h>
#include <iostream>
#include <string>
#include <AclAPI.h>
#include <sddl.h>

#pragma comment(lib, "advapi32.lib")

void DisplayError(std::string msg)
{
    std::cout << msg << ", Error: " << GetLastError() << std::endl;
}

void DisplayError(std::wstring msg)
{
    std::wcout << msg << ", Error: " << GetLastError() << std::endl;
}

template<typename T, size_t N>
constexpr size_t ElementCount(T(&)[N])
{
    return N;
}

void CreateSecurityDescriptor()
{
    PSID pSidEveryone = NULL;
    PSID pSidAdministrators = NULL;
    PACL pDacl = NULL;
    PSECURITY_DESCRIPTOR pSDAbsolute = NULL;
    // Should not do PSECURITY_DESCRIPTOR* pSDAbsolute. That is because PSECURITY_DESCRIPTOR is PVOID.

    HKEY hSubKey = NULL;

    while (1)
    {
        // STEP 1. Create SIDs for group "Everyone" and "Administrators"

        // STEP 1.1 Create a SID for group "Everyone"
        SID_IDENTIFIER_AUTHORITY AuthorityEveryone = SECURITY_WORLD_SID_AUTHORITY;

        if (!AllocateAndInitializeSid(&AuthorityEveryone, 1,
            SECURITY_WORLD_RID,
            0, 0, 0, 0, 0, 0, 0, &pSidEveryone))
        {
            DisplayError("Failed to allocate SID for group \"Everyone\"");
            break;
        }

        // STEP 1.2 Create a SID for group "Administrators"
        SID_IDENTIFIER_AUTHORITY AuthorityAdministrators = SECURITY_NT_AUTHORITY;
        if (!AllocateAndInitializeSid(&AuthorityAdministrators, 2,
            SECURITY_BUILTIN_DOMAIN_RID,
            DOMAIN_ALIAS_RID_ADMINS,
            0, 0, 0, 0, 0, 0, &pSidAdministrators))
        {
            DisplayError("Failed to allocate SID for group \"Administrators\"");
            break;
        }

        // STEP 2. Create ACEs - Access Control Entry
        EXPLICIT_ACCESS ea[] = { {}, {} };

        // element count of ea
        ULONG ea_count = (ULONG)ElementCount(ea);

        // STEP 2.1 Create ACE for group "Everyone"
        ea[0].grfAccessMode = SET_ACCESS;           // Indicates an ACCESS_ALLOWED_ACE structure that allows the specified rights        
        ea[0].grfAccessPermissions = KEY_READ;      // give permission to read registry key
        ea[0].grfInheritance = NO_INHERITANCE;      // do not allow inheritance for our ACE
        ea[0].Trustee.TrusteeForm = TRUSTEE_IS_SID;
        ea[0].Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
        ea[0].Trustee.ptstrName = (LPWSTR)pSidEveryone;

        // STEP 2.2 Create ACE for group "Administrators"
        ea[1].grfAccessMode = SET_ACCESS;           
        ea[1].grfAccessPermissions = KEY_ALL_ACCESS; 
        ea[1].grfInheritance = NO_INHERITANCE;      
        ea[1].Trustee.TrusteeForm = TRUSTEE_IS_SID;
        ea[1].Trustee.TrusteeType = TRUSTEE_IS_GROUP;
        ea[1].Trustee.ptstrName = (LPWSTR)pSidAdministrators;

        // STEP 3. Create a DACL,
        // DACL stands for Discretionary Access Control List
        if (SetEntriesInAcl(ea_count, ea, NULL, &pDacl)
            != ERROR_SUCCESS)
        {
            DisplayError("Failed to create a DACL with ACEs");
            break;
        }

        // STEP 4. Allocate and Initialize a Security Descriptor
        // STEP 4.1. Allocate memory for a Security Descriptor
        pSDAbsolute = 
            (PSECURITY_DESCRIPTOR)GlobalAlloc(GPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);

        if (!pSDAbsolute)
        {
            DisplayError("Failed to allocate memory for a Security Descriptor");
            break;
        }

        // STEP 4.2. Initialize the Security Descriptor
        if (!InitializeSecurityDescriptor(pSDAbsolute, SECURITY_DESCRIPTOR_REVISION))
        {
            DisplayError("Failed to initialize the Security Descriptor");
            break;
        }

        // STEP 5. Add DACL to the security descriptor
        if (!SetSecurityDescriptorDacl(pSDAbsolute, TRUE, pDacl, FALSE))
        {
            DisplayError("Failed to add DACL to the Security Descriptor");
            break;
        }

        // STEP 6. Create a Security Attribute
        SECURITY_ATTRIBUTES sa{ sizeof(SECURITY_ATTRIBUTES), pSDAbsolute, FALSE };

        // STEP 7. Use the security descriptor we created so far
        DWORD dwDisposition;
        if (RegCreateKeyEx(HKEY_CURRENT_USER, L"MyFirstRegKey", 0, (LPWSTR)L"", 0,
            KEY_READ | KEY_WRITE, &sa, &hSubKey, &dwDisposition) != ERROR_SUCCESS)
        {
            DisplayError("Failed to create registry subkey");
            break;
        }
        else
        {
            std::cout << "Successfully created registry" << std::endl;
            break;
        }
    }

    // STEP 8. Release the allocated resource
    if (pSidEveryone)
        FreeSid(pSidEveryone);

    if (pSidAdministrators)
        FreeSid(pSidAdministrators);

    if (pDacl)
        LocalFree(pDacl);

    if (hSubKey)
        RegCloseKey(hSubKey);

    if (pSDAbsolute)
        GlobalFree(pSDAbsolute);
}

int main()
{
    CreateSecurityDescriptor();

    return 0;
}
