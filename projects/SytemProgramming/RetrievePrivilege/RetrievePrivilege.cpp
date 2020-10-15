#include <iostream>
#include <Windows.h>
#include <sddl.h>
#include <memory> // for std::unique_ptr

#pragma comment(lib, "advapi32.lib")

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

int main()
{
    DisplayPrivilege();
}
