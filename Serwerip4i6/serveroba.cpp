

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

	if(argc != 2)
	{
		printf("%s numerportu",argv[0]);
		return -1;
	}
	
struct addrinfo hints; 
struct addrinfo *result; 
memset(&hints, 0, sizeof(struct addrinfo)); 
hints.ai_family = AF_UNSPEC; 
hints.ai_socktype = SOCK_STREAM; 
hints.ai_flags = AI_PASSIVE; /* wildcard IP address */ 

// Retrieve each address and print out the hex bytes

struct sockaddr_storage remoteaddr;
	  fd_set fd_write,master;
	  int sockip4,sockip6,newfd;
	      socklen_t addrlen;
	  WSADATA wsaData;
	
	
	 int   retval = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (retval != 0) {
        fprintf(stderr, "WSAStartup failed: %d\n", retval);
        exit(EXIT_FAILURE);
    }

	 retval = getaddrinfo(0, argv[1], &hints, &result); 
  if ( retval != 0 ) {
       fprintf(stderr, "bind() failed with error %d: %s\n",
                        WSAGetLastError(), PrintError(WSAGetLastError()));
        WSACleanup();
        return 1;
    }

	
	     
    

	FD_ZERO( & fd_write ); 
	FD_ZERO(& master);

    for( struct addrinfo *ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

      
		if(ptr->ai_family == AF_INET)
		{


			if(( sockip4 = socket( ptr->ai_family, ptr->ai_socktype, 0 ) ) == - 1 ) {
        perror( "socket" );
        exit( 1 );
    }
			if( bind( sockip4,( struct sockaddr * ) &  ptr->ai_addr,  ptr->ai_addrlen )
    != 0 ) {
        printf( "bind ip4 %s \n",ptr->ai_addr );
		 fprintf(stderr, "lliseb failed with error %d: %s\n",
                        WSAGetLastError(), PrintError(WSAGetLastError()));
        exit( 1 );
    }
    
    if( listen( sockip4, BACKLOG ) == - 1 ) {
        perror( "listen" );
		 fprintf(stderr, "lliseb failed with error %d: %s\n",
                        WSAGetLastError(), PrintError(WSAGetLastError()));
        exit( 1 );
    }
			printf("ip4");
		}
		else if(ptr->ai_family == AF_INET6)
		{
				printf("ip6");

			/*struct sockaddr_in6 * sockaddr_ipv6 = (struct sockaddr_in6 *) ptr->ai_addr;
			if( bind( sockip6,( struct sockaddr * ) &  sockaddr_ipv6,  sizeof(struct sockaddr_in6))==-1 ) {
		
		 fprintf(stderr, "bind()  z     ip6 failed with error %d: %s\n",
                         WSAGetLastError(), PrintError(WSAGetLastError()));
        exit( 1 );
    }
    
    if( listen( sockip6, BACKLOG ) == - 1 ) {
        perror( "listen" );
			 fprintf(stderr, "listen ip6 failed with error %d: %s\n",
                         WSAGetLastError(), PrintError(WSAGetLastError()));
        exit( 1 );
    }*/
			printf("ip6");
		}      
       
        }
       

	FD_SET( sockip4, & master );
	FD_SET( sockip6, & master );

	// przejdü przez obecne po≥πczenia szukajπc danych do odczytania
	int fdmax = sockip6;
	 for(;; ) {
        fd_write = master; // copy it
        if( select( fdmax + 1, NULL, &fd_write,NULL,NULL ) == - 1 ) {
            perror( "select" );
            exit( 1 );
        }
               
            if( FD_ISSET( sockip4, & fd_write ) ) { // mamy jednego!!
				             if(( newfd = accept( sockip4,( struct sockaddr * ) & remoteaddr,
                    & addrlen ) ) == - 1 ) {
                        perror( "accept" );
                    } 
			 	char* texttosend ="Laboratorium PUS!\n";
		if( send( newfd, texttosend,strlen(texttosend) , 0 ) == - 1 )
                 perror( "send" );
            
           
        
        closesocket( newfd ); // rodzic nie potrzebuje tego
			
			}
				else if(FD_ISSET( sockip4, & fd_write )) {
                  
				             if(( newfd = accept( sockip6,( struct sockaddr * ) & remoteaddr,
                    & addrlen ) ) == - 1 ) {
                        perror( "accept" );
                    } 
			 	char* texttosend ="Laboratorium PUS!\n";
		if( send( newfd, texttosend,strlen(texttosend) , 0 ) == - 1 )
                 perror( "send" );
            
           
        
        closesocket( newfd ); // rodzic nie potrzebuje tego

			
				
				
				}
	 }


	  if (WSACleanup() != 0) {
        fprintf(stderr, "WSACleanup failed.\n");
        exit(EXIT_FAILURE);
    }

}