#include<header.h>

int main(int argc, char *argv[])
{
	int client_sd, concat_status = 0, send_status = 0, rcv_status = 0;
	struct sockaddr_in serveraddr;
	char buf[MAX];
	if (argc < 5)
	{
		printf("Not enough Argument\n");
		exit(1);
	}

	if (argv[1] == NULL)
	{
		printf("provide IP address of server\n");
		exit(1);
	}
	
	if (argv[2] == NULL)
        {
                printf("provide PORT number of server\n");
                exit(1);
        }

	client_sd = socket(AF_INET, SOCK_STREAM, 0);
	if (client_sd < 0)
	{
		printf("socket creation error\n");
		exit(1);
	}
	
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(atoi(argv[2]));
	serveraddr.sin_addr.s_addr = inet_addr(argv[1]);

	if (connect(client_sd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0)
	{
		printf("Cannot connect to server\n");
		exit(1);
	}

	memset(buf,'\0',MAX);
	concat_status = string_concat(argc, argv,buf);
	if (concat_status == 0)
	{
		printf("concatination fail\n");
		exit(1);
	}

	send_status = send_string(client_sd, buf);

	shutdown(client_sd, SHUT_WR);
	if (send_status == 0)
        {
                printf("sending to server fail\n");
                exit(1);
        }

	rcv_status = get_response(client_sd);
	if (rcv_status == 0)
        {
                printf("receiving from  server fail\n");
                exit(1);
        }

	close(client_sd);
	exit(0);
}

int string_concat(int count,  char *argv[], char buf[])
{
	int index,status = 0;
	for (index = 3; index < count; index++)
	{
		strcat(buf, argv[index]);
		strcat(buf, " ");
		status++;
	} 
	strcat(buf, "\0");
	printf("\nconcat string %s\n",buf);
	return status;	
}

int send_string(int sd, char buf[])
{
	int bytewrite = 0, len, written;
	len = strlen(buf);
	printf("length is %d\n",len);
	while (bytewrite < len)
	{
		written = write(sd, buf+bytewrite, len - bytewrite);
//		written = write(sd, buf, len);
		bytewrite += written;
	}
	printf("\nstring send %s\n",buf);
	return bytewrite;
}

int get_response(int sd)
{
	char buffer[MAX];
	int byteread, status = 0;
	memset(buffer, '\0', MAX);
	while ((byteread = read(sd, buffer, MAX)) > 0)
	{
		status++;
		if (write(1, buffer, byteread) != byteread)
		{
			printf("writing error from server\n");
			exit(1);
		}
	}
	return status;
}
