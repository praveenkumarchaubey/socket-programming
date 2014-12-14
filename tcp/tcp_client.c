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


int main( int C, char *V[] )
{
	int	sd,fd;
	struct	sockaddr_in serveraddress;
	
	sd = socket( AF_INET, SOCK_STREAM, 0 );
	if( sd < 0 ) {
		perror( "socket" );
		exit( 1 );
	}

	if (V[1] == NULL ) {
		printf ("PL specfiy the IP address of the server. \n");
		exit(0);
	}
	if (V[2] == NULL ) {
		printf ("PL specfiy the port number for the server. \n");
		exit(0);
	}

	if (V[3] == NULL ) {
		printf ("PL specfiy the string to be sent \n");
		exit(0);
	}

	memset( &serveraddress, 0, sizeof(serveraddress) );
	serveraddress.sin_family = AF_INET;
	serveraddress.sin_port = htons(atoi(V[2]));//PORT NO
	serveraddress.sin_addr.s_addr = inet_addr(V[1]);//ADDRESS

	if(connect(sd,
		   (struct sockaddr*)&serveraddress,
		   sizeof(serveraddress)) < 0)
	{
		printf("Cannot Connect to server");
		exit(1);
	}

	write(sd, V[3], strlen(V[3]));
	close(sd);
	return 0;
}


