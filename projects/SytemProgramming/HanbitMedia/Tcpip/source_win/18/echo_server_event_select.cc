#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#define MAXLINE 1024
#define PORTNUM 3600
struct SOCKETINFO
{
	SOCKET fd;
	char buf[MAXLINE];
	int	readn;
	int	writen;
};
WSAEVENT EventArray[WSA_MAXIMUM_WAIT_EVENTS];
int	EventTotal=0;
struct SOCKETINFO *socketArray[WSA_MAXIMUM_WAIT_EVENTS];
int	CreateSocketInfo(SOCKET s)
{
	struct SOCKETINFO *sInfo;
	if ((EventArray[EventTotal] = WSACreateEvent()) == WSA_INVALID_EVENT)
	{
		printf("Event Failure\n");
		return -1;
	}
	sInfo = (SOCKETINFO *)malloc(sizeof(struct SOCKETINFO));
	memset((void *)sInfo, 0x00, sizeof(struct SOCKETINFO));
	sInfo->fd = s;
	sInfo->readn = 0;
	sInfo->writen = 0;
	socketArray[EventTotal] = sInfo;
	EventTotal ++;
	return 1;
}

void freeSocketInfo(int eventIndex)
{
	struct SOCKETINFO *si = socketArray[eventIndex];
	int i;
	closesocket(si->fd);
	free((void *)si);
	if(WSACloseEvent(EventArray[eventIndex]) == TRUE)
	{
		printf("Event Close OK\n");
	}
	else
	{
	printf("Event Close Failure\n");
	}
	for(i = eventIndex; i <EventTotal; i++)
	{
		EventArray[i] = EventArray[i+1];
		socketArray[i] = socketArray[i+1];
	}
	EventTotal--;
}

int main(int argc, char **argv)
{
	WSADATA wsaData;
	SOCKET sockfd, clientFd;
	WSANETWORKEVENTS networkEvents;
	char buf[MAXLINE];
	int eventIndex;
	int flags;
	struct SOCKETINFO *socketInfo;
	struct sockaddr_in sockaddr;

 
    if(WSAStartup(MAKEWORD(2,2),&wsaData)!= 0)
        return 1;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == INVALID_SOCKET)
        return 1;

    ZeroMemory(&sockaddr, sizeof(struct sockaddr_in));

    sockaddr.sin_family = PF_INET;
    sockaddr.sin_port = htons(3600);
    sockaddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

    if (bind(sockfd, (struct sockaddr*)&sockaddr, sizeof(struct sockaddr_in)) == SOCKET_ERROR)
    {
        return 0;
    }
    if(listen(sockfd, 5) == SOCKET_ERROR)
    {
        return 0;
    }

	if (CreateSocketInfo(sockfd)== -1)
	{
		return 1;
	}
	if( WSAEventSelect(sockfd, EventArray[EventTotal - 1], FD_ACCEPT|FD_CLOSE) == SOCKET_ERROR)
	{
		return 1;
	}
	while(1)
 	{
		 eventIndex = WSAWaitForMultipleEvents(EventTotal, EventArray, FALSE, WSA_INFINITE, FALSE);
		 if(eventIndex == WSA_WAIT_FAILED)
		 {
			 printf("Event Wait Failed\n");
		 }
		 if(WSAEnumNetworkEvents
			(socketArray[eventIndex - WSA_WAIT_EVENT_0]->fd,
			 EventArray[eventIndex - WSA_WAIT_EVENT_0], &networkEvents) == SOCKET_ERROR)
		 {
			 printf("Event Type Error\n");
		 }

		 if(networkEvents.lNetworkEvents& FD_ACCEPT)
		 {
			 if(networkEvents.iErrorCode[FD_ACCEPT_BIT] != 0)
			 {
				 break;
			 }
			 if ((clientFd  = accept(socketArray[eventIndex - WSA_WAIT_EVENT_0]->fd, NULL, NULL)) == INVALID_SOCKET)
			 {
				 break;
			 }
			 if(EventTotal> WSA_MAXIMUM_WAIT_EVENTS)
			 {
				 printf("Too many connections\n");
				 closesocket(clientFd);
			 }
			 CreateSocketInfo(clientFd);

			 if ((WSAEventSelect(clientFd, EventArray[EventTotal -1], FD_READ|FD_CLOSE) == SOCKET_ERROR))
			 {
				 return 1;
			 }
		 }

		 if(networkEvents.lNetworkEvents& FD_READ)
		 {
			 flags = 0;
			 memset(buf, 0x00, MAXLINE);
			 socketInfo = socketArray[eventIndex - WSA_WAIT_EVENT_0];
			 socketInfo->readn = recv(socketInfo->fd, socketInfo->buf, MAXLINE, 0);
			 send(socketInfo->fd, socketInfo->buf, socketInfo->readn, 0);
		 }

		 if(networkEvents.lNetworkEvents& FD_CLOSE)
		 {
			 printf("Socket Close\n");
			 freeSocketInfo(eventIndex-WSA_WAIT_EVENT_0);
		 }
 	}
}