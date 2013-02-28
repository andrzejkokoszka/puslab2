// Lab2.cpp : Defines the entry point for the console application.
//

#undef UNICODE
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//klient
	
#define BACKLOG 10
#define MAXDATASIZE 100

#pragma comment(lib, "ws2_32.lib")

/*! Multi-family socket end-point address. */
typedef union address
{
    struct sockaddr sa;
    struct sockaddr_in sa_in;
    struct sockaddr_in6 sa_in6;
    struct sockaddr_storage sa_stor;
}
address_t;
int main(int argc, char** argv)
{
	//init winsock
	WSADATA wsaData;
	int i;
	int retval;
	 int sockfd, new_fd; 
    struct sockaddr_in my_addr; // informacja o moim adresie
    struct sockaddr_in their_addr; //
	 int sin_size;
   int port;
   
	  int  numbytes;
    char buf[ MAXDATASIZE ];

    struct sockaddr server; // informacja o adresie osoby ³¹cz¹cej siê
	   struct addrinfo *result = NULL;
    struct addrinfo *ptr = NULL;
    struct addrinfo hints;
	 unsigned long       addr;
	   int dwRetval;
    struct sockaddr_in  *sockaddr_ipv4;
//    struct sockaddr_in6 *sockaddr_ipv6;


	  struct sockaddr_in6 *sockaddr_ptr;

struct sockaddr_storage      client; 
socklen_t                    client_len=sizeof(struct sockaddr_storage); 
char                         host[NI_MAXHOST], serv[NI_MAXSERV];

struct sockaddr_storage portstorage;
int   port_len = sizeof(struct sockaddr);

    LPSOCKADDR sockaddr_ip;

    char ipstringbuffer[46];
    DWORD ipbufferlength = 46;
	
	if(argc != 3)
	{
		printf("%s  adres numer portu",argv[0]);
		return -1;
	}
	port = atoi(argv[1]);
	addr = atoi(argv[2]);
	 /* Inicjalizacja Windows Sockets: */
    retval = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (retval != 0) {
        fprintf(stderr, "WSAStartup failed: %d\n", retval);
        exit(EXIT_FAILURE);
    }

   
    //--------------------------------
    // Setup the hints address info structure
    // which is passed to the getaddrinfo() function
    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    printf("Calling getaddrinfo with following parameters:\n");
    printf("\tnodename = %s\n", argv[1]);
    printf("\tservname (or port) = %s\n\n", argv[2]);
    
//--------------------------------
// Call getaddrinfo(). If the call succeeds,
// the result variable will hold a linked list
// of addrinfo structures containing response
// 
	dwRetval = getaddrinfo(argv[1], argv[2], &hints, &result);
    if ( dwRetval != 0 ) {
        printf("getaddrinfo failed with error: %d\n", dwRetval);
        WSACleanup();
        return 1;
    }
     printf("getaddrinfo returned success\n");
     int iRetval;
	 i = 0;
    // Retrieve each address and print out the hex bytes
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

        printf("getaddrinfo response %d\n", i++);
        printf("\tFlags: 0x%x\n", ptr->ai_flags);
        printf("\tFamily: ");
        switch (ptr->ai_family) {
            case AF_UNSPEC:
                printf("Unspecified\n");
                break;
            case AF_INET:
                printf("AF_INET (IPv4)\n");
                sockaddr_ipv4 = (struct sockaddr_in *) ptr->ai_addr;
                printf("\tIPv4 address %s\n",
                    inet_ntoa(sockaddr_ipv4->sin_addr) );
                break;
         
        }
        printf("\tSocket type: ");
        switch (ptr->ai_socktype) {
            case 0:
                printf("Unspecified\n");
                break;
            case SOCK_STREAM:
                printf("SOCK_STREAM (stream)\n");
                break;
            case SOCK_DGRAM:
                printf("SOCK_DGRAM (datagram) \n");
                break;
            case SOCK_RAW:
                printf("SOCK_RAW (raw) \n");
                break;
            case SOCK_RDM:
                printf("SOCK_RDM (reliable message datagram)\n");
                break;
            case SOCK_SEQPACKET:
                printf("SOCK_SEQPACKET (pseudo-stream packet)\n");
                break;
            default:
                printf("Other %ld\n", ptr->ai_socktype);
                break;
        }
        printf("\tProtocol: ");
        switch (ptr->ai_protocol) {
            case 0:
                printf("Unspecified\n");
                break;
            case IPPROTO_TCP:
                printf("IPPROTO_TCP (TCP)\n");
                break;
            case IPPROTO_UDP:
                printf("IPPROTO_UDP (UDP) \n");
                break;
            default:
                printf("Other %ld\n", ptr->ai_protocol);
                break;
        }
        printf("\tLength of this sockaddr: %d\n", ptr->ai_addrlen);
        printf("\tCanonical name: %s\n", ptr->ai_canonname);
    }

    if(( sockfd = socket( AF_INET, SOCK_STREAM, 0 ) ) == - 1 ) {
        perror( "socket" );
        exit( 1 );
    }
    
   
    
	if( connect( sockfd,result->ai_addr,
    sizeof( struct sockaddr ) ) == - 1 ) {
        perror( "connect eee" );
        exit( 1 );
    }
	ZeroMemory(&client,sizeof(client));

	ZeroMemory(&portstorage,sizeof(portstorage));
	ZeroMemory(host,sizeof(host));
	sockaddr test;
	ZeroMemory(&test,sizeof(test));

	
	getsockname(sockfd,(struct sockaddr*)&client,&port_len);

	if(getnameinfo((struct sockaddr*) &client , sizeof(struct sockaddr_storage), host,sizeof(host), 
		serv,sizeof(serv), NI_NUMERICHOST|NI_NUMERICSERV)==-1) {
	   perror( "getname eee" );
        exit( 1 );
	}

	
	
//
		printf("IP %s port %s",host,serv);//,serv);

    if(( numbytes = recv( sockfd, buf, MAXDATASIZE - 1, 0 ) ) == - 1 ) {
        perror( "recv" );
        exit( 1 );
    }
    
    buf[ numbytes ] = '\0';
    
    printf( "Received: %s", buf );
    
    closesocket( sockfd );
    
    return 0;
	  

    if (WSACleanup() != 0) {
        fprintf(stderr, "WSACleanup failed.\n");
        exit(EXIT_FAILURE);
    }


}

