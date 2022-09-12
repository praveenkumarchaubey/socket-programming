#include<header.h>
int sd;
#define MYPORT 15151
#define MAX 1024

int main(void)
{
    struct sockaddr_in serveraddr, clientaddr;
    socklen_t len;
    char buf[MAX];
    int bind_status, listen_status, confd, read_status;
    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd < 0)
    {
        printf("server socket fail\n");
        exit(1);
    }

    memset(&serveraddr, '\0', sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(MYPORT); 	
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind_status = bind(sd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    if (bind_status != 0)
    {
        printf("binding fail\n");
        exit(1);
    }

    listen_status = listen(sd, 5);
    if (listen_status != 0)
    {
        printf("error in listen call\n");
        exit(1);
    }

    while(sd > 0)
    {
        printf("SERVER waiting...\n");
        len = sizeof(clientaddr);
        confd = accept(sd, (struct sockaddr*)&clientaddr, &len);
        if (confd < 0)
        {
            printf("connection fail\n");
            continue;
        }

        memset(buf, '\0', MAX);
        printf("connection from %s\n",inet_ntop(AF_INET, &clientaddr.sin_addr, buf, sizeof(buf)));
        read_status = read_data(confd);
        if (read_status == 0)
        {
            printf("reading error\n");
            exit(1);
        }
    }//end of while
    return 0;
}

int read_data(int confd)
{
    int byteread, i, j;
    char buf[MAX];
    byteread = read(confd,&i,sizeof(int));
    i = ntohl(i);
    printf("len is %d\n",i);
    write(1, &i, sizeof(int));
    printf("\n");
    read(confd, buf, i);
    write(1,buf,i);
    j = atoi(buf);
    convert_binary(j,confd);	
    return byteread;
}


int convert_binary(int num, int confd)
{
    int index, rem[MAX],i=0,j;
    index = num;
    while(index > 0)
    {
        rem[i] = index % 2;
        index = index/2;
        i++;
    }
    for( j = i-1; j>=0;j--)
    {
        printf("arr %d\n",rem[j]);
    }
    send_response(rem,i,confd);
}


int send_response(int rem[], int i, int confd)
{
    char rev[MAX];
    int j,k=0;
    char buf[MAX];
    for( j = i-1; j>=0;j--)
    {
        printf("tanvi %d\n",rem[j]);
        rev[k] = rem[j]+'0';
        printf("tanvi %c\n",rev[k]);
        k++;
    }
    rev[k] = '\0';
    printf("rev is %s\n",rev);
    printf("size is %d\n",strlen(rev));
    write(confd, rev, strlen(rev));
}

