#include <winsock2.h>
#include <stdio.h>

#define MAX_PACKETLEN 256
#define nPort 3500

DWORD WINAPI t_func(void *data)
{
    SOCKET sockfd = (SOCKET)data;
    int readn, writen;
    int addrlen;
    struct sockaddr_in sock_addr;
    char szReceiveBuffer[MAX_PACKETLEN];

    printf("Thread Start : %d\n", GetCurrentThreadId());
    memset(szReceiveBuffer, 0x00, MAX_PACKETLEN);
    while( readn = recv(sockfd, szReceiveBuffer, MAX_PACKETLEN,0))
    {
   	 addrlen = sizeof(sock_addr);
   	 getpeername(sockfd, (struct sockaddr *)&sock_addr, &addrlen);
   	 printf( "Read Data %d %s(%d) : %s", GetCurrentThreadId(), inet_ntoa(sock_addr.sin_addr)
   			 ,ntohs(sock_addr.sin_port)
   			 ,szReceiveBuffer
   		 );
   	
   	 writen = send(sockfd, szReceiveBuffer, readn, 0);
   	 memset(szReceiveBuffer, 0x00, MAX_PACKETLEN);
    }
    closesocket(sockfd);
    printf("Thread Close : %d\n",GetCurrentThreadId());
    return 0;
}

int main(int argc, char **argv)
{
    WSADATA wsaData;
    SOCKET listen_s, client_s;
    struct sockaddr_in server_addr, client_addr;
    HANDLE hThread;
    int addr_len;

    if(WSAStartup(MAKEWORD(2,2),&wsaData)!= 0)
   	 return 1;

    listen_s = socket(AF_INET, SOCK_STREAM, 0);
    if(listen_s == INVALID_SOCKET)
   	 return 1;

    ZeroMemory(&server_addr, sizeof(struct sockaddr_in));

    server_addr.sin_family = PF_INET;
    server_addr.sin_port = htons(nPort);
    server_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY );

    if( bind( listen_s, (struct sockaddr*)&server_addr, sizeof(struct sockaddr_in)) == SOCKET_ERROR )
    {
   	 return 0;
    }
    if(listen(listen_s, 5) == SOCKET_ERROR)
    {
   	 return 0;
    }

    ZeroMemory(&client_addr, sizeof(struct sockaddr_in));

    while(1)
    {
   	 addr_len = sizeof(struct sockaddr_in);
   	 client_s =accept(listen_s, (struct sockaddr*) &client_addr, &addr_len);
   	 hThread = CreateThread(NULL, 0, t_func, (void *)client_s, 0, NULL);
   	 CloseHandle(hThread);
    }

    closesocket(listen_s);
    WSACleanup();
    return 0;
}
