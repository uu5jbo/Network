#include "etcp.h"
#include "wincompat.c"
//#include "skel.h"
#include "netaux.c"
#include <ws2tcpip.h>

#ifndef INET6_ADDRSTRLEN
	#define INET6_ADDRSTRLEN	46
#endif

#undef _WIN32_WINNT
	#define _WIN32_WINNT	0x0600  // to make work inet_ntop()

//union includes all structs necessary
typedef union
{
    struct sockaddr         	sa;
    struct sockaddr_in      	s4;
   // struct sockaddr_in6     	s6;
    //struct sockaddr_storage 	ss;
    //struct addrinfo         	adi;
} ADDR;

char *program_name;

int main( int argc, char **argv )
{
    //ADDR addr;
    //addr.adi    hint;
    //addr.adi    *result;
    struct addrinfo hint;
    struct addrinfo *result;

	INIT();
	
    if( argc != 2  )
    {
        puts( "Resolves hostname into IP address and in "
        "future vice versa:)" );
        printf( "Usage: %s <hostname>\n", argv[0] );
        exit (EXIT_FAILURE);
    }   
     
    char *hostname = argv[1]; //pointer to the hostname entered

    printf( "QUERY: %s\n", hostname );
    bzero( &hint, sizeof(hint) );
    hint.ai_family = AF_UNSPEC; // for all ipv4 and ipv6
    hint.ai_socktype =  SOCK_DGRAM;
    
    int status = getaddrinfo( hostname, NULL, &hint, &result);   
        
    if( status )
    {
        puts( "getaddrinfo() failure");
        exit( EXIT_FAILURE );
    }
    struct addrinfo *tmp = result; // struct addrinfo *result;
    while( tmp )
    {   
        /*
        puts( "Entry:" );
        printf( "\tType: %d\n", tmp->ai_socktype );
        printf( "\tFamily: %d\n", tmp->ai_family );
        */
        static int counter = 0;

        printf( "%2d - %s has ",++counter, hostname );
        char address_string[INET6_ADDRSTRLEN] = "\0";
        void *addr, *pline;
        
		if( tmp->ai_family == AF_INET )
        {
            //printf( "\tIPv4" );
            printf( "IPv4 address ");
            addr = &( (struct sockaddr_in*)tmp->ai_addr)->sin_addr;
        }
        else
        {
            printf( "IPv6 address " );
            addr = &( (struct sockaddr_in6*)tmp->ai_addr)->sin6_addr;
        }
		/*
        inet_ntop( tmp->ai_family, addr, address_string,
                    INET6_ADDRSTRLEN );
					*/
		pline = 
		//printf("%s\n", 
		inet_ntoa( *(struct in_addr*)addr ); 
        printf( "%s\n", (char*)pline );
        tmp = tmp->ai_next;
    }
    

    freeaddrinfo( result ); // to free memory of addrinfo

    exit( EXIT_SUCCESS );
}
