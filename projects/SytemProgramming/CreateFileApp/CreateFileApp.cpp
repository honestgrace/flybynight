#include <Windows.h>
#include <iostream>

int main()
{
    // STEP 1. Create or open file
    HANDLE hFile = CreateFileA(
        "TestSample.txt",   // file name
        GENERIC_WRITE,      // write
        0,                  // do not share
        NULL,               // default security
        CREATE_NEW,         // create file if not exist
        FILE_ATTRIBUTE_NORMAL, // normal file
        NULL);              // attribute template

    // we failed to open file
    if (hFile == INVALID_HANDLE_VALUE)
    {
        std::cout << "FAiled to create or open file" << std::endl;
    }

    char buffer[] = "I love it. Text to write";
    DWORD dwBytesToWrite = strlen(buffer) * sizeof(char);
    DWORD dwBytesWritten = 0;
    WriteFile(hFile,        // file handle
        (void*)buffer,    // start of buffer to write
        dwBytesToWrite,     // number of bytes to write
        &dwBytesWritten,    // number of bytes written to the file
        NULL); //synchronous write

    if (dwBytesToWrite == dwBytesWritten)
    {
        std::cout << "File write successful" << std::endl;
    }
    else if (dwBytesWritten > 0)
    {
        std::cout << "File write partially successful" << std::endl;
    }
    else
    {
        std::cout << "Fie write failed" << std::endl;
        return 0;
    }

    //STEP LAST
    CloseHandle(hFile);

    return 0;
}
