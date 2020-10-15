// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved

#include <Windows.h>
#include <WinHttp.h>
#include <stdio.h>

VOID
CALLBACK
OnWinHttpCompletion(
	HINTERNET   hRequest,
	DWORD_PTR   dwContext,
	DWORD       dwInternetStatus,
	LPVOID      lpvStatusInformation,
	DWORD       dwStatusInformationLength
)
{
	switch (dwInternetStatus)
	{
	case WINHTTP_CALLBACK_STATUS_RESOLVING_NAME:
		wprintf(L"WINHTTP_CALLBACK_STATUS_RESOLVING_NAME\n");
		break;
	case WINHTTP_CALLBACK_STATUS_NAME_RESOLVED:
		wprintf(L"WINHTTP_CALLBACK_STATUS_NAME_RESOLVED\n");
		break;
	case WINHTTP_CALLBACK_STATUS_CONNECTING_TO_SERVER:
		wprintf(L"WINHTTP_CALLBACK_STATUS_CONNECTING_TO_SERVER\n");
		break;
	case WINHTTP_CALLBACK_STATUS_CONNECTED_TO_SERVER:
		wprintf(L"WINHTTP_CALLBACK_STATUS_CONNECTED_TO_SERVER\n");
		break;
	case WINHTTP_CALLBACK_STATUS_SENDING_REQUEST:
		wprintf(L"WINHTTP_CALLBACK_STATUS_SENDING_REQUEST\n");
		break;
	case WINHTTP_CALLBACK_STATUS_REQUEST_SENT:
		wprintf(L"WINHTTP_CALLBACK_STATUS_REQUEST_SENT\n");
		break;
	case WINHTTP_CALLBACK_STATUS_RECEIVING_RESPONSE:
		wprintf(L"WINHTTP_CALLBACK_STATUS_RECEIVING_RESPONSE\n");
		break;
	case WINHTTP_CALLBACK_STATUS_RESPONSE_RECEIVED:
		wprintf(L"WINHTTP_CALLBACK_STATUS_RESPONSE_RECEIVED\n");
		break;
	case WINHTTP_CALLBACK_STATUS_CLOSING_CONNECTION:
		wprintf(L"WINHTTP_CALLBACK_STATUS_CLOSING_CONNECTION\n");
		break;
	case WINHTTP_CALLBACK_STATUS_CONNECTION_CLOSED:
		wprintf(L"WINHTTP_CALLBACK_STATUS_CONNECTION_CLOSED\n");
		break;
	case WINHTTP_CALLBACK_STATUS_HANDLE_CREATED:
		wprintf(L"WINHTTP_CALLBACK_STATUS_HANDLE_CREATED\n");
		break;
	case WINHTTP_CALLBACK_STATUS_HANDLE_CLOSING:
		wprintf(L"WINHTTP_CALLBACK_STATUS_HANDLE_CLOSING\n");
		break;
	case WINHTTP_CALLBACK_STATUS_DETECTING_PROXY:
		wprintf(L"WINHTTP_CALLBACK_STATUS_DETECTING_PROXY\n");
		break;
	case WINHTTP_CALLBACK_STATUS_REDIRECT:
		wprintf(L"WINHTTP_CALLBACK_STATUS_REDIRECT\n");
		break;
	case WINHTTP_CALLBACK_STATUS_INTERMEDIATE_RESPONSE:
		wprintf(L"WINHTTP_CALLBACK_STATUS_INTERMEDIATE_RESPONSE\n");
		break;
	case WINHTTP_CALLBACK_STATUS_SECURE_FAILURE:
		wprintf(L"WINHTTP_CALLBACK_STATUS_SECURE_FAILURE\n");
		break;
	case WINHTTP_CALLBACK_STATUS_HEADERS_AVAILABLE:
		wprintf(L"WINHTTP_CALLBACK_STATUS_HEADERS_AVAILABLE\n");
		break;
	case WINHTTP_CALLBACK_STATUS_DATA_AVAILABLE:
		wprintf(L"WINHTTP_CALLBACK_STATUS_DATA_AVAILABLE\n");
		break;
	case WINHTTP_CALLBACK_STATUS_READ_COMPLETE:
		wprintf(L"WINHTTP_CALLBACK_STATUS_READ_COMPLETE\n");
		break;
	case WINHTTP_CALLBACK_STATUS_WRITE_COMPLETE:
		wprintf(L"WINHTTP_CALLBACK_STATUS_WRITE_COMPLETE\n");
		break;
	case WINHTTP_CALLBACK_STATUS_REQUEST_ERROR:
		wprintf(L"WINHTTP_CALLBACK_STATUS_REQUEST_ERROR\n");
		break;
	case WINHTTP_CALLBACK_STATUS_SENDREQUEST_COMPLETE:
		wprintf(L"WINHTTP_CALLBACK_STATUS_SENDREQUEST_COMPLETE\n");
		break;
	case WINHTTP_CALLBACK_STATUS_GETPROXYFORURL_COMPLETE:
		wprintf(L"WINHTTP_CALLBACK_STATUS_GETPROXYFORURL_COMPLETE\n");
		break;
	case WINHTTP_CALLBACK_STATUS_CLOSE_COMPLETE:
		wprintf(L"WINHTTP_CALLBACK_STATUS_CLOSE_COMPLETE\n");
		break;
	case WINHTTP_CALLBACK_STATUS_SHUTDOWN_COMPLETE:
		wprintf(L"WINHTTP_CALLBACK_STATUS_SHUTDOWN_COMPLETE\n");
		break;
	case WINHTTP_CALLBACK_STATUS_SETTINGS_WRITE_COMPLETE:
		wprintf(L"WINHTTP_CALLBACK_STATUS_SETTINGS_WRITE_COMPLETE\n");
		break;
	case WINHTTP_CALLBACK_STATUS_SETTINGS_READ_COMPLETE:
		wprintf(L"WINHTTP_CALLBACK_STATUS_SETTINGS_READ_COMPLETE\n");
		break;
	default:
		wprintf(L"WINHTTP_Default\n");
		break;
	}	
}

int __cdecl wmain()
{
    DWORD dwError = ERROR_SUCCESS;
    BOOL fStatus = FALSE;
    HINTERNET hSessionHandle = NULL;
    HINTERNET hConnectionHandle = NULL;
    HINTERNET hRequestHandle = NULL;
	WINHTTP_STATUS_CALLBACK isCallback = NULL;
    HINTERNET hWebSocketHandle = NULL;
    BYTE rgbCloseReasonBuffer[123];
    BYTE rgbBuffer[1024];
    BYTE *pbCurrentBufferPointer = rgbBuffer;
    DWORD dwBufferLength = ARRAYSIZE(rgbBuffer);
    DWORD dwBytesTransferred = 0;
    DWORD dwCloseReasonLength = 0;
    USHORT usStatus = 0;
    WINHTTP_WEB_SOCKET_BUFFER_TYPE eBufferType;
    INTERNET_PORT Port = INTERNET_DEFAULT_HTTP_PORT;
    const WCHAR *pcwszServerName = L"localhost";
    //const WCHAR *pcwszPath = L"/websocket/echohandler.ashx";
	const WCHAR *pcwszPath = L"/aspnetcoreapp/websocket";
    const WCHAR *pcwszMessage = L"Hello world";
    const DWORD cdwMessageLength = ARRAYSIZE(L"Hello world") * sizeof(WCHAR);

    //
    // Create session, connection and request handles.
    //

    hSessionHandle = WinHttpOpen(L"WebSocket sample",
                                 WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
                                 NULL,
                                 NULL,
                                 0);
    if (hSessionHandle == NULL)
    {
        dwError = GetLastError();
        goto quit;
    }

    hConnectionHandle = WinHttpConnect(hSessionHandle,
                                       pcwszServerName,
                                       Port,
                                       0);
    if (hConnectionHandle == NULL)
    {
        dwError = GetLastError();
        goto quit;
    }

    hRequestHandle = WinHttpOpenRequest(hConnectionHandle,
                                        L"GET",
                                        pcwszPath,
                                        NULL,
                                        NULL,
                                        NULL,
                                        0);
    if (hRequestHandle == NULL)
    {
        dwError = GetLastError();
        goto quit;
    }

	isCallback = WinHttpSetStatusCallback(hRequestHandle,
		  								   OnWinHttpCompletion,
		                                   WINHTTP_CALLBACK_FLAG_ALL_NOTIFICATIONS,
										   //(WINHTTP_CALLBACK_FLAG_ALL_COMPLETIONS | WINHTTP_CALLBACK_FLAG_HANDLES | WINHTTP_CALLBACK_STATUS_SENDING_REQUEST),
										   NULL);
    if (isCallback == WINHTTP_INVALID_STATUS_CALLBACK)
	{
		dwError = GetLastError();
		goto quit;
	}

    //
    // Request protocol upgrade from http to websocket.
    //
#pragma prefast(suppress:6387, "WINHTTP_OPTION_UPGRADE_TO_WEB_SOCKET does not take any arguments.")
    fStatus = WinHttpSetOption(hRequestHandle,
                               WINHTTP_OPTION_UPGRADE_TO_WEB_SOCKET,
                               NULL,
                               0);
    if (!fStatus)
    {
        dwError = GetLastError();
        goto quit;
    }

    //
    // Perform websocket handshake by sending a request and receiving server's response.
    // Application may specify additional headers if needed.
    //

    fStatus = WinHttpSendRequest(hRequestHandle,
                                 WINHTTP_NO_ADDITIONAL_HEADERS,
                                 0,
                                 NULL,
                                 0,
                                 0,
                                 0);
    if (!fStatus)
    {
        dwError = GetLastError();
        goto quit;
    }

    fStatus = WinHttpReceiveResponse(hRequestHandle, 0);
    if (!fStatus)
    {
        dwError = GetLastError();
        goto quit;
    }

    //
    // Application should check what is the HTTP status code returned by the server and behave accordingly.
    // WinHttpWebSocketCompleteUpgrade will fail if the HTTP status code is different than 101.
    //

    hWebSocketHandle = WinHttpWebSocketCompleteUpgrade(hRequestHandle, NULL);
    if (hWebSocketHandle == NULL)
    {
        dwError = GetLastError();
        goto quit;
    }

    //
    // The request handle is not needed anymore. From now on we will use the websocket handle.
    //

    WinHttpCloseHandle(hRequestHandle);
    hRequestHandle = NULL;

    wprintf(L"Succesfully upgraded to websocket protocol\n");

    //
    // Send and receive data on the websocket protocol.
    //

    dwError = WinHttpWebSocketSend(hWebSocketHandle,
                                   WINHTTP_WEB_SOCKET_BINARY_MESSAGE_BUFFER_TYPE,
                                   (PVOID)pcwszMessage,
                                   cdwMessageLength);
    if (dwError != ERROR_SUCCESS)
    {
        goto quit;
    }

    wprintf(L"Sent message to the server: '%s'\n", pcwszMessage);

    do
    {
        if (dwBufferLength == 0)
        {
            dwError = ERROR_NOT_ENOUGH_MEMORY;
            goto quit;
        }

        dwError = WinHttpWebSocketReceive(hWebSocketHandle,
                                          pbCurrentBufferPointer,
                                          dwBufferLength,
                                          &dwBytesTransferred,
                                          &eBufferType);
        if (dwError != ERROR_SUCCESS)
        {
            goto quit;
        }

        //
        // If we receive just part of the message restart the receive operation.
        //

        pbCurrentBufferPointer += dwBytesTransferred;
        dwBufferLength -= dwBytesTransferred;
    }
    while (eBufferType == WINHTTP_WEB_SOCKET_BINARY_FRAGMENT_BUFFER_TYPE);

    //
    // We expected server just to echo single binary message.
    //

    if (eBufferType != WINHTTP_WEB_SOCKET_BINARY_MESSAGE_BUFFER_TYPE)
    {
        wprintf(L"Unexpected buffer type\n");
        dwError = ERROR_INVALID_PARAMETER;
        goto quit;
    }

    wprintf(L"Received message from the server: '%.*s'\n", dwBufferLength, (WCHAR*)rgbBuffer);

    //
    // Gracefully close the connection.
    //

    dwError = WinHttpWebSocketClose(hWebSocketHandle,
                                    WINHTTP_WEB_SOCKET_SUCCESS_CLOSE_STATUS,
                                    NULL,
                                    0);
    if (dwError != ERROR_SUCCESS)
    {
        goto quit;
    }

    //
    // Check close status returned by the server.
    //

    dwError = WinHttpWebSocketQueryCloseStatus(hWebSocketHandle,
                                               &usStatus,
                                               rgbCloseReasonBuffer,
                                               ARRAYSIZE(rgbCloseReasonBuffer),
                                               &dwCloseReasonLength);
    if (dwError != ERROR_SUCCESS)
    {
        goto quit;
    }

    wprintf(L"The server closed the connection with status code: '%d' and reason: '%.*S'\n",
            (int)usStatus,
            dwCloseReasonLength,
            rgbCloseReasonBuffer);

quit:

    if (hRequestHandle != NULL)
    {
        WinHttpCloseHandle(hRequestHandle);
        hRequestHandle = NULL;
    }

    if (hWebSocketHandle != NULL)
    {
        WinHttpCloseHandle(hWebSocketHandle);
        hWebSocketHandle = NULL;
    }

    if (hConnectionHandle != NULL)
    {
        WinHttpCloseHandle(hConnectionHandle);
        hConnectionHandle = NULL;
    }

    if (hSessionHandle != NULL)
    {
        WinHttpCloseHandle(hSessionHandle);
        hSessionHandle = NULL;
    }

    if (dwError != ERROR_SUCCESS)
    {
        wprintf(L"Application failed with error: %u\n", dwError);
        return -1;
    }

    return 0;
}
