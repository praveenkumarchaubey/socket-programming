/***************************************************************************
 *	FILENAME : iterativefs.c
 *	DESCRIPTION:Contains Code for a file server,that will accept filename
 *	from a file client process  read that file and sends it over socket  
 *	to the client process .(Shows a typical ITERATIVE SERVER )
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
#define MYPORT 16650

void readFileName(int,char *);
void readWriteFile(char * , int);

int main( int C, char *V[] )
{
    int	sd,connfd,retbind;
    struct	sockaddr_in serveraddress,cliaddr;
    socklen_t len;
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
    serveraddress.sin_addr.s_addr = INADDR_ANY;//ADDRESS
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
        if ( connfd < 0)
        {
            if (errno == EINTR) 
                printf("Interrupted system call ?");
            continue;
        }
        printf("Connection from %s\n",
                inet_ntop(AF_INET,&cliaddr.sin_addr,buf,sizeof(buf)));

        readFileName(connfd , databuf);
        /*databuf Contains the Name of the file To be Retrived*/
        readWriteFile(databuf,connfd);
        close(connfd);
        printf("FINISHED SERVING ONE CLIENT\n");
    }

}

/********************************************************************
 *	FUNCTION NAME:readFileName
 *	DESCRIPTION: Reads the filename sent by the client over the 
 *	socket	and stores it in the array fname .
 *	NOTES : No Error Checking is done .
 *	RETURNS :void 
 *********************************************************************/

void readFileName(
        int connfd,		/*Socket Descriptor*/	 
        char *fname)	/*Array Containing the file name */
{	
    int pointer=0,n;
    while ((n=read(connfd,(fname + pointer),1024))>0){
        pointer=pointer+n;
    }
    fname[pointer]='\0';
    printf("Server :Request to send file  %s\n " ,fname);

}

/*********************************************************************
 *	FUNCTION NAME:readWriteFile
 *	DESCRIPTION: Opens the file , whose name has been sent by the 
 *	client over the socket
 *	, reads the file and writes the data back to the socket .
 *	NOTES : No Error Checking is done .
 *	RETURNS :void 
 ********************************************************************/
void readWriteFile(
        char *fname,	/*Array Containing the file name */
        int connfd)	/*Connection Socket Descriptor*/
{	
    int fd,n;
    char sendbuffer[1024];
    fd = open(fname, O_RDONLY);
    if (fd < 0 )
    {
        printf("SERVER : Error Opening File \n");
        exit(0);
    }
    while ( (n=read( fd , sendbuffer , 1024)) >0)
        write( connfd , sendbuffer , n);
    printf ("File sending successful \n");
    close(fd);
}

