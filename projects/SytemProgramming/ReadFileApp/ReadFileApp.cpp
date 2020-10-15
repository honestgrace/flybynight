#include <windows.h>
#include <iostream>

int main()
{
    // STEP 1. open file
    HANDLE hFile = CreateFileA(
        "TestSample.txt",
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (hFile == INVALID_HANDLE_VALUE)
    {
        std::cout << "Failed to open file" << std::endl;
        return 0;
    }

    constexpr size_t BUFFSIZE = 256;
    char buffer[BUFFSIZE];
    DWORD dwBytesToRead = BUFFSIZE - 1;  // -1 for null terminator
    DWORD dwBytesRead = 0;
    BOOL bRlt = ReadFile(hFile, (void*)buffer, dwBytesToRead, &dwBytesRead, NULL);

    if (dwBytesRead > 0)
    {
        buffer[dwBytesRead] = '\0';
        std::cout << "File contents: " << buffer << std::endl;
    }

    // STEP ast
    CloseHandle(hFile);

    std::cout << "Press any key to continue...";
    std::cin.get();
    return 0;
}
