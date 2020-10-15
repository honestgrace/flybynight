#include <winsock2.h>
#include <stdio.h>

#define PACKET_LENGTH 65536
#define ICMP_ECHO 8

struct icmp_hdr
{
	u_char icmp_type;
	u_char icmp_code;
	u_short icmp_cksum;
	u_short icmp_id;
	u_short icmp_seq;
	char icmp_data[1];
};

struct ip_hdr {
	u_char ip_hl;
	u_char ip_v;
	u_char ip_tos;
	short ip_len;
	u_short ip_id;
	short ip_off;
	u_char ip_ttl;
	u_char ip_p;
	u_short ip_cksum;
	struct in_addr ip_src;
	struct in_addr ip_dst;
};
int in_cksum(u_short *p, int n);

int main(int argc, char **argv)
{
	WSADATA wsaData;
	SOCKET raw_socket;
	struct sockaddr_in addr, from;
	struct icmp_hdr *p, *rp;
	struct ip_hdr *ip;
	char buffer[1024];
	int ret;
	int sl;
	int hlen;
	if(WSAStartup(MAKEWORD(2,2), &wsaData) != 0)
	{
	   	return 1;
	}
	if((raw_socket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) == INVALID_SOCKET)
	{
	   return 1;
	}
	memset(buffer, 0x00, 1024);
	p = (struct icmp_hdr *)buffer;
	p->icmp_type = ICMP_ECHO;
	p->icmp_code = 0;
	p->icmp_cksum = 0;
	p->icmp_seq = 1;
	p->icmp_id = 1;
	p-> icmp_cksum = in_cksum((unsigned short *)p, sizeof(struct icmp_hdr));
	memset(&addr, 0, sizeof(addr));
	addr.sin_addr.s_addr = inet_addr(argv[1]);
	addr.sin_family = AF_INET;
	ret= sendto(raw_socket, (char *)p,sizeof(struct icmp_hdr), 0,(struct sockaddr *)&addr, sizeof(addr));
	if(ret < 0)
	{
	   	printf("ICMP Request Error\n");
	   	return 1;
	}
	sl = sizeof(from);
	ret  = recvfrom(raw_socket, buffer, 1024, 0, (struct sockaddr *)&from, &sl);
	if (ret < 0)
	{
	   printf("ICMP Error\n");
	   return 1;
	}
	ip = (struct ip_hdr *)buffer;
	hlen = ip->ip_hl * 4;
	rp = (struct icmp_hdr *)(buffer+hlen);
	printf("reply from %s\n", inet_ntoa(from.sin_addr));
	printf("Type : %d \n", rp->icmp_type);
	printf("Code : %d \n", rp->icmp_code);
	printf("Seq : %d \n", rp->icmp_seq);
	printf("Iden : %d \n", rp->icmp_id);
	closesocket(raw_socket);
	WSACleanup( );
	return 0;
}

int in_cksum( u_short *p, int n )
{
	register u_short answer;
	register long sum = 0;
	u_short odd_byte = 0;
	while( n > 1 )
	{
	   	sum += *p++;
	   	n -= 2;
	}
	if(n == 1)
	{
	   	*(u_char*)(&odd_byte) = *(u_char*)p;
	   	sum += odd_byte;
	}
	sum = (sum >> 16) + (sum & 0xffff);
	sum += (sum >> 16);
	answer = ~sum;
	return (answer);
}