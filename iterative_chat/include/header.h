#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<unistd.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<signal.h>
#include<fcntl.h>

#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#define MYPORT 1151
#define BACKLOG 5
#define MAX 100

