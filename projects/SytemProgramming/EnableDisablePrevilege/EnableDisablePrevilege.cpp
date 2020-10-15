#include <iostream>
#include <Windows.h>
#include <sddl.h>
#include <string>
#include <memory> // for std::unique_ptr

#pragma comment(lib, "advapi32.lib")

std::wostream& operator<<(std::wostream& os, const LUID& luid)
{
    os << L"(" << luid.HighPart << L", " << luid.LowPart << L")";

    return os;
}

bool operator==(const LUID& luid1, const LUID& luid2)
{
    return  (luid1.HighPart == luid2.HighPart)
        && (luid1.LowPart == luid2.LowPart);
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

BOOL DisplayPrivilege()
{
    HANDLE hToken = NULL;

    // STEP 1. Get access token of the process
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, &hToken))
    {
        std::cout << "Failed to get access token" << std::endl;
        return FALSE;
    }

    // STEP 2. Query the byte size of TokenPrivilege buffer
    DWORD dwSize = 0;
    if (!GetTokenInformation(hToken, TokenPrivileges, NULL, 0, &dwSize)
        && GetLastError() != ERROR_INSUFFICIENT_BUFFER)
    {
        std::cout << "Failed to query the buffer size" << std::endl;
        CloseHandle(hToken);
        return FALSE;
    }

    // STEP 3. Allocate buffer memory
    PTOKEN_PRIVILEGES pTokenPrivileges = (PTOKEN_PRIVILEGES)GlobalAlloc(GPTR, dwSize);

    // STEP 4. Retrieve Privileges from access token
    if (!GetTokenInformation(hToken, TokenPrivileges, pTokenPrivileges, dwSize, &dwSize))
    {
        std::cout << "Failed to retrieve privileges" << std::endl;
        GlobalFree(pTokenPrivileges);
        CloseHandle(hToken);
        return FALSE;
    }

    // STEP 5. Use the retrieved privilege LUID
    // Note that the retrieved Privilege is LUID type,
    // we have to convert it to string.
    for (DWORD i = 0; i < pTokenPrivileges->PrivilegeCount; ++i)
    {
        auto& attributes = pTokenPrivileges->Privileges[i].Attributes;
        auto& luid = pTokenPrivileges->Privileges[i].Luid;

        if (attributes & SE_PRIVILEGE_ENABLED)
        {
            std::cout << "Enabled - ";
        }
        else
        {
            std::cout << "Disabled - ";
        }

        // local block in for - loop
        {
            dwSize = 0;
            if (!LookupPrivilegeName(NULL, &luid, NULL, &dwSize)
                && GetLastError() != ERROR_INSUFFICIENT_BUFFER)
            {
                std::cout << "Failed to query buffer size for Privilege Name" << std::endl;
            }

            DWORD privilege_name_buffer = dwSize
                + sizeof(wchar_t); // for null terminator L'\0'

            std::unique_ptr<wchar_t[]> pPriName(new wchar_t[privilege_name_buffer]);

            if (!LookupPrivilegeName(NULL, &luid, pPriName.get(), &dwSize))
            {
                std::cout << "Failed to convert LUID to string" << std::endl;
                continue;
            }

            std::wcout << pPriName.get() << L" - ";

            dwSize = 0;
            DWORD langid = LANG_USER_DEFAULT;
            if (!LookupPrivilegeDisplayName(NULL, pPriName.get(), NULL, &dwSize, &langid)
                && GetLastError() != ERROR_INSUFFICIENT_BUFFER)
            {
                std::cout << "Failed to query buffer size for privilege display name" << std::endl;
                continue;
            }

            DWORD disp_name_buffer = dwSize
                + sizeof(wchar_t); // for null terminator L'\0'

            std::unique_ptr<wchar_t[]> pDispName(new wchar_t[disp_name_buffer]);

            if (!LookupPrivilegeDisplayName(NULL, pPriName.get(), pDispName.get(), &dwSize, &langid))
            {
                std::cout << "Failed to convert to display name" << std::endl;
                continue;
            }

            std::wcout << pDispName.get() << std::endl;
        }
    }

    // STEP 6. After use, free allocated memory and close handle.
    GlobalFree(pTokenPrivileges);
    CloseHandle(hToken);
}

BOOL SetPrivilege(HANDLE hToken, LUID luid, BOOL bEnable)
{
    TOKEN_PRIVILEGES tp;

    tp.PrivilegeCount = 1;
    tp.Privileges[0].Attributes = bEnable ? SE_PRIVILEGE_ENABLED : 0;
    tp.Privileges[0].Luid = luid;

    if (!AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL))
    {
        std::cout << "Failed to adjust previlege, error "
            << GetLastError() << std::endl;

        return FALSE;
    }
    
    DWORD rlt = GetLastError();
    if (rlt != ERROR_SUCCESS)
    {
        std::cout << "Failed to adjust privleges" << std::endl;
        return FALSE;
    }
    else
        return TRUE;
}

BOOL SetPrivilege(HANDLE hToken, const wchar_t* previlege_const, BOOL bEnable)
{
    LUID luid = GetLuidValue(previlege_const);

    if (luid == LUID{})
        return FALSE;
    else
        return SetPrivilege(hToken, luid, bEnable);
}

int TestSetPrevilege()
{
    std::cout << "Before Adjust Privilege ----" << std::endl;
    DisplayPrivilege();

    HANDLE hToken;
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken))
    {
        std::cout << "Failed to get access token" << std::endl;
        return 0;
    }

    std::cout << "After Adjust Privilege ----" << std::endl;
    SetPrivilege(hToken, SE_SHUTDOWN_NAME, TRUE);
    DisplayPrivilege();
    CloseHandle(hToken);


    return 0;
}

BOOL SetPrivilege(BOOL bEnable)
{
    HANDLE hToken = NULL;

    // STEP 1. Get access token of the process
    if (!OpenProcessToken(GetCurrentProcess(),
        TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
    {
        std::cout << "Failed to get access token" << std::endl;
        return FALSE;
    }

    // STEP 2. Query the byte size of TokenPrivilege buffer
    DWORD dwSize = 0;
    if (!GetTokenInformation(hToken, TokenPrivileges, NULL, 0, &dwSize)
        && GetLastError() != ERROR_INSUFFICIENT_BUFFER)
    {
        std::cout << "Failed to query the buffer size" << std::endl;
        CloseHandle(hToken);
        return FALSE;
    }

    // STEP 3. Allocate buffer memory
    PTOKEN_PRIVILEGES pTokenPrivileges = (PTOKEN_PRIVILEGES)GlobalAlloc(GPTR, dwSize);

    // STEP 4. Retrieve Privileges from access token
    if (!GetTokenInformation(hToken, TokenPrivileges, pTokenPrivileges, dwSize, &dwSize))
    {
        std::cout << "Failed to retrieve privileges" << std::endl;
        GlobalFree(pTokenPrivileges);
        CloseHandle(hToken);
        return FALSE;
    }

    // STEP 5. Use the retrieved privilege LUID
    // Note that the retrieved Privilege is LUID type,
    // we have to convert it to string.
    for (DWORD i = 0; i < pTokenPrivileges->PrivilegeCount; ++i)
    {
        auto& attributes = pTokenPrivileges->Privileges[i].Attributes;
        auto& luid = pTokenPrivileges->Privileges[i].Luid;

        SetPrivilege(hToken, luid, bEnable);
    }

    // STEP 6. After use, free allocated memory and close handle.
    GlobalFree(pTokenPrivileges);
    CloseHandle(hToken);
}

BOOL TestSetPrivilegeAll()
{
    std::cout << "Default process privilege ------" << std::endl;
    DisplayPrivilege();

    std::cout << "Disable all process privileges ------" << std::endl;
    SetPrivilege(FALSE);
    DisplayPrivilege();

    std::cout << "Enable all process privileges ------" << std::endl;
    SetPrivilege(TRUE);
    DisplayPrivilege();
    return 0;
}

int main()
{
    TestSetPrevilege();
    TestSetPrivilegeAll();
    return 0;
}


