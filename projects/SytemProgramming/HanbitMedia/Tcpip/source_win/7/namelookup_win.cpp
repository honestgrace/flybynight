#include <stdio.h>
#include <winsock2.h>
#include <windows.h>

int main(int argc, char **argv)
{
    WSADATA wsaData;
    int iResult;
    struct hostent *myent;

    if(argc != 2)
    {
        printf("Usage : %s [domain name]\n", argv[0]);
        return 1;
    }

    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if(iResult != 0)
    {
        printf("WSAStart up Error\n");
        return 1;
    }
    myent = gethostbyname(argv[1]);
    if (myent == NULL)
    {
        printf("Not Found Domain Name\n");
        return 1;
    }

    printf("Host Name : %s\n", myent->h_name);

    while(*myent->h_addr_list != NULL)
    {
        printf("%s\n", inet_ntoa(*(struct in_addr *)*myent->h_addr_list));
        myent->h_addr_list++;
    }
    WSACleanup( );
    return 0;
}

