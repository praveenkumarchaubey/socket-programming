#include<header.h>

int main(void)
{
	int free_check, bind_status, sd, confd, len, read_status = 0, word, status = 0, sent_status = 0;
	struct sockaddr_in serveraddr,cliaddr;
	char buf[MAX], databuf[MAX], *tok[COL], buffer[MAX];

	sd = socket(AF_INET, SOCK_STREAM, 0);
	if (sd < 0)
	{
		printf("error in creating sever socket\n");
		exit(1);
	}
	
	memset(&serveraddr, 0, sizeof(serveraddr));
	
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(MYPORT);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	bind_status = bind(sd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
	if (bind_status < 0)
	{
		printf("binding error\n");
		exit(1);
	}

	listen(sd, 5);
	for (;;)
	{
		printf("I AM SERVER\n");
		len = sizeof(cliaddr);
		confd = accept(sd, (struct sockaddr *)&cliaddr, &len);
		if (confd < 0)
		{
			printf("error in connecting to client\n");
			continue;
		}

		printf("connection from %s\n",inet_ntop(AF_INET, &cliaddr.sin_addr, buf, sizeof(buf)));
		memset(databuf, '\0', MAX);

		read_status = read_string(confd, databuf);
		if (read_status == 0)
		{
			printf("error in reading from client\n");
			exit(1);
		}
		
		word = tokenize(databuf,tok);

		status = countlen_concat(tok, word, buffer);
		if (status != word)
		{
			printf("concatination at server fail\n");
			exit(1);
		}

		sent_status = sent_response(confd, buffer);
		if (sent_status == 0)
		{
			printf("sending to client fail\n");
			exit(1);
		}

		free_check = free_arr(tok, word);
		if (free_check != word)
		{
			printf("error in freeing memory\n");
			exit(1);
		}
		printf("Finished serving client one\n");
		close(confd);
	}
	close(sd);
	exit(0);
}

int free_arr(char *tok[], int word)
{
	int index;
	for(index = 0; index < word; index++)
	{
		free(tok[index]);
	}
	return index;
}


int read_string(int confd, char databuf[])
{
	int byteread, point = 0;
//	point = read(confd, databuf, MAX);
	while((byteread = read(confd, databuf + point, MAX)) > 0)
	{
		point += byteread;
	}
	printf("server receive is %s\n", databuf);
	return point; 
}

int tokenize(char line[], char *store_string[])
{
/*      keep the copy of original line as strtok changes lines into token      */
        int word_count=0, i =0;
        char *token;
       // word_count counts the number of words in line
        printf("string is in tokenize %s\n",line);
        token = strtok(line," ");
        while(token!=NULL)
        {
                store_string[i] = (char *)calloc(sizeof(token)+1, sizeof(char));
                if(store_string[i] == NULL)
                {
                        printf("calloc fail\n");
                        exit(1);
                }
                word_count++;
                strcpy(store_string[i], token);
                printf("token is %s\n",token);
                token = strtok(NULL," ");
                i++;
        }
        printf("word count is %d\n",i);
        return i;
}

int countlen_concat(char *tok[], int size, char buffer[])
{
	int index, status = 0;
	char str[MAX];
	for (index = 0; index < size; index++)
	{
		status++;
		sprintf(str,"%s%d",tok[index],strlen(tok[index])); //sprintf function
		strcat(buffer, str);
		strcat(buffer, " ");
	}
	strcat(buffer, "\0");
	return status;
}

int sent_response(int confd, char buf[])
{
	int len, bytewrite = 0, written;
	len = strlen(buf);
	while (bytewrite < len)
	{
		written = write(confd, buf + bytewrite, len - bytewrite);
		bytewrite += written;
	}
	return bytewrite;
}
