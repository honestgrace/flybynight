#include <iostream>
#include <Windows.h>
#include <strsafe.h>

int main()
{
    // STEP 1. Allocate Memory
    SIZE_T size = 256;
    HLOCAL hMem = LocalAlloc(LMEM_ZEROINIT, size);
    if (hMem == NULL)
    {
        std::cout << "Failed to allocate local heap" << std::endl;
        return 0;
    }

    SIZE_T chars = LocalSize(hMem) / sizeof(char);

    // STEP 2. Use the allocated memory
    HRESULT hr = StringCchPrintfA((char*)hMem,  // local memory handle
        chars - 1, // buffer size, -1 for null terminating character
        "%s is a cool programmer.",
        "Tomas Kim");
    
    if (SUCCEEDED(hr))
    {
        std::cout << "Formatted text:" << (char*)hMem << std::endl;
    }
    else
    {
        std::cout << "Formatting text failed" << std::endl;
    }

    // STEP last
    LocalFree(hMem);

    return 0;
}

