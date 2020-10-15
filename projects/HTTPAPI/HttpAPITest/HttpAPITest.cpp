#include <Windows.h>
#include <iostream>

#pragma comment(lib, "advapi32.lib")

#include "SecurityUtility.cpp"
using namespace SecurityUtility;

#include "RegistryUtility.cpp"
using namespace RegistryUtility;

int main()
{
    HKEY key = NULL;
    LPWSTR subKeyName = (LPWSTR) L"test4";
    LPWSTR userName = (LPWSTR) L"redmond\\jhkim";
    LPWSTR valueName = (LPWSTR)L"test4";
    LPWSTR data = (LPWSTR) L"abc";
    size_t dataLength = 0;

    SECURITY_ATTRIBUTES securityAttribute;
    if (!CreateSecurityAttributes(userName, &securityAttribute))
    {
        std::wcout << L"Failed to create the security attribute of the user, " << userName << std::endl;
    }

    if (!CreateRegistryKey(HkeyDestination::CurrentUser, subKeyName, &securityAttribute, &key))
    {
        std::wcout << L"Failed to create the registry key of the " << subKeyName << std::endl;
        goto CleanUp;
    }

    dataLength = (wcslen(data) + 1) * sizeof(wchar_t);
    if (!SetValue(&key, valueName, data, dataLength))
    {
        std::wcout << L"Failed to set value" << std::endl;
        goto CleanUp;
    }

    goto CleanUp;

CleanUp:

    if (key)
    {
        RegCloseKey(key);
        key = NULL;
    }
    return 0;
}