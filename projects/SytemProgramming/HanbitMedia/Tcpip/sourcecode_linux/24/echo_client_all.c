#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <net/if.h>

int main(int argc, char **argv)
{
    struct if_nameindex *if_idx, *ifp;
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    struct sockaddr_in6 *sin6;
    int sock_fd;
    int connection = 0;

    if(argc != 3)
    {
        printf("Usage : %s [ip] [port]\n", argv[0]);
        return 1;
    }

    memset(&hints, 0x00, sizeof(hints));

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = 0;
    hints.ai_flags = 0;

    if(getaddrinfo(argv[1], argv[2], &hints, &result) != 0 )
    {
        return 1;
    }

    for(rp = result; rp != NULL; rp = rp->ai_next)
    {
        sock_fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if(sock_fd == -1)
        {
            continue;
        }

        if(rp->ai_family == AF_INET6)
        {
            if_idx = if_nameindex();
            for(ifp = if_idx; ifp->if_name != NULL; ifp++)
            {
                sin6 = (void *)rp->ai_addr;
                sin6->sin6_scope_id = ifp->if_index;
                if(strcmp(ifp->if_name, "lo") == 0) continue;
                printf("Connection ID==>%d\n", ifp->if_index);
                if(connect(sock_fd, (struct sockaddr *)sin6, rp->ai_addrlen) != -1)
                {
                    printf("ok IPv6 connect\n");
                    connection = 1;
                    break;
                }
                else
                {
                    perror("Connection failed\n");
                }
            }
        }
        else
        {
            if(connect(sock_fd, rp->ai_addr, rp->ai_addrlen) != -1)
            {
                printf("ok IPv4connect\n");
                connection = 1;
                break;
            }
        }
        if(connection) break;
        close(sock_fd);
    }

    if(rp == NULL)
    {
        printf("Connection Failed\n");
        return 1;
    }
    freeaddrinfo(result);


    return 1;
}

