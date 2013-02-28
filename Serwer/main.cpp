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

   
    
    if(( sockfd = socket( AF_INET, SOCK_STREAM, 0 ) ) == - 1 ) {
        perror( "socket" );
        exit( 1 );
    }
    
  
    
    my_addr.sin_family = AF_INET; // host byte order
    my_addr.sin_port = htons( port ); // short, network byte order
    my_addr.sin_addr.s_addr = INADDR_ANY; // 
    memset( &( my_addr.sin_zero ), '\0', 8 ); // wyzeruj reszttruktury
    
    if( bind( sockfd,( struct sockaddr * ) & my_addr, sizeof( struct sockaddr ) )
    == - 1 ) {
        perror( "bind" );
        exit( 1 );
    }
    
    if( listen( sockfd, BACKLOG ) == - 1 ) {
        perror( "listen" );
        exit( 1 );
    }
    
   
    
    while( 1 ) { // gwna pa accept()
        sin_size = sizeof( struct sockaddr_in );
        if(( new_fd = accept( sockfd,( struct sockaddr * ) & their_addr,
        & sin_size ) ) == - 1 ) {
            perror( "accept" );
            continue;
        }
        printf( "server: got connection from %s\n",
        inet_ntoa( their_addr.sin_addr ) );
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

