#include <winsock2.h>
#include <stdio.h>
#include <windows.h>

#define MAXLINE 1024
struct SOCKETINFO
{
	WSAOVERLAPPED overlapped;
	SOCKET fd;
	WSABUF dataBuf;
	char buf[MAXLINE];
	int readn;
	int writen;
};

struct SOCKETINFO *socketArray[WSA_MAXIMUM_WAIT_EVENTS];
int EventTotal = 0;
WSAEVENT EventArray[WSA_MAXIMUM_WAIT_EVENTS];
DWORD WINAPI ThreadProc(LPVOID argv);
CRITICAL_SECTION CriticalSection;

int main(int argc, char **argv)
{
	WSADATA wsaData;
	SOCKET listen_fd, client_fd;
	struct sockaddr_in server_addr;
	struct SOCKETINFO *sInfo;
	unsigned long ThreadId;
	unsigned long flags;
	unsigned long readn;

	if (argc != 2)
	{
		printf("Usage : %s [port number]\n", argv[0]);
		return 1;
	}

    if(WSAStartup(MAKEWORD(2,2),&wsaData)!= 0)
        return 1;
          
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(listen_fd == INVALID_SOCKET)
        return 1;
        
    ZeroMemory(&server_addr, sizeof(struct sockaddr_in));
            
    server_addr.sin_family = PF_INET;
    server_addr.sin_port = htons(3600);
    server_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    
	if (bind(listen_fd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr_in)) == SOCKET_ERROR)
    {
         return 0;
    }
    if(listen(listen_fd, 5) == SOCKET_ERROR)
    {
        return 0;
    }


	if( CreateThread(NULL, 0, ThreadProc, NULL, 0, &ThreadId) == NULL)
	{
		return 1;
	}
	EventTotal = 1;
	while(1)
	{
		if(( client_fd = accept(listen_fd, NULL, NULL)) == INVALID_SOCKET)
		 {
			 return 1;
		 }

		 EnterCriticalSection(&CriticalSection);
		 sInfo = (struct SOCKETINFO *)malloc(sizeof(struct SOCKETINFO));
		 memset((void *)sInfo, 0x00, sizeof(struct SOCKETINFO));

		 socketArray[EventTotal] = sInfo;
		 sInfo->fd = client_fd;
		 sInfo->dataBuf.len = MAXLINE;
		 sInfo->dataBuf.buf = socketArray[EventTotal]->buf;

		 EventArray[EventTotal] = WSACreateEvent( );
		 sInfo->overlapped.hEvent = EventArray[EventTotal];

		 flags = 0;
		 WSARecv(socketArray[EventTotal]->fd, &socketArray[EventTotal]->dataBuf, 1, &readn, &flags, 
				&(socketArray[EventTotal]->overlapped), NULL);
		 EventTotal++;
		 LeaveCriticalSection(&CriticalSection);
	
	}
}

DWORD WINAPI ThreadProc(LPVOID argv)
{
unsigned long readn;
unsigned long index;
unsigned long flags;
int i;
struct SOCKETINFO *si;

while(1)
{
	if(( index = WSAWaitForMultipleEvents(EventTotal, EventArray, FALSE, WSA_INFINITE, FALSE) ) == WSA_WAIT_FAILED)
	{
		return 1;
	}
 	 si = socketArray[index - WSA_WAIT_EVENT_0];
 	 WSAResetEvent(EventArray[index - WSA_WAIT_EVENT_0]);

 	 if( WSAGetOverlappedResult(si->fd, &(si->overlapped), &readn, FALSE, &flags) == FALSE || readn == 0)
 	 {
 		 closesocket(si->fd);
 		 free(si);
 		 WSACloseEvent(EventArray[index - WSA_WAIT_EVENT_0]);

 		 EnterCriticalSection(&CriticalSection);
 		 if(index - WSA_WAIT_EVENT_0 + 1 != EventTotal)
 		 {
 			 for(i = index - WSA_WAIT_EVENT_0; i <EventTotal; i++)
 			 {
 				 EventArray[i] = EventArray[i+1];
					 socketArray[i] = socketArray[i+1];
				 }
			 }
			 EventTotal --;
			 LeaveCriticalSection(&CriticalSection);
			 continue;
		 }

		 memset((void *)&si->overlapped, 0x00, sizeof(WSAOVERLAPPED));
		 si->overlapped.hEvent = EventArray[index-WSA_WAIT_EVENT_0];

		 si->dataBuf.len = MAXLINE;
		 si->dataBuf.buf = si->buf;
		 send(si->fd, si->buf, strlen(si->buf), 0);
		 if( WSARecv(si->fd, &(si->dataBuf),1, &readn, &flags, &(si->overlapped), NULL) == SOCKET_ERROR)
		 {
			 if(WSAGetLastError( ) != ERROR_IO_PENDING)
			 {
			 }
		 }
	
 	}
}