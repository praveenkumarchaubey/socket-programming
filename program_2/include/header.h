#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<errno.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<arpa/inet.h>
#include<string.h>

int accept_data(int sd);
int convert_binary(int num, int);
int get_response(int sd);
int send_response(int rem[], int i, int confd);
int read_data(int confd);
