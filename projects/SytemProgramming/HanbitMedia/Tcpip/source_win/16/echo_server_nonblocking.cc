#include <winsock2.h>
#include <stdio.h>

#define MAX_PACKETLEN 256
#define nPort 3600

struct client_list
{
	 SOCKET fd_num;
	 int is_connect;
};

int main(int argc, char **argv)
{
	 WSADATA wsaData;
	 SOCKET listen_s, client_s;
	 struct sockaddr_in server_addr, client_addr;
	 char szReceiveBuffer[MAX_PACKETLEN];
	 int readn, writen;
	 int addr_len;
	 int errno;
	 u_long iMode;
	 unsigned int i, client_num=0;
	 struct client_list clientList[1024];

	 memset ((void *)clientList, 0x00, sizeof(struct client_list)*1024);

	 if(WSAStartup(MAKEWORD(2,2),&wsaData)!= 0)
		 return 1;

	 listen_s = socket(AF_INET, SOCK_STREAM, 0);
	 if(listen_s == INVALID_SOCKET)
		 return 1;

	 ZeroMemory(&server_addr, sizeof(struct sockaddr_in));

	 server_addr.sin_family = PF_INET;
	 server_addr.sin_port = htons(nPort);
	 server_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	 if (bind(listen_s, (struct sockaddr*)&server_addr, sizeof(struct sockaddr_in)) == SOCKET_ERROR)
	 {
		 return 0;
	 }
	 if(listen(listen_s, 5) == SOCKET_ERROR)
	 {
		 return 0;
	 }

	 ZeroMemory(&client_addr, sizeof(struct sockaddr_in));

	 iMode = 1;
	 if(ioctlsocket(listen_s, FIONBIO, &iMode)!= 0)
	 {
		 printf("FIONBIO setting error\n");
		 return 1;
	 }

	 while(1)
	 {
		 addr_len = sizeof(struct sockaddr_in);
		 client_s =  accept(listen_s, (struct sockaddr*) &client_addr, &addr_len);
		 if(client_s == -1)
		 {
			 errno = WSAGetLastError();
			 if(errno == WSAEWOULDBLOCK)
			 {
			 }
			 else
			 {
				 printf("Accept Error\n");
			 }
		 }
		 else
		 {
			 printf ("Client Fd Accept %d %d\n", client_num, client_s);
			 iMode = 1;
			 ioctlsocket(client_s, FIONBIO, &iMode);

			 clientList[client_num].fd_num = client_s;
			 clientList[client_num].is_connect = 1;
			 client_num++;
		 }

		 for(i = 0; i < client_num; i++)
		 {
			 if(clientList[i].is_connect != 1) continue;
			 client_s = clientList[i].fd_num;
			 readn =  recv(client_s, szReceiveBuffer, MAX_PACKETLEN,0);
			 errno = WSAGetLastError();
			 if(readn > 0)
			 {
				 writen =  send(client_s, szReceiveBuffer, readn, 0);
			 }
			 else if(readn == 0)
			 {
				 printf("client close\n");
				 closesocket(client_s);
				 clientList[i].is_connect = 0;
			 }
			 else
			 {
				 if(errno != WSAEWOULDBLOCK)
				 {
					 printf ("client socket error %d\n", client_s);
					 closesocket(client_s);
					 clientList[i].is_connect = 0;
				 }
			 }
		 }
	 }

	 closesocket(listen_s);
	 WSACleanup();
	 return 0;
}