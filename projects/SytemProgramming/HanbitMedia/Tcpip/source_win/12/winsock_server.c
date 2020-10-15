#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXLINE 1024
#define PORTNUM 3600

int main(int argc, char **argv)
{
    WSADATA wsaData;
    SOCKET sockfd;
    WSANETWORKEVENTS networkEvents;
    int eventIndex;
    int iOptVal = TRUE;
    int iOptLen = sizeof(int);
    char buf[MAXLINE];

    WSAEVENT sEvent;
    struct sockaddr_in sockaddr;

    if(WSAStartup(MAKEWORD(2,2),&wsaData) != 0)
    {
   	 printf("dll load error\n");
   	 return 1;
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == INVALID_SOCKET)
    {
   	 printf("socket create error\n");
   	 return 1;
    }

    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(PORTNUM);
    sockaddr.sin_addr.s_addr = inet_addr("192.168.56.1");

    if(connect(sockfd, (struct sockaddr *)&sockaddr,
   	 sizeof(sockaddr)) == SOCKET_ERROR)
    {
   	 printf("connect error\n");
   	 WSACleanup( );
   	 return 1;
    }

    sEvent = WSACreateEvent( );
    if(WSAEventSelect( sockfd, sEvent, FD_READ|FD_OOB) == SOCKET_ERROR)
    {
   	 printf("event select error %d\n", WSAGetLastError( ));
   	 return 1;
    }

    while(1)
    {
   	 eventIndex = WSAWaitForMultipleEvents(1, &sEvent, FALSE, WSA_INFINITE, FALSE);
   	 printf("Read Data\n");
   	 if(eventIndex == WSA_WAIT_FAILED)
   	 {
   		 printf("Event Wait Failed\n");
   	 }
   	 if(WSAEnumNetworkEvents(sockfd, sEvent, &networkEvents) == SOCKET_ERROR)
   	 {
   		 printf("Event Type Error\n");
   	 }
   	 if(networkEvents.lNetworkEvents & FD_READ)
   	 {
   		 memset(buf, 0x00, MAXLINE);
   		 recv(sockfd, buf, MAXLINE - 1, 0);
   		 printf("%s", buf);
   	 }
   	 if(networkEvents.lNetworkEvents & FD_OOB)
   	 {
   		 memset(buf, 0x00, MAXLINE);
   		 recv(sockfd, buf, 1, MSG_OOB);
   		 printf("OOB DATA %s\n", buf);
   	 }
    }
    return 0;
}
