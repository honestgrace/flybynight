#include "stdafx.h"

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <thread>
#include <iostream>
#include <string>

using namespace std;

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "80"
//#define DEFAULT_PORT "27015"

class TCPContext
{
public:
	enum Work_Mode
	{
		Intialize,
		Connect,
		Send,
		Receive,
		ShutdownSend,
		ReceiveLoop,
		Cleanup,
		Finished
	};

	Work_Mode mode = Work_Mode::Intialize;
	char pMethod[20];
	char pHost[256];
	char pUrl[256];
	char pPostData[256];
	SOCKET socket = INVALID_SOCKET;
	int result = 0;
	int signature = 0;
	int repeatCount = 2;
	int threadID = 0;

	BOOL TestOption_KEEPALIVE = TRUE;
	BOOL TestOption_REUSEADDR = TRUE;
	BOOL TestOption_LingerOn = TRUE;
	int TestOption_LingerValue = 0;

	BOOL TestOption_SendAndHardClose = FALSE;
	BOOL TestOption_SendOnly = FALSE;

	void printf(char const* const _Format, ...)
	{
		int _Result;
		va_list _ArgList;
		__crt_va_start(_ArgList, _Format);
		_Result = _vfprintf_s_l(stdout, _Format, NULL, _ArgList);
		__crt_va_end(_ArgList);		
	}
};

class HTTPConnection
{
public:
	static void Initialize_Task(TCPContext * pContext)
	{
		int iResult = 0;
		WSADATA wsaData;

		// Initialize Winsock
		iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != 0) 
		{
			pContext->printf("WSAStartup failed with error: %d\n", iResult);
		}
		pContext->result = iResult;
	}

	static void Cleanup_Task(TCPContext * pContext)
	{
		// cleanup
		closesocket(pContext->socket);
		WSACleanup();
	}
	
	static void Connect_Task(TCPContext * pContext)
	{
		pContext->socket = INVALID_SOCKET;
		struct addrinfo *result = NULL,
			*ptr = NULL,
			hints;
		int iResult = 0;

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		// Resolve the server address and port
		iResult = getaddrinfo(pContext->pHost, DEFAULT_PORT, &hints, &result);
		if (iResult != 0) 
		{
			pContext->printf("getaddrinfo failed with error: %d\n", iResult);
			WSACleanup();
			goto Finalize;
		}

		// Attempt to connect to an address until one succeeds
		for (ptr = result; ptr != NULL; ptr = ptr->ai_next) 
		{
			// Create a SOCKET for connecting to server
			pContext->socket = socket(ptr->ai_family, ptr->ai_socktype,
				ptr->ai_protocol);
			pContext->signature = 555;

			if (pContext->socket == INVALID_SOCKET)
			{
				pContext->printf("socket failed with error: %ld\n", WSAGetLastError());
				goto Finalize;
			}

			SetSocketOption_Task(pContext);

			// Connect to server.
			iResult = connect(pContext->socket, ptr->ai_addr, (int)ptr->ai_addrlen);
			if (iResult == SOCKET_ERROR) 
			{
				closesocket(pContext->socket);
				pContext->socket = INVALID_SOCKET;
				continue;
			}
			break;
		}

		freeaddrinfo(result);

Finalize:
		if (pContext->socket == INVALID_SOCKET)
		{
			pContext->printf("Unable to connect to server!\n");
			WSACleanup();
		}
	}

	static void SetSocketBooleanOption(TCPContext * pContext, int option, BOOL optionValue, string optionName)
	{
		int iResult = 0;

		BOOL bOptVal = FALSE;
		if (optionValue)
		{
			bOptVal = TRUE;
		}
		int iOptVal = 0;
		int iOptLen = sizeof(int);
		iResult = getsockopt(pContext->socket, SOL_SOCKET, option, (char *)&iOptVal, &iOptLen);
		if (iResult == SOCKET_ERROR)
		{
			pContext->printf("%s: getsockopt failed with error: %u\n", optionName.c_str(), WSAGetLastError());
		}
		else
		{
			pContext->printf("%s: current value: %ld\n", optionName.c_str(), iOptVal);
		}

		int bOptLen = sizeof(BOOL);
		iResult = setsockopt(pContext->socket, SOL_SOCKET, option, (char *)&bOptVal, bOptLen);
		if (iResult == SOCKET_ERROR) {
			pContext->printf("%s: setsockopt failed with error: %u\n", optionName.c_str(), WSAGetLastError());
		}
		else
		{
			pContext->printf("%s: new value : %ld\n", optionName.c_str(), optionValue);
		}
		pContext->result = iResult;
	}

	static void SetSocketOption_Task(TCPContext * pContext)
	{
		int iResult = 0;

		linger lin_out;
		int lin_out_len = sizeof(lin_out);
		iResult = getsockopt(pContext->socket, SOL_SOCKET, SO_LINGER, (char *)&lin_out, &lin_out_len);
		if (iResult == SOCKET_ERROR)
		{
			pContext->printf("getsockopt for SO_LINGER failed with error: %u\n", WSAGetLastError());
		}
		else
		{
			pContext->printf("LINGER current on/off: %ld\n", lin_out.l_onoff);
			pContext->printf("LINGER current value: %ld\n", lin_out.l_linger);
		}

		// set SO_LINGER true with 0 timeout in order to test HardClose
		// set SO_LINGER true with 0 timeout in order to test HardClose
		linger lin;
		unsigned int lin_len = sizeof(lin);
		lin.l_onoff = 0;
		if (pContext->TestOption_LingerOn)
		{
			lin.l_onoff = 1;
		}
		lin.l_linger = pContext->TestOption_LingerValue;

		iResult = setsockopt(pContext->socket, SOL_SOCKET, SO_LINGER, (const char*)(&lin), lin_len);
		if (iResult == SOCKET_ERROR) {
			pContext->printf("setsockopt for SO_LINGER failed with error: %u\n", WSAGetLastError());
		}
		else
		{
			pContext->printf("LINGER new on/off: %ld\n", lin.l_onoff);
			pContext->printf("LINGER new value: %ld\n", lin.l_linger);
		}

		pContext->result = iResult;

		if (pContext->result != SOCKET_ERROR)
		{
			SetSocketBooleanOption(pContext, SO_KEEPALIVE, pContext->TestOption_KEEPALIVE, string("KEEPALIVE"));
		}
		if (pContext->result != SOCKET_ERROR)
		{
			SetSocketBooleanOption(pContext, SO_REUSEADDR, pContext->TestOption_REUSEADDR, string("REUSEADDR"));
		}
	}

	static void ShutdownSend_Task(TCPContext * pContext)
	{
		int iResult = 0;

		// shutdown the connection since no more data will be sent
		iResult = shutdown(pContext->socket, SD_SEND);
		if (iResult == SOCKET_ERROR) 
		{
			pContext->printf("shutdown failed with error: %d\n", WSAGetLastError());
			closesocket(pContext->socket);
			WSACleanup();
		}
		pContext->result = iResult;
	}

	static void Send_Task(TCPContext * pContext)
	{
		int iResult = 0;
		const int MAXBUFFERLENGTH = 255;
		char sendbuf[MAXBUFFERLENGTH];
		string postRequestHeaders = "";
		if (pContext->pMethod[0] == 'P')
		{
			postRequestHeaders = "Content-Type: application/x-www-form-urlencoded\r\nContent-Length:";
			char contentLength[20];
			sprintf_s(contentLength, 20, "%d", strlen(pContext->pPostData));
			postRequestHeaders += contentLength; 
			postRequestHeaders += "\r\n";
		}
		sprintf_s(sendbuf, MAXBUFFERLENGTH, "%s %s HTTP/1.1\r\nHost:%s\r\n%s\r\n%s", pContext->pMethod, pContext->pUrl, pContext->pHost, postRequestHeaders.c_str(), pContext->pPostData);
		
		// Send an initial buffer
		iResult = send(pContext->socket, sendbuf, (int)strlen(sendbuf), 0);
		
		if (iResult == SOCKET_ERROR) 
		{
			pContext->printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(pContext->socket);
			WSACleanup();
		}
		pContext->printf("Bytes Sent: %ld\n", iResult);

		pContext->result = iResult;
	}

	static void Receive_Task(TCPContext * pContext)
	{
		int iResult = 0;
		int recvbuflen = DEFAULT_BUFLEN;
		char recvbuf[DEFAULT_BUFLEN];
		iResult = recv(pContext->socket, recvbuf, recvbuflen, 0);
		if (iResult > 0)
		{
			pContext->printf("Bytes received: %d\n", iResult);
		}
		else if (iResult == 0)
		{
			pContext->printf("Connection closed from server\n");
			iResult = SOCKET_ERROR;
		}
		else
		{
			pContext->printf("recv failed with error: %d\n", WSAGetLastError());
			iResult = SOCKET_ERROR;
		}
		if (iResult != DEFAULT_BUFLEN)
		{
			recvbuf[iResult] = '\0';
		}
		pContext->printf("%s\r\n", recvbuf);

		pContext->result = iResult;
	}

	static void ReceiveLoop_Task(TCPContext * pContext)
	{
		int iResult = 0;
		int recvbuflen = DEFAULT_BUFLEN;
		char recvbuf[DEFAULT_BUFLEN];
		// Receive until the peer closes the connection
		do {
			iResult = recv(pContext->socket, recvbuf, recvbuflen, 0);
			if (iResult > 0)
			{
				pContext->printf("Bytes received: %d\n", iResult);
			}
			else if (iResult == 0)
			{
				pContext->printf("Connection closed\n");
			}
			else
			{
				pContext->printf("recv failed with error: %d\n", WSAGetLastError());
			}
		} while (iResult > 0);

		pContext->result = iResult;
	}

	static void SendReceive(TCPContext *pContext)
	{
		switch (pContext->mode)
		{
		case TCPContext::Work_Mode::Intialize:
			Initialize_Task(pContext);
			if (pContext->mode == TCPContext::Work_Mode::Intialize)
			{
				if (pContext->result != 0)
				{
					pContext->mode = TCPContext::Work_Mode::Finished;
				}
				else
				{
					pContext->mode = TCPContext::Work_Mode::Connect;
				}
			}
			break;
		case TCPContext::Work_Mode::Connect:
			Connect_Task(pContext);
			if (pContext->mode == TCPContext::Work_Mode::Connect)
			{
				if (pContext->socket == INVALID_SOCKET)
				{
					pContext->mode = TCPContext::Work_Mode::Finished;
				}
				else
				{
					pContext->mode = TCPContext::Work_Mode::Send;
				}
			}
			break;
		case TCPContext::Work_Mode::Send:
			if (pContext->TestOption_SendAndHardClose || pContext->TestOption_SendOnly)
			{
				// hard closing scenario after sending request is done
				shutdown(pContext->socket, SD_RECEIVE);
				Send_Task(pContext);
				pContext->mode = TCPContext::Work_Mode::Cleanup;
			}
			else
			{
				Send_Task(pContext);
			}
			if (pContext->mode == TCPContext::Work_Mode::Send)
			{
				if (pContext->result == SOCKET_ERROR)
				{
					pContext->mode = TCPContext::Work_Mode::Finished;
				}
				else
				{
					pContext->mode = TCPContext::Work_Mode::Receive;
				}
			}
			break;
		case TCPContext::Work_Mode::Receive:
			Receive_Task(pContext);
			if (pContext->mode == TCPContext::Work_Mode::Receive)
			{
				pContext->mode = TCPContext::Work_Mode::ShutdownSend;
			}
			break;
		case TCPContext::Work_Mode::ShutdownSend:
			ShutdownSend_Task(pContext);
			if (pContext->mode == TCPContext::Work_Mode::ShutdownSend)
			{
				if (pContext->result == SOCKET_ERROR)
				{
					pContext->mode = TCPContext::Work_Mode::Finished;
				}
				else
				{
					pContext->mode = TCPContext::Work_Mode::ReceiveLoop;
				}
			}
			break;
		case TCPContext::Work_Mode::ReceiveLoop:
			ReceiveLoop_Task(pContext);
			if (pContext->mode == TCPContext::Work_Mode::ReceiveLoop)
			{
				pContext->mode = TCPContext::Work_Mode::Cleanup;
			}
			break;
		case TCPContext::Work_Mode::Cleanup:
			if (!pContext->TestOption_SendOnly)
			{
				Cleanup_Task(pContext);
			}
			pContext->printf("\r\n****\r\n");
			pContext->printf("ThreadID: %d RepeatCount: %d\r\n", pContext->threadID, pContext->repeatCount);
			pContext->printf("****\r\n");
			pContext->repeatCount--;
			if (pContext->mode == TCPContext::Work_Mode::Cleanup)
			{
				if (pContext->repeatCount <= 0)
				{
					pContext->mode = TCPContext::Work_Mode::Finished;
				}
				else
				{
					pContext->mode = TCPContext::Work_Mode::Intialize;
				}
			}
			break;
		case TCPContext::Work_Mode::Finished:
			break;
		default:
			break;
		}
	}

	static void DoWork(TCPContext * pContext)
	{
		while (pContext->mode != TCPContext::Work_Mode::Finished)
		{
			SendReceive(pContext);
		}
	}
};

char * GetParameterValue(char* pParamName, char** ppArgv, int argvLength)
{
	char * pValue = nullptr;
	for (int i = 0; i < argvLength; i++)
	{
		int index = -1;
		string tempString = ppArgv[i];
		index = tempString.find(pParamName);
		if (index >= 0)
		{
			if (strlen(pParamName) == strlen(ppArgv[i]))
			{
				pValue = ppArgv[i];
			}
			else
			{
				pValue = (ppArgv[i] + index + strlen(pParamName));
			}
			break;
		}
	}
	return pValue;
}

int TestWinSockClient(int argc, char **argv)
{
	// Validate the parameters
	if (argc == 1) {
		printf("usage: %s -h:localhost -rc:1 -tn:1 -url:/aspnetcoreapp/EchoPostData -method:POST -postData:x=XY TestWinSockClient\r\n", argv[0]);
		printf("Supported Parameters: -h: host\r\n-rc: repeatcount\r\n-tn: total thread number\r\n");
		return 1;
	}
	string method = "GET";
	string host = "localhost";
	string url = "/";
	string postData = "";
	int iRepeatCount = 1;
	int iThreadNumber = 1;
	BOOL testOption_SendAndHardClose = false;
	BOOL testOption_SendOnly = false;
	BOOL testOption_KeepAlive = true;
	BOOL testOption_ReUseAddr = true;
	BOOL testOption_LingerOn = false;
	int testOption_iLingerValue = 0;

	char *paramValue = nullptr;

	paramValue = GetParameterValue("-method:", argv, argc);
	if (paramValue != nullptr)
	{
		method = paramValue;
	}
	printf("Method: %s\n", method.c_str());

	paramValue = GetParameterValue("-h:", argv, argc);
	if (paramValue != nullptr)
	{
		host = paramValue;
	}
	printf("Host: %s\n", host.c_str());

	paramValue = GetParameterValue("-url:", argv, argc);
	if (paramValue != nullptr)
	{
		url = paramValue;
	}
	printf("Url: %s\n", url.c_str());

	paramValue = GetParameterValue("-postData:", argv, argc);
	if (paramValue != nullptr)
	{
		postData = paramValue;
	}
	printf("PostData: %s\n", postData.c_str());

	paramValue = GetParameterValue("-rc:", argv, argc);
	if (paramValue != nullptr)
	{
		string repeatCount = paramValue;
		iRepeatCount = stoi(repeatCount, nullptr, 0);
	}
	printf("RepeatCount: %d\n", iRepeatCount);

	paramValue = GetParameterValue("-tn:", argv, argc);
	if (paramValue != nullptr)
	{
		string threadNumber = paramValue;
		iThreadNumber = stoi(threadNumber, nullptr, 0);
	}
	printf("ThreadNumber: %d\n", iThreadNumber);

	printf("\n************\n");
	printf("Test Options\n");
	printf("************\n");
	paramValue = GetParameterValue("-SendAndHardClose", argv, argc);
	if (paramValue != nullptr)
	{
		testOption_SendAndHardClose = true;
		testOption_KeepAlive = false;
		testOption_LingerOn = true;
		testOption_iLingerValue = 0;
	}
	printf("SendAndHardClose: %d\n", testOption_SendAndHardClose);

	paramValue = GetParameterValue("-SendOnly", argv, argc);
	if (paramValue != nullptr)
	{
		testOption_SendOnly = true;
		testOption_KeepAlive = false;
		testOption_LingerOn = true;
		testOption_iLingerValue = 0;
	}
	printf("SendOnly: %d\n", testOption_SendOnly);

	printf("\n**************\n");
	printf("Socket Options\n");
	printf("**************\n");
	paramValue = GetParameterValue("-KeepAlive:", argv, argc);
	if (paramValue != nullptr)
	{
		if (paramValue[0] == 'n' || paramValue[0] == 'N')
		{
			testOption_KeepAlive = false;
		}
	}
	printf("KeepAlive: %d\n", testOption_SendAndHardClose);

	paramValue = GetParameterValue("-ReUseAddr:", argv, argc);
	if (paramValue != nullptr)
	{
		if (paramValue[0] == 'n' || paramValue[0] == 'N')
		{
			testOption_ReUseAddr = false;
		}
	}
	printf("ReUseAddr: %d\n", testOption_SendAndHardClose);

	paramValue = GetParameterValue("-LingerOn:", argv, argc);
	if (paramValue != nullptr)
	{
		if (paramValue[0] == 'n' || paramValue[0] == 'N')
		{
			testOption_LingerOn = false;
		}
	}
	printf("LingerOn: %d\n", testOption_SendAndHardClose);

	paramValue = GetParameterValue("-LingerValue:", argv, argc);
	if (paramValue != nullptr)
	{
		string lingerValue = paramValue;
		testOption_iLingerValue = stoi(lingerValue, nullptr, 0);
	}
	printf("LingerValue: %d\n\n", testOption_iLingerValue);

	TCPContext * contexts = new TCPContext[iThreadNumber]();
	thread * threads = new thread[iThreadNumber]();

	for (int j = 0; j < iThreadNumber; j++)
	{
		strcpy_s(contexts[j].pMethod, method.c_str());
		strcpy_s(contexts[j].pHost, host.c_str());
		strcpy_s(contexts[j].pUrl, url.c_str());
		strcpy_s(contexts[j].pPostData, postData.c_str());
		contexts[j].repeatCount = iRepeatCount;
		contexts[j].threadID = j;
		contexts[j].TestOption_SendAndHardClose = testOption_SendAndHardClose;
		contexts[j].TestOption_SendOnly = testOption_SendOnly;
		contexts[j].TestOption_KEEPALIVE = testOption_KeepAlive;
		contexts[j].TestOption_REUSEADDR = testOption_ReUseAddr;
		contexts[j].TestOption_LingerOn = testOption_LingerOn;
		contexts[j].TestOption_LingerValue = testOption_iLingerValue;

		threads[j] = thread(HTTPConnection::DoWork, &(contexts[j]));
	}

	for (int j = 0; j < iThreadNumber; j++)
	{
		threads[j].join();
	}
	return 0;
}