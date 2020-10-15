#include <stdio.h>
#include <winsock2.h>

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

HANDLE g_hlocp;
DWORD WINAPI Thread_func(LPVOID parm)
{
	unsigned long readn;
	unsigned long coKey;
	unsigned long flags;
	struct SocketInfo *sInfo;
	while(1)
	{
		GetQueuedCompletionStatus(g_hlocp, &readn, &coKey, (LPOVERLAPPED *)&sInfo, INFINITE);
		 if(readn == 0)
		 {
			 closesocket(sInfo->fd);
			 free(sInfo);
			 continue;
		 }
		 else
		 {
			 WSASend(sInfo->fd, &(sInfo->wsabuf), 1, &readn, 0, NULL, NULL);
		 }
		 flags=0;
		 memset(sInfo->buf, 0x00, MAXLINE);
		 sInfo->readn = 0;
		 sInfo->writen = 0;
		 WSARecv (sInfo->fd, &(sInfo->wsabuf), 1, &readn, &flags, &sInfo->overlapped, NULL);
	}
}

HANDLE g_Handle[10];
int main(int argc, char **argv)
{
	 WSADATA wsaData;
	 struct sockaddr_in addr;
	 struct SocketInfo *sInfo;
	 DWORD threadid;
	 SOCKET listen_fd, client_fd;
	 unsigned long readn;
	 int addrlen;
	 unsigned long flags;
	 if (argc != 2)
	 {
		printf("Usage : %s [port num]\n", argv[0]);
	 }
	 if(WSAStartup(MAKEWORD(2,2), &wsaData) != 0)
	 {
		return 1;
	 }
	 if(( listen_fd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	 {
	 return 1;
	 }
	 addr.sin_family = AF_INET;
	 addr.sin_port = htons(atoi(argv[1]));
	 addr.sin_addr.s_addr=htonl(INADDR_ANY);
	 if( bind(listen_fd,(struct sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR)
	 {
			 return 1;
	 }
	 if(listen(listen_fd, 5) == SOCKET_ERROR)
	 {
			 return 1;
	 }
	 CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	 for(int i = 0; i < 10; i++)
	 {
		g_Handle[i] = CreateThread(NULL, 0, Thread_func, 0, 0, &threadid);
	 }
	 while(TRUE)
	 {
		 addrlen = sizeof(addr);
 	 	 client_fd = accept( listen_fd, (struct sockaddr *)&addr, &addrlen);
	 	 if(client_fd == INVALID_SOCKET)
 		 {
 			 return 1;
 	 	 }
 	 	sInfo = (struct SocketInfo *)malloc(sizeof(struct SocketInfo));
 	 	sInfo->fd = client_fd;
 	 	sInfo->readn = 0;
 	 	sInfo->writen = 0;
 	 	sInfo->wsabuf.buf = sInfo->buf;
		sInfo->wsabuf.len = MAXLINE;

		CreateIoCompletionPort((HANDLE)client_fd, g_hlocp, (unsigned long) sInfo, 0);
		WSARecv( sInfo->fd, &sInfo->wsabuf, 1, &readn, &flags, &sInfo->overlapped, NULL);
 	}
}