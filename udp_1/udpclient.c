#include<string.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    char hostname[100];
    int port_no, sd ;
    struct sockaddr_in client_addr;
    struct sockaddr_in server_addr;
    int len;
    char buff[20]="System Program";

    if ((sd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) 
    {
        perror("socket");
        exit(1);
    }
    else
    {
        printf("socket created successfully.\n");
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(atoi(argv[2]));
    bind(sd,(struct sockaddr *)&server_addr,sizeof(server_addr));

    printf("Client sending a message after fill in the socket struct.\n");
    sendto(sd,buff, sizeof(buff), 0, (struct sockaddr *) &server_addr, sizeof(server_addr));

    close(sd);
    return 0;
} /* main*/

