/******************************************************************************
 *	FILENAME : server.c
 *	DESCRIPTION:Contains Code for a file server ,that will accept filename
 *	from a file client process  read that file and sends it over socket  
 *	to the client process .(Uses UDP to break the file into fixed size blocks
 *	,except perhaps the last block . )
 *	Invoke the Executable as a.out    
 ******************************************************************************/


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

#define PACKETSIZE 512
#define BLOCKSIZE 512
#define BUFFERSIZE 1024
#define MYPORT 60000 

void readFileName(int sd, struct sockaddr *cliaddr, 
        socklen_t *length, char *fname);

void readWriteFile(int sd, char *fname, 
        struct sockaddr *cliaddr, socklen_t length);

int main(int C, char *V[])
{
    int	sd, connfd, ret;
    struct	sockaddr_in serveraddress, cliaddr;
    socklen_t length;
    char buf[BUFFERSIZE], databuf[BUFFERSIZE];

    /*Opening the socket descriptor*/
    sd = socket(AF_INET, SOCK_DGRAM, 0 );
    if(sd < 0)
    {
        perror("socket");
        exit(1);
    }

    /*Populating Own address structure to bind it to the socket*/
    memset(&serveraddress, 0, sizeof(serveraddress));
    serveraddress.sin_family = AF_INET;
    serveraddress.sin_port = htons(MYPORT);//PORT NO
    serveraddress.sin_addr.s_addr = htonl(INADDR_ANY);//IP ADDRESS

    /*Now Binding*/
    ret = bind(sd, (struct sockaddr*)&serveraddress, sizeof(serveraddress));
    if ( ret < 0)
    {
        printf("Error in Bind\n");
        perror("BIND");
        exit(1);
    }

    /*Now waiting for Getting request*/
    for(;;)
    {
        printf("I am waiting\n");
        length = sizeof(cliaddr);

        /*Read file name and store it in databuf*/
        readFileName(sd, (struct sockaddr*)&cliaddr, &length, databuf);

        /*Printing the IP Address of the Client*/
        printf("Request  from %s\n", inet_ntop(AF_INET, &cliaddr.sin_addr, buf, sizeof(buf)));
        /*Now reading the requested file and sending over the socket*/
        readWriteFile(sd,databuf,(struct sockaddr*)&cliaddr,length);

    }
    return 0;
}

/************************************************************************
 *	FUNCTION NAME:readFileName
 *	DESCRIPTION: Reads the filename sent by the client over the socket 
 *	and stores it in the array fname .
 *	NOTES : No Error Checking is done .
 *	RETURNS :void 
 ************************************************************************/

void readFileName(
        int sd, /*Socket Descriptor*/	
        struct sockaddr *cliaddr,
        socklen_t *length ,		 
        char *fname)	/*Array Containing the file name */
{	
    int n;
    n=recvfrom(sd, fname, PACKETSIZE, 0, cliaddr, length);
    /*Adding the string terminator*/
    fname[n]='\0';

    /*Printing the name of the file to send*/
    printf("Server :Request to send file  %s\n " ,fname);

}

/************************************************************************
 *	FUNCTION NAME:readWriteFile
 *	DESCRIPTION: Opens the file , whose name has been sent by the client over the socket 
 *	, reads the file and writes the data back to the socket .
 *	NOTES : No Error Checking is done .
 *	RETURNS :void 
 ************************************************************************/
void readWriteFile(
        int sd,	/*Socket Descriptor*/	
        char *fname,	/*Array Containing the file name */
        struct sockaddr *cliaddr,/*Client Address Structure*/
        socklen_t length)	/*length of the address structure*/
{	
    int fd,n;
    char sendbuffer[BUFFERSIZE];

    /*Opening the requested file*/
    fd = open(fname, O_RDONLY);
    if (fd < 0 )
    {
        printf("SERVER : ERROR OPENING FILE \n");
        exit(0);
    }
    /*Read the file in Blocks of BLOCKSIZE and send it over socket*/
    while ((n=read(fd , sendbuffer , BLOCKSIZE)) >0)
        sendto(sd,sendbuffer,n,0,cliaddr,length);
    //Send a zero length UDP Message to show the end of file transfer.
    sendto(sd,sendbuffer,0,0,cliaddr,length);
    printf ("File sending successful \n");
    /*Closing the file descriptor*/
    close(fd);
}
