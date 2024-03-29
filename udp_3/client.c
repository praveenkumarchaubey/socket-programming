/***************************************************************************
 *	FILENAME : client.c
 *	DESCRIPTION:Contains Code for a file client that will accept a filename
 *	from the user and retrive that file from a server process and saves it 
 *	in current directory.(Using UDP)
 *	Invoke the Executable as a.out IPAddress PortNo Filename   
 *****************************************************************************/

#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<errno.h>
#include	<string.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<sys/socket.h>
#include	<netinet/in.h>
#include	<arpa/inet.h>
#include	<fcntl.h>

#define PACKETSIZE 512
#define BUFFERSIZE 1024

void sendFileName(int ,struct sockaddr*,socklen_t , char *);
void readWriteFile (int,int);

int main(int C, char *V[])
{
    int	sd,fd;
    struct	sockaddr_in serveraddress;
    char *ofile = "test";

    /*Opening a Socket descriptor*/
    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sd < 0) {
        perror("socket");
        exit(1);
    }

    /* argv[1]=IP Address of the serrver,
     * argv[2]=Port No,
     * argv[3]=file name to be fetched*/
    if (V[1] == NULL) {
        printf ("Enter the IP address of server. \n");
        exit(0);
    }

    if (V[2] == NULL) {
        printf ("Enter the port number for the server. \n");
        exit(0);
    }

    if (V[3] == NULL) {
        printf ("PL specfiy the file to be fetched. \n");
        exit(0);
    }

    /*Pupulate the server address structure*/
    memset(&serveraddress, 0, sizeof(serveraddress));
    serveraddress.sin_family = AF_INET;
    serveraddress.sin_port = htons(atoi(V[2]));//PORT NO
    serveraddress.sin_addr.s_addr = inet_addr(V[1]);//ADDRESS

    /*Send the file name to the server */
    sendFileName(sd, (struct sockaddr*)&serveraddress, 
            sizeof(serveraddress), V[3]);

    /*Opening the output file , assume the name is "test" as 
      initialised in main*/
    fd=open(ofile ,O_WRONLY|O_CREAT|O_TRUNC,S_IRWXU); 
    if (fd < 0)
    {
        printf("File opening failed!\n");
        exit(0);
    }

    /*Read the bytes sent by the server and store in the output file*/
    readWriteFile(sd,fd);

    return 0;
}

/************************************************************************
 *	FUNCTION NAME:sendFileName
 *	DESCRIPTION: sends the filename received as a string over the socket .
 *	NOTES : No Error Checking is done .
 *	RETURNS :void 
 ************************************************************************/

void sendFileName(
        int sd,	/*Socket Descriptor*/	
        struct sockaddr *to ,
        socklen_t length , /*File server's Address*/		 
        char *fname)	/*Array Containing the file name */
{	int n , byteswritten=0 , written ;
    char buffer[BUFFERSIZE];

    strcpy(buffer , fname);
    n=strlen(buffer);
    sendto(sd,buffer,n,0,to,length);
    printf("File name : %s sent to server \n",buffer);

}

/************************************************************************
 *	FUNCTION NAME:readWriteFile
 *	DESCRIPTION: Reads the socket and write in the file descriptor .
 *	NOTES : No Error Checking is done .
 *	RETURNS :void 
 ************************************************************************/
void readWriteFile(
        int sd,		/*Socket Descriptor*/	 
        int fd)		/*File descriptor  */
{	
    struct sockaddr_in serveraddress;
    socklen_t length;
    char buffer[BUFFERSIZE];
    int n;
    length=sizeof(serveraddress);
    while ((n=recvfrom(sd,buffer,PACKETSIZE,0,(struct sockaddr*)&serveraddress , &length)) >0)
    {
        /*Write the received number of Bytes in the file descriptor*/
        if (write(fd, buffer , n)!=n)
        {
            printf("File write Error \n");
            exit(0);
        }

    }
    printf ("File written successfully \n");

}

