/*****************************************************************************************
 *		CLIENT PROGRAM
 **************************************************************************************/
#include "header.h"

int main(int argc, char *argv[])
{
    while(3 != argc)
    {
        printf("Insufficient Number Of Arguments\n");
        if(NULL == argv[1])
        {
            printf("Please provide the IP address of the server\n");
        }
        if(NULL == argv[2])
        {
            printf("Please provide the PORT address of the server\n");
        }
        exit(-1);
    }
    int sd, bytes, connect_status;
    char buffer[MAX];
    int i=0;

    struct sockaddr_in server_address;
    sd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == sd)
    {
        printf("Error In socket creation\n");
        perror("Socket Error");
        exit(-1);
    }
    memset(&server_address, '\0', sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(atoi(argv[2]));
    server_address.sin_addr.s_addr = inet_addr(argv[1]);

    connect_status = connect(sd, (struct sockaddr *)&server_address, sizeof(server_address));
    if(connect_status == -1)
    {
        printf("Error In Connecting With the server\n");
        perror("CONNECT ERROR");
        exit(-1);
    }
    else
    {
        printf("Connnection ESTABLISHED\n");
    }
    do
    {
        /*TALKING TO THE SERVER*/
        while(1)
        {
            printf("Enter Message\n");
            bytes = read(0, buffer, MAX);
            if( 0!= bytes)
            {
                while(bytes !=0)
                {
                    write(sd, &buffer[i], sizeof(char));
                    --bytes;
                    ++i;
                }
                i=0;
                break;
            }
        }

        if(strncmp(buffer, "bye" ,3)==0)
        {
            break;
        }
        /*RECEIVING FROM SERVER*/
        printf("Server's Response\n");
        while(1)
        {
            read(sd, &buffer[i], sizeof(char));
            if(buffer[i]!='\n')
            {
                write(1, &buffer[i], sizeof(char));
            }
            else
            {
                printf("\n");
                i = 0;
                break;
            }
            ++i;
        }
        if(strncmp(buffer, "bye" ,3)==0)
        {
            break;
        }

    }while(1);

    close(sd);
    return 0;

}//end of main

