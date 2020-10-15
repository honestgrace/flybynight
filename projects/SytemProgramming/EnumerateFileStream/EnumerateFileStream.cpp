// EnumerateFileStream.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <sstream>
#include <memory>
#include <string>
#include <Windows.h>
#include "smart_handle.h"

using namespace tpf;

std::string ReadStream(wchar_t const* filename, WIN32_FIND_STREAM_DATA& fsd)
{
    std::wostringstream wos;
    wos << filename << fsd.cStreamName;

    SmartHandle sh = {
        CreateFile(wos.str().c_str(), // filename
            GENERIC_READ,
            FILE_SHARE_READ,
            NULL,
            OPEN_ALWAYS,
            0,
            NULL),
        CloseHandle
    };

    // + 1 for null terminating character
    DWORD dwBytesToRead = (DWORD)fsd.StreamSize.QuadPart + 1;
    DWORD dwBytesWritten = 0;

    // allocate memory
    std::unique_ptr<char[]> ptr(new char[(size_t)dwBytesToRead]{ '0' });

    if (ReadFile(sh, // file handle
        (void*)& ptr[0], dwBytesToRead, &dwBytesWritten, NULL))
    {
        ptr[dwBytesWritten] = '\0';
    }

    return { ptr.get() };
}

int wmain(int argc, wchar_t* argv[])
{
    if (argc != 2)
    {
        std::cout << "Specify file name to enumerate file streams in it." << std::endl;
    }
    else
    {
        WIN32_FIND_STREAM_DATA fsd{};

        SmartHandle sh_streams = {
            FindFirstStreamW(argv[1], FindStreamInfoStandard, (void*)& fsd, 0),
            CloseHandle };

        if (sh_streams)
        {
            std::wcout << L"Stream name - " << fsd.cStreamName
                << L"\t\t Size: " << fsd.StreamSize.QuadPart << std::endl;

            std::cout << "Contents: [" << ReadStream(argv[1], fsd) << "]" << std::endl << std::endl;
         }

        while (FindNextStreamW(sh_streams, (void*)& fsd))
        {
            std::wcout << L"Stream name - " << fsd.cStreamName
                << L"\t\t Size: " << fsd.StreamSize.QuadPart << std::endl;

            std::cout << "Contents: [" << ReadStream(argv[1], fsd) << "]" << std::endl << std::endl;
        }
    }

    return 0;
}

