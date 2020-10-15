#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#define WM_SOCKET WM_USER + 1
#define MAXLINE 1024
#define PORTNUM 3600

LRESULT CALLBACK WinProc(HWND hWnd, int uMsg, WPARAM wParam, LPARAM lParam);
SOCKET listen_fd;

void errMsg(char *msg)
{
	MessageBox(NULL, msg, "Error!", MB_ICONEXCLAMATION | MB_OK);
	exit(0);
}

void makeSocket(HWND hWnd)
{
	WSADATA wsaData;
	struct sockaddr_in addr;

	WSAStartup(MAKEWORD(2,2), &wsaData);

	listen_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	memset((void *)&addr, 0x00, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORTNUM);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(listen_fd, (struct sockaddr *)&addr, sizeof(addr)); 
	listen(listen_fd,5);
	WSAAsyncSelect(listen_fd, hWnd, WM_SOCKET, FD_ACCEPT | FD_CLOSE);
}

int WINAPIWinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	WNDCLASSEX wClass;
	HWND Windows;

	ZeroMemory(&wClass,sizeof(WNDCLASSEX));
	wClass.cbClsExtra=0;
	wClass.cbSize=sizeof(WNDCLASSEX);
	wClass.cbWndExtra=0;
	wClass.hbrBackground=(HBRUSH)COLOR_WINDOW;
	wClass.hCursor=LoadCursor(NULL,IDC_ARROW);
	wClass.hIcon=NULL;
	wClass.hIconSm=NULL;
	wClass.hInstance=hInst;
	wClass.lpfnWndProc=(WNDPROC)WinProc;
	wClass.lpszClassName="Window Class";
	wClass.lpszMenuName=NULL;
	wClass.style=CS_HREDRAW|CS_VREDRAW;

	RegisterClassEx(&wClass);

	Windows = CreateWindow(
 				 "Window Class", "echo server",
 				 WS_OVERLAPPEDWINDOW,
 				 200, 200, 600, 300, NULL, 0, hInst, NULL);

	while(GetMessage(&msg, 0,0,0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

LRESULT CALLBACK  WinProc(HWND hWnd, int uMsg, WPARAM wParam, LPARAM lParam)
{
	 SOCKET listen_fd=0;
	 char buf[MAXLINE]={0x00,};
	 if(uMsg == WM_CREATE)
	 {
		 makeSocket(hWnd);
	 }
	 if(uMsg == WM_SOCKET)
	 {
		 switch(WSAGETSELECTEVENT(lParam))
		 {
			 case FD_ACCEPT:
				 listen_fd = accept(wParam, NULL, NULL);
				 WSAAsyncSelect(listen_fd, hWnd, WM_SOCKET, FD_READ | FD_CLOSE);
				 break;
			 case FD_READ:
				 recv(wParam, buf, MAXLINE, 0);
				 send(wParam, buf, strlen(buf), 0);
				 break;
			 case FD_CLOSE:
				 closesocket(listen_fd);
				 break;
		 }
	 }
	 return 1;
}