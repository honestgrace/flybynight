#include <Windows.h>
#include <iostream>
#include <vector>

namespace RegistryUtility
{
    enum HkeyDestination
    {
        ClassRoot,      // HKEY_CLASSES_ROOT
        CurrentUser,    // HKEY_CURRENT_USER
        LocalMachine    // HKEY_LOCAL_MACHINE
    };

    HKEY GetHKey(HkeyDestination desitnation)
    {
        HKEY result{};
        switch (desitnation)
        {
        case ClassRoot:
            result = HKEY_CLASSES_ROOT;
            break;
        case CurrentUser:
            result = HKEY_CURRENT_USER;
            break;
        case LocalMachine:
            result = HKEY_LOCAL_MACHINE;
            break;
        default:
            std::cout << "Invalid Key destination" << std::endl;
            break;
        };
        return result;
    }

    bool CreateRegistryKey(HkeyDestination desitnation, LPWSTR keyName, LPSECURITY_ATTRIBUTES pSecurityAttribute, HKEY* pKey)
    {
        DWORD dwDisposition;
        
        if (ERROR_SUCCESS != RegCreateKeyEx(GetHKey(desitnation),
            keyName,
            0,
            (LPWSTR)L"", 0,
            KEY_READ | KEY_WRITE,
            pSecurityAttribute,
            pKey,
            &dwDisposition))
        {
            std::cout << "Failed to create key" << std::endl;
            return FALSE;
        }
        
        return TRUE;
    }

    bool SetValue(HKEY * pKey, LPWSTR valueName, LPWSTR data, size_t dataLength)
    {
        if (ERROR_SUCCESS != RegSetValueEx(*pKey, valueName, 0, REG_SZ, (LPBYTE)data, dataLength))
        {
            std::cout << "Failed to set registry value" << std::endl;
            return FALSE;
        }
        else
            return TRUE;
    }
}