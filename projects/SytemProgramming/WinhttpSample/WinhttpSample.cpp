#include <Windows.h>
#include <winhttp.h>
#include <iostream>
#pragma comment(lib, "winhttp.lib")
int main()
{
    DWORD dwSize = 0;
    DWORD dwDownloaded = 0;
    LPSTR pszOutBuffer;
    BOOL bResults = FALSE;
    HINTERNET hSession = NULL,
        hConnect = NULL,
        hRequest = NULL;

    hSession = WinHttpOpen(L"WinHttp Example/1.0",
        WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
        WINHTTP_NO_PROXY_NAME,
        WINHTTP_NO_PROXY_BYPASS, 0);
    if (hSession)
        hConnect = WinHttpConnect(hSession,
            L"www.microsoft.com",
            INTERNET_DEFAULT_HTTPS_PORT, 0);

    if (hConnect)
        hRequest = WinHttpOpenRequest(hConnect,
            L"GET",
            NULL,
            NULL,
            WINHTTP_NO_REFERER,
            WINHTTP_DEFAULT_ACCEPT_TYPES,
            WINHTTP_FLAG_SECURE);

    if (hRequest)
        bResults = WinHttpSendRequest(hRequest,
            WINHTTP_NO_ADDITIONAL_HEADERS,
            0,
            WINHTTP_NO_REQUEST_DATA,
            0,
            0,
            0);

    if (bResults)
        bResults = WinHttpReceiveResponse(hRequest, NULL);

    if (bResults)
        do
        {
            dwSize = 0;
            if (!WinHttpQueryDataAvailable(hRequest,
                &dwSize))
                printf("Error %u in WinHttpQueryDataAvailable.\n", GetLastError());

            pszOutBuffer = new char[dwSize + 1];
            if (!pszOutBuffer)
            {
                printf("out of memory\n");
                dwSize = 0;
            }
            else
            {
                ZeroMemory(pszOutBuffer, dwSize + 1);
                if (!WinHttpReadData(hRequest,
                    (LPVOID)pszOutBuffer,
                    dwSize,
                    &dwDownloaded))
                    printf("Error %u in WinHttpReadData.\n", GetLastError());
                else
                    printf("%s\n", pszOutBuffer);
            }
        } while (dwSize > 0);
        
        if (!bResults)
            printf("Error %d has occurred.\n", GetLastError());

        if (hRequest) WinHttpCloseHandle(hRequest);
        if (hConnect) WinHttpCloseHandle(hConnect);
        if (hSession) WinHttpCloseHandle(hSession);
}
