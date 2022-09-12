/***************************************************************************
 *	FILENAME : tcp_file_server.c
 *	DESCRIPTION:Contains Code for a file server,that will accept filename
 *	from a file client process  read that file and sends it over socket  
 *	to the client process .(Shows a typical ITERATIVE SERVER )
 *	Pl. change the value of MYPORT, when you are exceuting
 *	Invoke the Executable as a.out
 ****************************************************************************/

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
#include	<unistd.h>
#define MYPORT 11710

void readFileName(int,char *);
void readWriteFile(char * , int);

int main( int C, char *V[] )
{
    int	sd,connfd,retbind;
    struct	sockaddr_in serveraddress,cliaddr;
    socklen_t len;
    int ret;
    char buf[100],databuf[1024];

    sd = socket( AF_INET, SOCK_STREAM, 0 );
    if (sd < 0 )
    {
        perror( "socket" );
        exit( 1 );
    }

    memset( &serveraddress, 0, sizeof(serveraddress) );
    serveraddress.sin_family = AF_INET;
    serveraddress.sin_port = htons(MYPORT);//PORT NO
    serveraddress.sin_addr.s_addr = htonl(INADDR_ANY);//ADDRESS
    retbind=bind(sd,(struct sockaddr*)&serveraddress,sizeof(serveraddress));
    if(-1 == retbind)
    {
        perror("Bind Fails\n");
        exit(1);
    }
    listen(sd,5);
    /*Beginning of the Main Server Processing Loop*/

    for (;;)
    {
        printf("I am waiting-----Start of Main Loop\n");
        len=sizeof(cliaddr);
        connfd=accept(sd,(struct sockaddr*)&cliaddr,&len);
        printf("Connection from %s\n",
                inet_ntop(AF_INET,&cliaddr.sin_addr,buf,sizeof(buf)));
        if (0 == fork())
        {
            printf("Created one Child\n");
            sleep(5);
            //Reading data 
            ret=read(connfd, databuf, 100);
            databuf[ret]='\0';
            printf("Received from client%s", databuf);
            close(connfd);
            printf("FINISHED SERVING ONE CLIENT\n");
            exit(0);
        }
        close(connfd);
    }
    return 0;	
}
