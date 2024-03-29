/********************************************************************************************************************
 *	FILENAME : UDP_server.c
 *	DESCRIPTION: Contains Code for a echo  server, that will accept data from a client process and sends  
 *	that data back to client, using UDP
 *	Invoke the Executable as a.out    
 ****************************************************************************************************************/

#include	<stdio.h>
#include	<stdlib.h>
#include	<errno.h>
#include	<string.h>
#include	<sys/types.h>
#include	<sys/socket.h>
#include	<netinet/in.h>
#include	<arpa/inet.h>
#include	<sys/wait.h>
#include	<fcntl.h>
#include	<signal.h>

#define BUFSIZE 512
#define MYPORT 15594
#define MAXNAME 100 

int main(int C, char **V )
{
    int	sd,n,ret;
    struct	sockaddr_in serveraddress,cliaddr;
    socklen_t length;
    char clientname[MAXNAME],datareceived[BUFSIZE];

    sd = socket( AF_INET, SOCK_DGRAM, 0 );
    if( sd < 0 ) 
    {
        perror( "socket" );
        exit( 1 );
    }

    memset( &serveraddress, 0, sizeof(serveraddress) );
    memset( &cliaddr, 0, sizeof(cliaddr) );
    serveraddress.sin_family = AF_INET;
    serveraddress.sin_port = htons(MYPORT);//PORT NO
    serveraddress.sin_addr.s_addr = htonl(INADDR_ANY);//IP ADDRESS
    ret=bind(sd,(struct sockaddr*)&serveraddress,sizeof(serveraddress));

    if(ret<0)
    {
        perror("BIND FAILS");
        exit(1);
    }
    for(;;)
    {
        printf("I am waiting\n");
        /*Received a datagram*/
        length=sizeof(cliaddr);
        n=recvfrom(sd,datareceived,BUFSIZE,0,(struct sockaddr*)&cliaddr, &length);

        printf("Data Received from %s\n",inet_ntop(AF_INET,&cliaddr.sin_addr,clientname,sizeof(clientname)));

        /*Sending the Received datagram back*/
        datareceived[n]='\0';
        printf("I have received %s\n",datareceived);
        sendto(sd,datareceived,n,0,(struct sockaddr *)&cliaddr,length);
    }
    return 0;
}

