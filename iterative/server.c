/***************************************************************************
*	FILENAME :server.c
*	DESCRIPTION:Contains Code for a  server,that will accept 
*       a string from a client process , prints the string and the 
*	IP Address of the client .(Shows a typical ITERATIVE SERVER )
*	Invoke the Executable as a.out PortNo
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

#define MYPORT 16650/*The port users will be connecting to*/

void readstring(int,char *);

int main(int C, char *V[] )
{
	int	listensocket, connectionsocket;
	struct	sockaddr_in serveraddress,cliaddr;
	int binderror;
	socklen_t len;
	char buf[100], databuf[1024];

	listensocket = socket(AF_INET, SOCK_STREAM, 0 );
	if (listensocket < 0 ) 
	{
		perror("socket" );
		exit(1 );
	}

	memset(&serveraddress, 0, sizeof(serveraddress) );
	serveraddress.sin_family = AF_INET;
	serveraddress.sin_port = htons(MYPORT);/*PORT NO*/
	serveraddress.sin_addr.s_addr = htonl(INADDR_ANY);/*ADDRESS*/
	binderror=bind(listensocket,(struct sockaddr*)&serveraddress, sizeof(serveraddress));
	if (-1 == binderror)
	{
			perror("BIND !!!!!!!");
			exit(1);
	}
	listen(listensocket,5);
	
	/*Beginning of the Main Server Processing Loop*/
	
	for (;;)
	{
		printf("Server:I am waiting-----Start of Main Loop\n");
		len=sizeof(cliaddr);
		connectionsocket=accept(listensocket, (struct sockaddr*)&cliaddr,&len);
		if (connectionsocket < 0)
		{
			if (errno == EINTR) 
			printf("Interrupted system call ??");
			continue;
		}
		printf("Connection from %s\n", inet_ntop(AF_INET,&cliaddr.sin_addr,buf,sizeof(buf)));
		readstring(connectionsocket , databuf);
		printf("Finished Serving One Client\n");
		close(connfd);	
	}
	close(sd);	
}

/********************************************************************
*	FUNCTION NAME:readstring
*	DESCRIPTION: Reads the string  sent by the client over the 
*	socket	and stores it in the array fname .
*	NOTES : No Error Checking is done .
*	RETURNS :void 
*********************************************************************/

void readstring(
	int connectionsocket,		/*Socket Descriptor*/	 
	char *fname)	/*Array , to be populated by the string from client*/
/********************************************************************/
{	
	int pointer=0,n;
	while ((n=read(connectionsocket,(fname + pointer),1024))>0)
	{
		pointer=pointer+n;
	}
	fname[pointer]='\0';
	printf("Server :Received   %s\n " ,fname);

}
/**********************************************************************/

