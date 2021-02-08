#pragma once
#ifndef UNICODE
#define UNICODE
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x600
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include "C:\gitroot\flybynight\projects\HTTPAPI\HttpAPITest\SecurityUtility.cpp"

#ifdef USEHTTPP
#include <httpp.h>
#else
#include <http.h>
#pragma comment(lib, "httpapi.lib")
#endif

#include <stdio.h>

#define INITIALIZE_HTTP_RESPONSE( resp, status, reason) \
	do  \
	{	\
		RtlZeroMemory( (resp), sizeof(*(resp))); \
		(resp)->StatusCode = (status); \
		(resp)->pReason = (reason); \
		(resp)->ReasonLength = (USHORT) strlen(reason); \
	} while (FALSE)	\

#define ADD_KNOWN_HEADER(Response, HeaderId, RawValue) \
	do \
	{	\
		(Response).Headers.KnownHeaders[(HeaderId)].pRawValue = (RawValue); \
		(Response).Headers.KnownHeaders[(HeaderId)].RawValueLength = (USHORT)strlen(RawValue);	\
	} while (FALSE)	\

#define ALLOC_MEM(cb) HeapAlloc(GetProcessHeap(), 0, (cb))
#define FREE_MEM(ptr) HeapFree(GetProcessHeap(), 0, (ptr))

DWORD DoReceiveRequests(IN HANDLE hReqQueue);

DWORD
SendHttpResponse(
    IN HANDLE hReqQueue,
    IN PHTTP_REQUEST pRequest,
    IN USHORT StatusCode,
    IN PCHAR pReason,
    IN PCHAR pEntityString
);

DWORD
SendHttpPostResponse(
    IN HANDLE hReqQueue,
    IN PHTTP_REQUEST pRequest
);

int __cdecl wmain(
    int argc,
    wchar_t* argv[]
)
{
    ULONG retCode;

    HANDLE hReqQueue = NULL;
    HTTP_SERVER_SESSION_ID httpSessionId = NULL;
    HTTP_URL_GROUP_ID urlGroupId = NULL;

    int UrlAdded = 0;

    HTTPAPI_VERSION HttpApiVersion = {};
    HttpApiVersion = HTTPAPI_VERSION_2;

    HTTP_STATE_INFO StateInfo;
    StateInfo.Flags.Present = 1;
    StateInfo.State = HttpEnabledStateActive;
    ULONG stateInfoSize = sizeof(StateInfo);

    HTTP_STATE_INFO ConfigGroupState;
    HTTP_ENABLED_STATE HttpReason = HTTP_ENABLED_STATE::HttpEnabledStateActive;
    HTTP_BINDING_INFO BindingInfo;
    
    if (argc < 2)
    {
        wprintf(L"Usage: %ws: <Url1> [Url2] ... \n\n", argv[0]);
        wprintf(L"Ex.) Run\n");
        wprintf(L"%ws http://localhost:1234/\n", argv[0]);
        wprintf(L"And then send a request with http://localhost:1234/\n");
        return -1;
    }

    //
    // STEP 1: Initialize HTTP Server APIs
    //
    retCode = HttpInitialize(HttpApiVersion,
        HTTP_INITIALIZE_SERVER, // Flags 
        NULL); //Reserved

    if (retCode != NO_ERROR)
    {
        wprintf(L"HttpInitialize failed with %lu \n", retCode);
        return retCode;
    }

    //
    // STEP 2: Create RequestQueue and enable it (Enabling is optional because it is enabled by default)
    //
    SECURITY_ATTRIBUTES sa;
    if (!SecurityUtility::CreateSecurityAttributes((LPWSTR) L"builtin\\administrator", &sa))
    {
        wprintf(L"Can't create SA\n");
        return -1;
    }

    retCode = HttpCreateRequestQueue(HttpApiVersion,
        L"JHKIMQueue3",
        &sa,
        0, // BUGBUG: I can't use HTTP_CREATE_REQUEST_QUEUE_FLAG_CONTROLLER for this parameter.
        &hReqQueue);  // Req Queue

    if (retCode != NO_ERROR)
    {
        wprintf(L"HttpCreateRequestQueue failed with %lu \n", retCode);
        goto CleanUp;
    }

    retCode = HttpSetRequestQueueProperty(hReqQueue,
        HttpServerStateProperty,
        reinterpret_cast <VOID*> (&HttpReason),
        sizeof(HTTP_ENABLED_STATE),
        0,
        NULL);

    if (retCode != NO_ERROR)
    {
        wprintf(L"HttpSetRequestQueueProperty failed with %lu\n", retCode);
        goto CleanUp;
    }

    //
    // STEP 3: Create ServerSession and initialize
    //
    retCode = HttpCreateServerSession(HttpApiVersion,
        &httpSessionId,
        0);

    if (retCode != NO_ERROR)
    {
        wprintf(L"HttpCreateServerSession failed with %lu\n", retCode);
        goto CleanUp;
    }

    retCode = HttpSetServerSessionProperty(
        httpSessionId,                 // control channel
        HttpServerStateProperty,       // information class
        &StateInfo,                    // data to set
        stateInfoSize                  // data length
    );

    if (retCode != NO_ERROR)
    {
        wprintf(L"HttpSetServerSessionProperty failed with %lu \n", retCode);
        goto CleanUp;
    }

    //
    // STEP 4: Create UrlGroup with the session and bind to the request queue and then enable (Enabling is optional because it is enabled by default)
    //
    
    // STEP 4.1 Create UrlGroup
    retCode = HttpCreateUrlGroup(httpSessionId,
        &urlGroupId,
        NULL);

    if (retCode != NO_ERROR)
    {
        wprintf(L"HttpCreateUrlGroup failed with %lu\n", retCode);
        goto CleanUp;
    }

    // STEP 4.2 Bind to the request queue
    SecureZeroMemory(&BindingInfo, sizeof(BindingInfo));
    BindingInfo.Flags.Present = 1;
    BindingInfo.RequestQueueHandle = hReqQueue;

    retCode = HttpSetUrlGroupProperty(urlGroupId,
        HttpServerBindingProperty,
        &BindingInfo,
        sizeof(HTTP_BINDING_INFO));

    if (retCode != NO_ERROR)
    {
        wprintf(L"HttpSetUrlGroupProperty failed with %lu\n", retCode);
        goto CleanUp;
    }

    // STEP 4.3 Initialize UrlGroup with adding multipe Urls
    for (int i = 1; i < argc; i++)
    {
        wprintf(L"listening for requests on the following url: %s\n", argv[i]);
        retCode = HttpAddUrlToUrlGroup(
            urlGroupId,
            argv[i], // Fully qualified URL
            NULL, // URL context
            NULL  // Reserved
        );

        if (retCode != NO_ERROR)
        {
            wprintf(L"HttpAddUrlToUrlGroup failed with %lu\n", retCode);
            goto CleanUp;
        }
        // Track the currently added URLS.
        UrlAdded++;
    }

    // STEP 4.4 Enable UrlGroup
    ConfigGroupState.Flags.Present = 1;
    ConfigGroupState.State = HttpEnabledStateActive;
    retCode = HttpSetUrlGroupProperty(
        urlGroupId,                      // config group ID
        HttpServerStateProperty,         // information class
        &ConfigGroupState,               // data to set
        sizeof(HTTP_STATE_INFO)          // data length
    );

    if (retCode != NO_ERROR)
    {
        wprintf(L"HttpSetUrlGroupProperty failed with %lu\n", retCode);
        goto CleanUp;
    }

    //
    // STEP 5: Start Receiving requests
    //
    DoReceiveRequests(hReqQueue);
    
CleanUp:

    // Call HttpRemoveUrl for all added URLs
    if (urlGroupId)
    {
        HttpCloseUrlGroup(urlGroupId);
        urlGroupId = NULL;
    }

    if (httpSessionId)
    {
        HttpCloseServerSession(httpSessionId);
        httpSessionId = NULL;
    }

    if (hReqQueue)
    {
        HttpCloseRequestQueue(hReqQueue);
        hReqQueue = NULL;
    }

    HttpTerminate(HTTP_INITIALIZE_SERVER, NULL);
    return retCode;
}

DWORD DoReceiveRequests(
    IN HANDLE hReqQueue
)
{
    ULONG	result;
    HTTP_REQUEST_ID requestId;
    DWORD bytesRead;
    PHTTP_REQUEST pRequest;
    PCHAR pRequestBuffer;
    ULONG RequestBufferLength;

    // Allocate a 2 KB buffer. This size should work for most
    // requests. The buffer size can be increased if required. Space
    // is also required for an HTTP_REQUEST structure.

    RequestBufferLength = sizeof(HTTP_REQUEST) + 2048;
    pRequestBuffer = (PCHAR)ALLOC_MEM(RequestBufferLength);

    if (pRequestBuffer == NULL)
    {
        return ERROR_NOT_ENOUGH_MEMORY;
    }

    pRequest = (PHTTP_REQUEST)pRequestBuffer;

    // Wait for a new request. This is indicated by a NULL request ID.
    HTTP_SET_NULL_ID(&requestId);

    for (;;)
    {
        RtlZeroMemory(pRequest, RequestBufferLength);

        result = HttpReceiveHttpRequest(hReqQueue,  // Req Queue
            requestId,                              // Req ID
            0, //HTTP_RECEIVE_REQUEST_FLAG_COPY_BODY,    // Flags
            pRequest,                               // HTTP request buffer
            RequestBufferLength,                    // req buffer length
            &bytesRead,                             // bytes received
            NULL);                                  // LPOVERLAPPED

#ifdef USEHTTPP
/*
        HttpDelegateRequestEx(
            hReqQueue,
            hReqQueue,
            requestId,
            0, //    _In_ HTTP_URL_GROUP_ID DelegateUrlGroupId,
            0, //_In_ ULONG PropertyInfoSetSize,
            0 // _In_ PHTTP_DELEGATE_REQUEST_PROPERTY_INFO PropertyInfoSet
            );
*/
#endif        

        if (NO_ERROR == result)
        {
            // Worked!
            switch (pRequest->Verb)
            {
            case HttpVerbGET:
                wprintf(L"Got a GET request for %ws \n",
                    pRequest->CookedUrl.pFullUrl);

                result = SendHttpResponse(
                    hReqQueue,
                    pRequest,
                    200,
                    (PCHAR) "OK",
                    (PCHAR) "Hey! You hit the server \r\n"
                );
                break;

            case HttpVerbPOST:
                wprintf(L"Got a POST request for %ws \n",
                    pRequest->CookedUrl.pFullUrl);

                result = SendHttpPostResponse(hReqQueue, pRequest);
                break;

            default:
                wprintf(L"Got a unknown request for %ws\n",
                    pRequest->CookedUrl.pFullUrl);

                result = SendHttpResponse(
                    hReqQueue,
                    pRequest,
                    503,
                    (PCHAR) "Not Impelmented",
                    NULL
                );
                break;
            }

            if (result != NO_ERROR)
            {
                break;
            }

            // Reset the Request ID to handle the next request.
            HTTP_SET_NULL_ID(&requestId);
        }
        else if (result == ERROR_MORE_DATA)
        {
            // The input buffer was too small to hold the request
            // headers. Increase the buffer size and call the API again
            // When calling the API again, handle the request
            // that failed by passing a RequestID.
            // This RequestID is read from the old buffer.
            requestId = pRequest->RequestId;

            // Free the old buffer and allocate a new buffer.
            RequestBufferLength = bytesRead;
            FREE_MEM(pRequestBuffer);
            pRequestBuffer = (PCHAR)ALLOC_MEM(RequestBufferLength);

            if (pRequestBuffer == NULL)
            {
                result = ERROR_NOT_ENOUGH_MEMORY;
                break;
            }

            pRequest = (PHTTP_REQUEST)pRequestBuffer;
        }
        else if (ERROR_CONNECTION_INVALID == result && !HTTP_IS_NULL_ID(&requestId))
        {
            // The TCP connection was corrupted by the peer when
            // attempting to handle a request with more buffer.
            // Continue to the next request.

            HTTP_SET_NULL_ID(&requestId);
        }
        else
        {
            break;
        }
    }

    if (pRequestBuffer)
    {
        FREE_MEM(pRequestBuffer);
    }

    return result;
}

/*
Routine Descrption:
    The routine sends a HTTP response

Arguments:
    hReqQueue - Handle to the request queue
    pRequest - The parsed HTTP request
    StatusCode - Response Status Code
    pReason - Response reason phrase
    pEntityString - Response entity body

Return Value:
    Success/Failure.
*/
DWORD
SendHttpResponse(
    IN HANDLE hReqQueue,
    IN PHTTP_REQUEST pRequest,
    IN USHORT StatusCode,
    IN PCHAR pReason,
    IN PCHAR pEntityString
)
{
    HTTP_RESPONSE response;
    HTTP_DATA_CHUNK dataChunk;
    DWORD result;
    DWORD bytesSent;

    // Initialize the HTTP response structure.
    INITIALIZE_HTTP_RESPONSE(&response, StatusCode, pReason);

    // Add a known header
    ADD_KNOWN_HEADER(response, HttpHeaderContentType, "text/html");

    if (pEntityString)
    {
        // Add an entity chunk.
        dataChunk.DataChunkType = HttpDataChunkFromMemory;
        dataChunk.FromMemory.pBuffer = pEntityString;
        dataChunk.FromMemory.BufferLength =
            (ULONG)strlen(pEntityString);

        response.EntityChunkCount = 1;
        response.pEntityChunks = &dataChunk;
    }

    // Because the entity body is sent in one call, it is not
    // required to specify the Content-Length.

    result = HttpSendHttpResponse(
        hReqQueue, // ReqQueueHandle
        pRequest->RequestId, // Request ID
        0, // Flags
        &response, // HTTP response
        NULL, // pReserved1
        &bytesSent, // bytes sent (OPTIONAL)
        NULL, // pReserved2
        0, // Reserved3
        NULL, // LPOVERLAPPED (OPTIONAL)
        NULL // pReserved4 (must be NULL)
    );

    if (result != NO_ERROR)
    {
        wprintf(L"HttpsendHttpResponse failed with %lu\n", result);
    }

    return result;
}

#define MAX_ULONG_STR ((ULONG) sizeof("4294967295"))
/*
Routine Descrption:
    The routine sends a HTTP response after reading the entity body.

Arguments:
    hReqQueue - Handle to the request queue.
    pRequest - The parsed HTTP request.

Return value:
    Success/Failure.
*/
DWORD
SendHttpPostResponse(
    IN HANDLE hReqQueue,
    IN PHTTP_REQUEST pRequest
)
{
    HTTP_RESPONSE response;
    DWORD result;
    DWORD bytesSent;
    PUCHAR pEntityBuffer;
    ULONG EntityBufferLength;
    ULONG BytesRead;
    ULONG TempFileBytesWritten;
    HANDLE hTempFile;
    TCHAR szTempName[MAX_PATH + 1];
    CHAR szContentLength[MAX_ULONG_STR];
    HTTP_DATA_CHUNK dataChunk;
    ULONG TotalBytesRead = 0;

    BytesRead = 0;
    hTempFile = INVALID_HANDLE_VALUE;

    // Allocate space for an entity buffer. Buffer can be increased
    // on demand.
    EntityBufferLength = 2048;
    pEntityBuffer = (PUCHAR)ALLOC_MEM(EntityBufferLength);

    if (pEntityBuffer == NULL)
    {
        result = ERROR_NOT_ENOUGH_MEMORY;
        wprintf(L"Insufficient resources\n");
        goto Done;
    }

    // Intialize the HTTP response structure.
    INITIALIZE_HTTP_RESPONSE(&response, 200, "OK");

    // For POST, echo back the entity from the client
    // NOTE: If the HTTP_RECEIVE_REQUEST_FLAG_COPY_BODY flag had been
    //	passed with HttpReceivedHttpRequest(), the entity would
    //	have been a part of HTTP_REQUEST (using the pEntityChunks
    //	field). Because that flag was not passed, there are no 
    //	o entity bodies in HTTP_REQUEST.

    if (pRequest->Flags & HTTP_REQUEST_FLAG_MORE_ENTITY_BODY_EXISTS)
    {
        // The entity body is sent over multiple calls. Collect
        // these in a file and send back. Create a temporary
        // file.

        if (GetTempFileName(
            L".",
            L"New",
            0,
            szTempName
        ) == 0)
        {
            result = GetLastError();
            wprintf(L"GetTempFileName failed with %lu\n", result);
            goto Done;
        }

        hTempFile = CreateFile(
            szTempName,
            GENERIC_READ | GENERIC_WRITE,
            0, // Do not share
            NULL, // No security descriptor.
            CREATE_ALWAYS, // Overwrite existing.
            FILE_ATTRIBUTE_NORMAL, // Normal file.
            NULL
        );

        if (hTempFile == INVALID_HANDLE_VALUE)
        {
            result = GetLastError();
            wprintf(L"Cannot create temporary fle. Error %lu\n", result);
            goto Done;
        }

        do
        {
            // Read the entity chunk from the request.
            BytesRead = 0;
            result = HttpReceiveRequestEntityBody(
                hReqQueue,
                pRequest->RequestId,
                0,
                pEntityBuffer,
                EntityBufferLength,
                &BytesRead,
                NULL
            );

            switch (result)
            {
            case NO_ERROR:

                if (BytesRead != 0)
                {
                    TotalBytesRead += BytesRead;
                    WriteFile(
                        hTempFile,
                        pEntityBuffer,
                        BytesRead,
                        &TempFileBytesWritten,
                        NULL
                    );
                }
                break;

            case ERROR_HANDLE_EOF:

                // The last request entity body has been read.
                // Send back a response.
                // To illustrate entity sends via 
                // HttpSendResponseEntityBody, the response will
                // be sent over multiple calls. To do this,
                // pass the HTTP_SEND_RESPONSE_FLAG_MORE_DATA flag.

                if (BytesRead != 0)
                {
                    TotalBytesRead += BytesRead;
                    WriteFile(
                        hTempFile,
                        pEntityBuffer,
                        BytesRead,
                        &TempFileBytesWritten,
                        NULL
                    );

                    // Because the response is sent over multiple
                    // API calls, add a content-length.
                    // Alternatively, the response could have been
                    // sent using chunked transfer encoding, by
                    // passing "Transfer-Encoding: Chunked".

                    // NOTE: Because the TotalBytesRead in a ULONG
                    //	are accumulated, this will not work
                    //	for entity bodies larger than 4 GB.
                    //	For support of large entity bodies,
                    //	use a ULONGLONG.

                    sprintf_s(szContentLength, MAX_ULONG_STR, "%lu", TotalBytesRead);

                    ADD_KNOWN_HEADER(
                        response,
                        HttpHeaderContentLength,
                        szContentLength
                    );

                    result =
                        HttpSendHttpResponse(
                            hReqQueue, // Request Queue Handle
                            pRequest->RequestId, // Request ID
                            HTTP_SEND_RESPONSE_FLAG_MORE_DATA,
                            &response, // HTTP response
                            NULL, // pReserved1
                            &bytesSent, // bytes sent (Optional)
                            NULL, // pReserved2
                            0, // pReserved3
                            NULL, // LPOVRLAPPED
                            NULL // pReserved4
                        );

                    if (result != NO_ERROR)
                    {
                        wprintf(
                            L"HttpSendHttpResponse failed with %lu \n",
                            result
                        );
                        goto Done;
                    }

                    // Send entity body from a file handle.
                    dataChunk.DataChunkType = HttpDataChunkFromFileHandle;
                    dataChunk.FromFileHandle.ByteRange.StartingOffset.QuadPart = 0;
                    dataChunk.FromFileHandle.ByteRange.Length.QuadPart = HTTP_BYTE_RANGE_TO_EOF;
                    dataChunk.FromFileHandle.FileHandle = hTempFile;

                    result = HttpSendResponseEntityBody(
                        hReqQueue,
                        pRequest->RequestId,
                        0, // This is the last send.
                        1, // Entity Chunk Count
                        &dataChunk,
                        NULL,
                        NULL,
                        0,
                        NULL,
                        NULL
                    );

                    if (result != NO_ERROR)
                    {
                        wprintf(L"HttpSendResponseEntityBody failed %lu\n", result);
                    }

                    goto Done;
                    break;
                }

            default:
                wprintf(
                    L"HttpReceiveRequestEntityBody failed with %lu\n",
                    result);
                goto Done;
            }
        } while (TRUE);
    }
    else
    {
        // This request does not have an entity body.

        result = HttpSendHttpResponse(
            hReqQueue, // ReqQueue Handle
            pRequest->RequestId, // Request ID
            0,
            &response, // HTTP response
            NULL, // pReserved1
            &bytesSent, // bytes sent (Optional)
            NULL, // pReserved2
            0, // pReserved3
            NULL, // LPOVERLAPPED
            NULL // pReserved
        );

        if (result != NO_ERROR)
        {
            wprintf(L"HttpSendHttpResponse failed with %lu\n", result);
        }
    }

Done:

    if (pEntityBuffer)
    {
        FREE_MEM(pEntityBuffer);
    }

    if (INVALID_HANDLE_VALUE != hTempFile)
    {
        CloseHandle(hTempFile);
        DeleteFile(szTempName);
    }
    return result;
}
