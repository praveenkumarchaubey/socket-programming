#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc,char *argv[])
{
    int sd, fromlen, tolen;
    int addrlen;
    struct sockaddr_in client_addr;
    struct sockaddr_in server_addr;
    int recvd;
    int structlength;
    char buf[100];

    if(argc != 2)
    {
        printf("a.out Portno\n");
        exit(1);
    }
    /* get an internet domain socket */
    if ((sd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) 
    {
        perror("socket");
        exit(1);
    }
    else
    {
        printf("socket created successfully on server side\n");
    }
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(atoi(argv[1]));

    /* bind the socket to the port number */
    if (bind(sd, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) 
    {
        perror("bind");
        exit(1);
    } 
    else
    {
        printf("bind successfull.\n");
    }
    printf("Recieved Message\n");
    structlength = sizeof(client_addr);
    recvd = recvfrom(sd, buf, sizeof(buf), 0,(struct sockaddr *) &client_addr, &structlength);

    printf("%s\n",buf);

    /* close socket */
    close(sd);
    return 0;
} /* main*/
