// Lab2.cpp : Defines the entry point for the console application.
//


#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//serwer
	
#define BACKLOG 10


#pragma comment(lib, "ws2_32.lib")
#ifndef INET6_ADDRSTRLEN
#define INET6_ADDRSTRLEN 46
#endif /* INET6_ADDRSTRLEN */

LPSTR PrintError(int ErrorCode)
{
    static char Message[1024];

    // If this program was multithreaded, we'd want to use
    // FORMAT_MESSAGE_ALLOCATE_BUFFER instead of a static buffer here.
    // (And of course, free the buffer when we were done with it)

    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS |
                  FORMAT_MESSAGE_MAX_WIDTH_MASK, NULL, ErrorCode,
                  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                  (LPSTR) Message, 1024, NULL);
    return Message;
}
int main(int argc, char** argv)
{
	//init winsock
	WSADATA wsaData;
	int i;
	int retval;
	 int sockfd, new_fd; 
    struct sockaddr_in6 my_addr; // informacja o moim adresie
    struct sockaddr_in6 their_addr; //
	 int sin_size;
   int port;
    int yes = 1;
	if(argc != 2)
	{
		printf("%s numerportu",argv[0]);
		return -1;
	}
	port = atoi(argv[1]);
	 /* Inicjalizacja Windows Sockets: */
    retval = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (retval != 0) {
        fprintf(stderr, "WSAStartup failed: %d\n", retval);
        exit(EXIT_FAILURE);
    }

   
    
    if(( sockfd = socket( AF_INET6, SOCK_STREAM, 0 ) ) == - 1 ) {
        perror( "socket" );
        exit( 1 );
    }
    
	ZeroMemory(&my_addr,sizeof(struct sockaddr_in6));
    ZeroMemory(&their_addr,sizeof(struct sockaddr_in6));
    my_addr.sin6_family = AF_INET6; // host byte order
    my_addr.sin6_port = htons( port ); // short, network byte order
    my_addr.sin6_addr = in6addr_any; // 
  
    
    if( bind( sockfd,( struct sockaddr * ) & my_addr, sizeof( my_addr ) )
    == - 1 ) {
        perror( "bind" );
        exit( 1 );
    }
    
    if( listen( sockfd, BACKLOG ) == - 1 ) {
        perror( "listen" );
        exit( 1 );
    }
    
   //sin_size = sizeof(struct sockaddr_in6);
    
    while( 1 ) { // gwna pa accept()
        sin_size = sizeof( struct sockaddr_in6 );
		if(( new_fd = accept( sockfd,( struct sockaddr * ) & their_addr,
        & sin_size ) ) == - 1 ) {
            perror( "accept" );
			 fprintf(stderr, "accept() failed with error %d: %s\n",
                        WSAGetLastError(), PrintError(WSAGetLastError()));
            continue;
        }
		char  druk[INET6_ADDRSTRLEN];
        inet_ntop(AF_INET6, &(their_addr.sin6_addr),druk,INET6_ADDRSTRLEN );
		printf( "server: got connection from %s\n",
        druk);
     
           	char* texttosend ="Laboratorium PUS!\n";
		if( send( new_fd, texttosend,strlen(texttosend) , 0 ) == - 1 )
                 perror( "send" );
            
           
        
        closesocket( new_fd ); // rodzic nie potrzebuje tego
    }
    
	  

    if (WSACleanup() != 0) {
        fprintf(stderr, "WSACleanup failed.\n");
        exit(EXIT_FAILURE);
    }


}

