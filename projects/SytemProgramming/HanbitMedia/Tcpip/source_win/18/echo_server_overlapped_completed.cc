#include <winsock2.h>
#include <windows.h>
#include <stdio.h>

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
	
void CALLBACK WorkerRoutine(DWORD Error, DWORD readn, LPWSAOVERLAPPED overlapped, DWORD lnFlags);

int main(int argc, char **argv)
{
	WSADATA wsaData;
	SOCKET listen_fd, client_fd;
	struct sockaddr_in addr;
	struct SOCKETINFO *sInfo;
	unsigned long flags;
	unsigned long readn;

	if(argc != 2)
	{
		printf("Usage : %s [port number]\n", argv[0]);
		return 1;
	}

    if(WSAStartup(MAKEWORD(2,2),&wsaData)!= 0)
        return 1;
          
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(listen_fd == INVALID_SOCKET)
        return 1;
        
    ZeroMemory(&addr, sizeof(struct sockaddr_in));
            
    addr.sin_family = PF_INET;
    addr.sin_port = htons(3600);
    addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    
	if (bind(listen_fd, (struct sockaddr*)&addr, sizeof(struct sockaddr_in)) == SOCKET_ERROR)
    {
         return 0;
    }
    if(listen(listen_fd, 5) == SOCKET_ERROR)
    {
        return 0;
    }


	while(1)
	{
		if ( (client_fd = accept(listen_fd, NULL, NULL)) == INVALID_SOCKET)
		{
			return 1;
		}

		sInfo = (struct SOCKETINFO *)malloc(sizeof(struct SOCKETINFO));
		memset((void *)sInfo, 0x00, sizeof(struct SOCKETINFO));
		sInfo->fd = client_fd;
		sInfo->dataBuf.len = MAXLINE;
		sInfo->dataBuf.buf = sInfo->buf;
		flags = 0;
		if (WSARecv(sInfo->fd, &sInfo->dataBuf, 1, &readn, &flags, &(sInfo->overlapped), WorkerRoutine) == SOCKET_ERROR )
 	 {
 		 if(WSAGetLastError( ) != WSA_IO_PENDING )
 		 {
 			 printf("wsarecv error %d\n", WSAGetLastError( ));
 		 }
 	 }
  }
  return 1;
}

void CALLBACK WorkerRoutine(DWORD Error, DWORD transfern, LPWSAOVERLAPPED overlapped, DWORD lnFlags)
 {
	 struct SOCKETINFO *si;

	 unsigned long readn, writen;
	 unsigned long flags = 0;
	 si = (struct SOCKETINFO *)overlapped;

	 if(transfern == 0)
	 {
		 closesocket(si->fd);
		 free(si);
		 return ;
	 }

	 if(si->readn == 0)
	 {
		 si->readn = transfern;
		 si->writen = 0;
	 }
	 else
	 {
		 si->writen += transfern;
	 }

	 if(si->readn>si->writen)
	 {
		 memset(&(si->overlapped), 0x00, sizeof(WSAOVERLAPPED));
		 si->dataBuf.buf = si->buf + si->writen;
		 si->dataBuf.len = si->readn - si->writen;

		 if( WSASend(si->fd, &(si->dataBuf), 1, &writen, 0, &(si->overlapped), WorkerRoutine) == SOCKET_ERROR )
		 {
			 if(WSAGetLastError( ) != WSA_IO_PENDING)
			 {
				 printf("WSASend Error\n");
			 }
		 }
	 }
	 else
	 {
		 si->readn = 0;
		 flags = 0;
		 memset(&(si->overlapped), 0x00, sizeof(WSAOVERLAPPED));
		 si->dataBuf.len = MAXLINE;
		 si->dataBuf.buf = si->buf;
		 if( WSARecv(si->fd, &si->dataBuf, 1, &readn, &flags, &(si->overlapped), WorkerRoutine) == SOCKET_ERROR)
 	 {
 		 if(WSAGetLastError( ) != WSA_IO_PENDING )
 		 {
 			 printf("wsarecv error %d\n", WSAGetLastError( ));
 		 }
 	 }
  }
}