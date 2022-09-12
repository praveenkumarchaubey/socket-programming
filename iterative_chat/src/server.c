/***************************************************************************************************
 *			SERVER PROGRAM
 *************************************************************************************************/

#include "header.h"

int sd; //global variable

void signal_handler(int errno)
{
    close(sd);
}

int main()
{
    int connfd;
    int bind_status, listen_status;
    char buffer[MAX];
    int i=0, bytes;
    socklen_t len;

    struct sockaddr_in server_address, client_address;

    signal(SIGINT, signal_handler);

    sd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == sd)
    {
        printf("Error In socket creation\n");
        perror("Socket Error");
        exit(-1);
    }

    memset(&server_address, '\0', sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(MYPORT);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);

    bind_status = bind(sd, (struct sockaddr *)&server_address, sizeof(server_address));
    if(-1 == bind_status)
    {
        printf("Error In binding the socket\n");
        perror("BIND ERROR");
        exit(-1);
    }

    listen_status = listen(sd, BACKLOG);
    if(listen_status == -1)
    {
        printf("Error In Listening\n");
        perror("LISTEN ERROR");
        exit(-1);
    }
    else
    {
        printf("Server Is Listening...\n");
    }

    while (1)
    {
        len = sizeof(client_address);
        connfd = accept(sd, (struct sockaddr *)&client_address, &len);
        if(connfd == -1)
        {
            printf("Error In Accepting the client request\n");
            perror("ACCEPT ERROR");
            exit(-1);
        }
        else
        {
            printf("Request For Connnection Accepted From %s\n", 
                    inet_ntop(AF_INET, &client_address.sin_addr, buffer, sizeof(buffer)));
        }

        do
        {
            /*READING FROM CLIENT*/
            printf("Message Sent From Client:\n");
            while(1)
            {
                read(connfd, &buffer[i], sizeof(char));
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

            /*REPLYING TO THE CLIENT*/
            while(1)
            {
                printf("Enter Response\n");
                bytes = read(0, buffer, MAX*sizeof(char));
                if( 0!= bytes)
                {
                    while(bytes !=0)
                    {
                        write(connfd, &buffer[i], sizeof(char));
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
        }while(1);

        printf("Client %s Disconnected\n", 
                inet_ntop(AF_INET, &client_address.sin_addr, buffer, sizeof(buffer)));
        close(connfd);
        printf("Server is listening again...\n");

    }//end of while(1)
    close(sd);
    return 0;
}//end of main
