#include <iostream>
#include "smart_handle.h"

int main()
{
    // MSDN File Stream
    // file-streams, using-streams
    using namespace tpf;
    {
        SmartHandle sh =
        {
            CreateFileA("FileWithStream.txt",  // write to default stream
                GENERIC_WRITE,
                FILE_SHARE_WRITE,
                NULL,
                OPEN_ALWAYS,
                0,
                NULL),
             CloseHandle
        };

        if (sh)
        {
            char buffer[] = "Message to default stream";
            DWORD dwBytesToWrite = (DWORD) (strlen(buffer) * sizeof(char));
            DWORD dwBytesWritten = 0;

            WriteFile(sh,
                (void*)buffer,
                dwBytesToWrite,
                &dwBytesWritten,
                NULL);
        }

        ////////////////
        SmartHandle sh_name =
        {
            CreateFileA("FileWithStream.txt:MyName",  // write to MyName stream
                GENERIC_WRITE,
                FILE_SHARE_WRITE,
                NULL,
                OPEN_ALWAYS,
                0,
                NULL),
             CloseHandle
        };
        if (sh_name)
        {
            char buffer[] = "My name is As.G. Thomas Kim";
            DWORD dwBytesToWrite = (DWORD)(strlen(buffer) * sizeof(char));
            DWORD dwBytesWritten = 0;

            WriteFile(sh_name,
                (void*)buffer,
                dwBytesToWrite,
                &dwBytesWritten,
                NULL);
        }

        ////////////////
        SmartHandle sh_age =
        {
            CreateFileA("FileWithStream.txt:MyAge",  // write to MyName stream
                GENERIC_WRITE,
                FILE_SHARE_WRITE,
                NULL,
                OPEN_ALWAYS,
                0,
                NULL),
             CloseHandle
        };
        if (sh_age)
        {
            char buffer[] = "I am 19 years old";
            DWORD dwBytesToWrite = (DWORD)(strlen(buffer) * sizeof(char));
            DWORD dwBytesWritten = 0;

            WriteFile(sh_age,
                (void*)buffer,
                dwBytesToWrite,
                &dwBytesWritten,
                NULL);
        }
    }
}
