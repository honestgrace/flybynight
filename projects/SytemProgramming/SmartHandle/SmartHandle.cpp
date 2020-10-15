// SmartHandle.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <iostream>

#include "smart_handle.h"
#include <strsafe.h>

using namespace tpf;

SmartHandle CreateFile(char const* filename)
{
    return {
        CreateFileA(filename,
            GENERIC_WRITE,
            0,
            NULL,
            CREATE_NEW,
            FILE_ATTRIBUTE_NORMAL,
            NULL
        ),
        CloseHandle
    };
}

int main()
{
    /*
    // STEP 1. Call normally
    SmartHandle sh{ 
        CreateFileA("CoolText.txt",
            GENERIC_WRITE,
            0,
            NULL,
            CREATE_NEW,
            FILE_ATTRIBUTE_NORMAL,
            NULL
        ),
        CloseHandle 
    };

    // STEP 2. Call with rambda function
    SmartHandle sh{
        CreateFileA("CoolText.txt",
            GENERIC_WRITE,
            0,
            NULL,
            CREATE_NEW,
            FILE_ATTRIBUTE_NORMAL,
            NULL
        ),
        [](auto handle) { return CloseHandle(handle); }
    };
    */

    // STEP 3. Call with a function return value
    SmartHandle sh = CreateFile("CoolText.txt");

    if (sh)
    {
        std::cout << "file created successfully" << std::endl;
        char buffer[] = "This demonstrates the power of the class SmartHandle.";
        DWORD dwBytesToWrite = (DWORD) strlen(buffer) * sizeof(char);
        DWORD dwBytesWritten = 0;
        WriteFile(sh, // file handle implicitly converted from SmartHandle
            (void*)buffer,
            dwBytesToWrite,
            &dwBytesWritten,
            NULL);

        if (dwBytesToWrite == dwBytesWritten)
        {
            std::cout << "Fie write successful" << std::endl;
        }
        else
        {
            std::cout << "File write failed" << std::endl;
        }
    }

    LocalMem lm{ LocalAlloc(LMEM_ZEROINIT, 256), LocalFree };
    if (lm)
    {
        auto chars = LocalSize(lm) / sizeof(char);
        HRESULT hr = StringCchPrintfA(lm,
            chars - 1,
            "So cool smart handle by %s", "Thomas Kim");
        if (SUCCEEDED(hr))
        {
            std::cout << "Formated Text: " << (char*)lm << std::endl;
        }
    }
    return 0;
}
