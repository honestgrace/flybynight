// Winsock TCP Message Client
// Chin-Shiuh Shieh
// 2002-04-03
#include <winsock2.h>
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
SOCKET Socket;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormCreate(TObject *Sender)
{
WORD RequiredVersion=0x0002;
WSADATA WSAData;
int WSAStartupErrorCode;

if((WSAStartupErrorCode=WSAStartup(RequiredVersion,&WSAData))==0)
    StatusBar1->SimpleText="Winsock is started!";
else
    StatusBar1->SimpleText="WSAStartup Error "+IntToStr(WSAStartupErrorCode)+"!";
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
if(WSACleanup()==SOCKET_ERROR)
    StatusBar1->SimpleText="WSACleanup Error "+IntToStr(WSAGetLastError())+"!";
else
    StatusBar1->SimpleText="Winsock is stopped!";
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
sockaddr_in RemoteSockAddr; // Data Structure for Socket Address

if((Socket=socket(AF_INET,SOCK_STREAM,0))==INVALID_SOCKET)
    {
    StatusBar1->SimpleText="socket Error "+IntToStr(WSAGetLastError())+"!";
    return;
    }
RemoteSockAddr.sin_family=AF_INET;
RemoteSockAddr.sin_addr.s_addr=inet_addr(Edit1->Text.c_str());
RemoteSockAddr.sin_port=htons(StrToInt(Edit2->Text));
if(connect(Socket,(sockaddr*)&RemoteSockAddr,sizeof(sockaddr_in))==SOCKET_ERROR)
    {
    StatusBar1->SimpleText="connect Error "+IntToStr(WSAGetLastError())+"!";
    return;
    }
else
    {
    if(WSAAsyncSelect(Socket,Button4->Handle,0x202,FD_READ)==SOCKET_ERROR)
        {
        StatusBar1->SimpleText="WSAAsyncSelect Error "+IntToStr(WSAGetLastError())+"!";
        return;
        }
    else
        {
        Edit4->Text="";
        Button1->Enabled=false;
        Button2->Enabled=true;
        Button3->Enabled=true;
        StatusBar1->SimpleText="Connected ...";
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button2Click(TObject *Sender)
{
if(closesocket(Socket)==SOCKET_ERROR)
    {
    StatusBar1->SimpleText="closesocket Error "+IntToStr(WSAGetLastError())+"!";
    return;
    }
else
    {
    Button1->Enabled=true;
    Button2->Enabled=false;
    Button3->Enabled=false;
    StatusBar1->SimpleText="Disonnected ...";
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button3Click(TObject *Sender)
{
char buffer[256];
int len;

strcpy(buffer,Edit3->Text.c_str());
if((len=send(Socket,buffer,strlen(buffer),0))==SOCKET_ERROR)
    {
    StatusBar1->SimpleText="send Error "+IntToStr(WSAGetLastError())+"!";
    return;
    }
else
    StatusBar1->SimpleText=IntToStr(len)+" bytes sent!";
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button4MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
char buffer[256];
int len;

if((len=recv(Socket,buffer,sizeof(buffer),0))==SOCKET_ERROR)
    {
    StatusBar1->SimpleText="recv Error "+IntToStr(WSAGetLastError())+"!";
    return;
    }
else
    {
    buffer[len]=0;
    Edit4->Text=StrPas(buffer);
    StatusBar1->SimpleText=IntToStr(len)+" bytes received!";
    }
}
//---------------------------------------------------------------------------

