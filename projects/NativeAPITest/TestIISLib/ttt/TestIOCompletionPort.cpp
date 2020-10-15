#include "stdafx.h"

#include <stdio.h>
#include <winsock2.h>
#include <iostream>
using namespace std;

#define MAXLINE 1024
struct SocketInfo
{
	OVERLAPPED overlapped;
	SOCKET fd;
	char buf[MAXLINE];
	int readn;
	int writen;
	WSABUF wsabuf;
};

HANDLE g_hlocp = NULL;

DWORD WINAPI TestIOCompletionPort_Thread_func(LPVOID parm)
{
	unsigned long readn = 0;
	unsigned long coKey = 0;
	unsigned long flags = 0;
	struct SocketInfo *sInfo;

	while (1)
	{
		BOOL fSuccess = GetQueuedCompletionStatus(
			g_hlocp,
			&readn,
			//&coKey,
			(PULONG_PTR) &coKey,
			(LPOVERLAPPED *)&sInfo,
			INFINITE);

		if (sInfo == NULL)
		{
			printf("TestIOCompletionPort_Thread_func() - An recoverable error occurred in the completion port. Wait for the next notification");
			continue;
		}

		if (readn == 0)
		{
			// 5. Connection closed from client side. Release memory.
			closesocket(sInfo->fd);
			free(sInfo);
			continue;
		}
		else
		{
			// 2. Echo the received data
			sInfo->wsabuf.len = readn;
			WSASend(sInfo->fd, &(sInfo->wsabuf), 1, &readn, 0, NULL, NULL);

			// 
			// 3. Send shutdown message to client; Disconnecting like this is only for test purpose. The connection should not be managed like this.
			//
			auto iResult = shutdown(sInfo->fd, SD_SEND);
			if (iResult == SOCKET_ERROR) {
				printf("shutdown failed with error: %d\n", WSAGetLastError());
				closesocket(sInfo->fd);
				WSACleanup();
				return 1;
			}
		}

		// 4. Cleanup data before the next read
		flags = 0;
		memset(sInfo->buf, 0x00, MAXLINE);
		sInfo->readn = 0;
		sInfo->writen = 0;

		// 5. Initializing Asynch-Read for the next one
		int rc = WSARecv(sInfo->fd, &(sInfo->wsabuf), 1, &readn, &flags, &sInfo->overlapped, NULL);
		int err = -1;
		if ((rc == SOCKET_ERROR) && (WSA_IO_PENDING != (err = WSAGetLastError()))) {
			wprintf(L"WSARecv failed with error: %d\n", err);
			return 1;
		}
	}
}

HANDLE g_Handle[10];
int TestIOCompletionPort(int argc, char **argv)
{
	WSADATA wsaData;
	struct sockaddr_in addr;
	struct SocketInfo *sInfo;
	DWORD threadid;
	SOCKET listen_fd, client_fd;
	unsigned long readn;
	int addrlen;
	unsigned long flags;
	if (argc == 1)
	{
		printf("Usage : %s [port num]\n", argv[0]);
	}
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		return 1;
	}

	if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
	    printf("socket() failed with error %d\n", WSAGetLastError());
		return 1;
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(argv[1]));
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(listen_fd, (struct sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		return 1;
	}
	if (listen(listen_fd, 5) == SOCKET_ERROR)
	{
		return 1;
	}

	// 1. Create CP
	int numberOfThread = 1;
	g_hlocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, numberOfThread);
	if (g_hlocp == NULL)
	{
		cout << GetLastError() << endl;
		return 1;
	}

	// 2. Create thread pool
	for (int i = 0; i < numberOfThread; i++)
	{
		g_Handle[i] = CreateThread(NULL, 0, TestIOCompletionPort_Thread_func, 0, 0, &threadid);
	}

	while (TRUE)
	{
		addrlen = sizeof(addr);

		// 4. accept
		client_fd = accept(listen_fd, (struct sockaddr *)&addr, &addrlen);
		if (client_fd == INVALID_SOCKET)
		{
			return 1;
		}

		sInfo = (struct SocketInfo *)malloc(sizeof(struct SocketInfo));
		ZeroMemory(sInfo, sizeof(SocketInfo));
		sInfo->fd = client_fd;
		sInfo->readn = 0;
		sInfo->writen = 0;
		sInfo->wsabuf.buf = sInfo->buf;
		sInfo->wsabuf.len = MAXLINE;

		// 5. mapping the received port number to CP via g_hlocp
		g_hlocp = CreateIoCompletionPort((HANDLE)client_fd, g_hlocp, (unsigned long)sInfo, numberOfThread);
		if (g_hlocp == NULL)
		{
			cout << GetLastError() << endl;
			return 1;
		}

		// 6. Initializing Asynch-Read for the first read
		flags = 0;
		int rc = WSARecv(sInfo->fd, &sInfo->wsabuf, 1, &readn, &flags, &sInfo->overlapped, NULL);
		int err = -1;
		if ((rc == SOCKET_ERROR) && (WSA_IO_PENDING != (err = WSAGetLastError()))) {
			wprintf(L"WSARecv failed with error: %d\n", err);
			return 1;
		}
	}
}