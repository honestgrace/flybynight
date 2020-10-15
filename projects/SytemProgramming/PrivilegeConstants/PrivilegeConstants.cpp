#include <iostream>
#include <Windows.h>
#include <sddl.h>
#include <string>
#pragma comment(lib, "advapi32.lib")

std::wostream& operator<<(std::wostream& os, const LUID& luid)
{
    os << L"(" << luid.HighPart << L", " << luid.LowPart << L")";

    return os;
}


void TestLuid()
{
    LUID luid1{};
    std::wcout << luid1 << std::endl;

    LUID luid2{};
    AllocateLocallyUniqueId(&luid2);
    std::wcout << L"Luid2: " << luid2 << std::endl;

    AllocateLocallyUniqueId(&luid1);
    std::wcout << L"Luid1: " << luid1 << std::endl;
}

LUID GetLuidValue(const wchar_t* pc)
{
    LUID luid;
    if (!LookupPrivilegeValue(NULL, pc, &luid))
    {
        std::cout << "Failed to convert Privilege string to LUID" << std::endl;
        return {};
    }
    else
        return luid;
}

void TestStringToLuid()
{
    std::wcout << SE_ASSIGNPRIMARYTOKEN_NAME << L": " 
        << GetLuidValue(SE_ASSIGNPRIMARYTOKEN_NAME) << std::endl;

    std::wcout << SE_AUDIT_NAME << L": "
        << GetLuidValue(SE_AUDIT_NAME) << std::endl;

    std::wcout << SE_BACKUP_NAME << L": "
        << GetLuidValue(SE_BACKUP_NAME) << std::endl;
}

std::wstring GetPrivilegeString(LUID luid)
{
    DWORD dwSize = 0;
    
    // STEP 1. query the character count of the string buffer
    if (!LookupPrivilegeName(NULL, &luid, NULL, &dwSize)
        && GetLastError() != ERROR_INSUFFICIENT_BUFFER)
    {
        std::wcout << L"Failed to query character size of the buffer" << std::endl;
        return L"";
    }

    std::wstring str(dwSize, L'*');

    if (!LookupPrivilegeName(NULL, &luid, &str[0], &dwSize))
    {
        std::wcout << L"Failed to convert to string" << std::endl;
        return L"";
    }
    else
    {
        return str;
    }
}

void TestConvertToPrivilegeString()
{
    std::wcout << SE_ASSIGNPRIMARYTOKEN_NAME << L": "
        << L": [" << GetPrivilegeString(GetLuidValue(SE_ASSIGNPRIMARYTOKEN_NAME)) << L"]" << std::endl;

    std::wcout << SE_AUDIT_NAME << L": "
        << L": [" << GetPrivilegeString(GetLuidValue(SE_AUDIT_NAME)) << L"]" << std::endl;

    std::wcout << SE_BACKUP_NAME << L": "
        << L": [" << GetPrivilegeString(GetLuidValue(SE_BACKUP_NAME)) << L"]" << std::endl;
}

std::wstring GetPrivilegeDisplayName(const wchar_t* pc)
{
    DWORD dwSize = 0;
    DWORD langId = LANG_USER_DEFAULT;

    if (!LookupPrivilegeDisplayName(NULL, pc, NULL, &dwSize, &langId)
        && GetLastError() != ERROR_INSUFFICIENT_BUFFER)
    {
        std::wcout << "Failed to query string buffer size" << std::endl;
        return L"";
    }

    std::wstring str(dwSize, L'*');

    if (!LookupPrivilegeDisplayName(NULL, pc, &str[0], &dwSize, &langId))
    {
        std::wcout << "Failed to retrieve privilege display string" << std::endl;
    }

    return str;
}

void TestConvertToPrivilegeDisplayName()
{
    std::wcout << SE_ASSIGNPRIMARYTOKEN_NAME << L": "
        << L": [" << GetPrivilegeDisplayName(GetPrivilegeString(GetLuidValue(SE_ASSIGNPRIMARYTOKEN_NAME)).c_str()) << L"]" << std::endl;

    std::wcout << SE_AUDIT_NAME << L": "
        << L": [" << GetPrivilegeDisplayName(GetPrivilegeString(GetLuidValue(SE_AUDIT_NAME)).c_str()) << L"]" << std::endl;

    std::wcout << SE_BACKUP_NAME << L": "
        << L": [" << GetPrivilegeDisplayName(GetPrivilegeString(GetLuidValue(SE_BACKUP_NAME)).c_str()) << L"]" << std::endl;

}

int main()
{
    //TestLuid();
    //TestStringToLuid();
    //TestConvertToPrivilegeString();
    TestConvertToPrivilegeDisplayName();
    return 0;
}

