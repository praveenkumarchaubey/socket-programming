#include<header.h>
#define MAX 1024

int main(int argc, char *argv[])
{
    int client_sd, num;
    struct sockaddr_in serveraddr;
    if (argc < 3)
    {
        printf("Not enough argument\n");
        exit(1);
    }
    if (NULL == argv[1])
    {
        printf("provide ip address\n");
        exit(1);
    }
    if ( NULL == argv[2])
    {
        printf("provide port number\n");
        exit(1);
    }

    client_sd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_sd < 0)
    {
        printf("client socket not created\n");
        exit(1);
    }

    memset(&serveraddr,'\0',sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(atoi(argv[2]));
    serveraddr.sin_addr.s_addr = inet_addr(argv[1]);

    if (0 > (connect(client_sd, (struct sockaddr *)&serveraddr, sizeof(serveraddr))))
    {
        printf("connection fail\n");
        exit(1);
    }

    while (1)
    {
        num = accept_data(client_sd);
        //	sent_data(num);
        get_response(client_sd);
    }
    close(client_sd);
    exit(0);
}

int accept_data(int sd)
{
    int  i,len = 0, bytewrite, index,byteread;
    char buf[MAX], num[MAX];
    printf("enter number\n");
    byteread = read(0,num,MAX);
    printf("value of num %s\n",num);
    len = strlen(num);
    len = len - 1;
    printf("len is %d\n",len);
    len = htonl(len);
    bytewrite = write(sd, &len, sizeof(int));
    if (bytewrite < 0)
    {
        printf("write error\n");	
        exit(1);
    }

    bytewrite = write(sd, num, sizeof(num));	
    if (bytewrite < 0)
    {
        printf("write error\n");
        exit(1);
    }
}

int get_response(int sd)
{
    char buffer[MAX];
    int byteread, status = 0;
    memset(buffer, '\0', MAX);
    byteread = read(sd,buffer,MAX);
    write(1,buffer,byteread);
    return status;

}
