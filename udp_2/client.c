/*************************************************************************************************************************
 *	FILENAME : client.c
 *	DESCRIPTION: Contains Code for a echo client that will accept data from the user(keyboard) and sends that data to a echo server process
 *	and prints the received data back on the screen. (UDP)
 *	Invoke the Executable as a.out ServerIP ServerPort 
 *********************************************************************/

#include	<stdio.h>
#include	<stdlib.h>
#include	<errno.h>
#include	<string.h>
#include	<sys/types.h>
#include	<sys/socket.h>
#include	<netinet/in.h>
#include	<arpa/inet.h>
#include	<fcntl.h>
#include	<signal.h>
#include	<unistd.h>
#define BUFSIZE 512

static void sig_usr(int);
void str_cli(FILE *fp , int sockfd , struct sockaddr *server , socklen_t len);

int main( int C, char *argv[] )
{
    int	sd;
    struct	sockaddr_in serveraddress;
    /*Installing signal Handlers*/
    signal(SIGPIPE,sig_usr);
    signal(SIGINT,sig_usr);
    if (NULL==argv[1])
    {
        printf("Please enter the IP Address of the server\n");
        exit(0);
    }
    if (NULL==argv[2])
    {
        printf("Please enter the Port Number of the server\n");
        exit(0);
    }
    sd = socket( AF_INET, SOCK_DGRAM, 0 );
    if( sd < 0 ) 
    {
        perror( "socket" );
        exit( 1 );
    }

    memset( &serveraddress, 0, sizeof(serveraddress) );
    serveraddress.sin_family = AF_INET;
    serveraddress.sin_port = htons(atoi(argv[2]));//PORT NO
    serveraddress.sin_addr.s_addr = inet_addr(argv[1]);//ADDRESS

    printf("Client Starting service\n");
    printf("Enter Data For the server\n");
    str_cli(stdin,sd ,(struct sockaddr *)&serveraddress, sizeof(serveraddress));
    return 0;
}

/************************************************************************
 *	FUNCTION NAME:sig_usr
 *	DESCRIPTION: Signal Handler for Trappinf SIGPIPE
 *	NOTES : No Error Checking is done.
 *	RETURNS :void 
 ************************************************************************/
static void sig_usr(
        int signo) /*Signal Number*/
{
    char *strpipe="RECEIVED SIGPIPE - ERROR";
    char *strctrl="RECEIVED CTRL-C FROM YOU";
    if(signo==SIGPIPE)
    {
        write(1,strpipe,strlen(strpipe));
        exit(1);
    }
    else if(signo==SIGINT)
    {
        write(1,strctrl,strlen(strctrl));
        exit(1);
    }
}

/************************************************************************
 *	FUNCTION NAME:str_cli
 *	DESCRIPTION: Main Client Processing (Select waits for readiness of 
 *	connection socket or stdin
 *	NOTES : No Error Checking is done.
 *	RETURNS :void 
 **********************************************************************/

void str_cli(FILE *fp, /*Here to be used as stdin as argument*/	 
        int sockfd ,
        struct sockaddr *to ,socklen_t length)	/*Connection Socket */
{
    int maxdes,n;
    fd_set rset;
    char sendbuf[BUFSIZE] , recvbuf[BUFSIZE] ,servername[100];
    struct sockaddr_in serveraddr;
    socklen_t slen;
    FD_ZERO(&rset);
    maxdes=(sockfd>fileno(fp)?sockfd+1:fileno(fp)+1);
    for(;;){
        FD_SET(fileno(fp) , &rset);
        FD_SET(sockfd , &rset);
        select(maxdes,&rset,NULL,NULL,NULL);
        if(FD_ISSET(sockfd , & rset))
        {

            slen=sizeof(serveraddr);
            n=recvfrom(sockfd,recvbuf,BUFSIZE,0,
                    (struct sockaddr*)&serveraddr,&slen);
            printf("Data Received from server %s:\n",
                    inet_ntop(AF_INET,&serveraddr.sin_addr,
                        servername,sizeof(servername)));
            write(1,recvbuf,n);
            printf("Enter Data For the server\n");


        }
        if(FD_ISSET(fileno(fp) , & rset))
        {	
            /*Reading data from the keyboard*/
            fgets(sendbuf,BUFSIZE,fp);
            n = strlen (sendbuf);
            /*Sending the read data over socket*/
            sendto(sockfd,sendbuf,n,0,to,length);
            printf("Data Sent To Server\n");
        }
    }

}
