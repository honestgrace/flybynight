#include <Windows.h>
#include <iostream>
#include <string>
#include <AclAPI.h>
#include <sddl.h>

#pragma comment(lib, "advapi32.lib")

// converts PSID to string
std::wstring SidToString(PSID pSid)
{
    wchar_t* pSidString = NULL;
    if (!ConvertSidToStringSid(pSid, &pSidString))
    {
        DWORD dwResult = GetLastError();
        if (dwResult == ERROR_NOT_ENOUGH_MEMORY)
            std::cout << "ERROR_NOT_ENOUGH_MEMORY" << std::endl;
        else if (dwResult == ERROR_INVALID_SID)
            std::cout << "ERROR_INVALID_SID" << std::endl;
        else
            // ERROR_INVALID_PARAMETER
            std::cout << "ERROR_INVALID_PARAMETER" << std::endl;

        return L"";
    }

    std::wstring SidString(pSidString);
    LocalFree(pSidString);

    return SidString;
}

void TestSecurityDescriptor()
{
    PSID pSidOwner = NULL;
    PSID pSidGroup = NULL;
    PSECURITY_DESCRIPTOR pSD = NULL;
    
    if (GetSecurityInfo(GetCurrentProcess(),
        SE_KERNEL_OBJECT,
        OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION,
        &pSidOwner,
        &pSidGroup,
        NULL, // for Dacl
        NULL, //for Sacl
        &pSD) != ERROR_SUCCESS)
    {
        std::cout << "Failed to retrieve Security Descriptor, error: " << GetLastError() << std::endl;
        return;
    }

    DWORD dwAccountName = 0;
    DWORD dwDomainName = 0;
    SID_NAME_USE SidUse = SidTypeUnknown;

    if (!LookupAccountSid(NULL,
        pSidOwner,
        NULL,
        &dwAccountName,
        NULL,
        &dwDomainName,
        &SidUse) && GetLastError() != ERROR_INSUFFICIENT_BUFFER)
    {
        std::cout << "Failed to retrieve buffer size" << std::endl;
        LocalFree(pSD);
        return;
    }

    std::wstring AccountName(dwAccountName - 1, L'*');
    std::wstring DomainName(dwDomainName - 1, L'*');

    if (!LookupAccountSid(NULL,
        pSidOwner,
        &AccountName[0],
        &dwAccountName,
        &DomainName[0],
        &dwDomainName,
        &SidUse))
    {
        std::cout << "Failed to retrieve Account Name and Domain Name" << std::endl;
        LocalFree(pSD);
        return;
    }

    std::wcout << L"pSidOwner - [" << SidToString(pSidOwner) << L"]" << std::endl;
    std::wcout << L"pSidGroup - [" << SidToString(pSidGroup) << L"]" << std::endl;
    std::wcout << L"Owner Name: [" << AccountName << L"]" << std::endl;
    std::wcout << L"Domain Name: [" << DomainName << L"]" << std::endl;

    /*
    We can use pSD, the Security Descriptor of our process.

    NOTE: You should not do 
        FreeSid(pSidOwner)
        FreeSid(pSidGroup)
    That is because pSidOwner and pSidGroup is the member of pSD.

    */

    LocalFree(pSD);
    return;
}

int main()
{
    TestSecurityDescriptor();
}
