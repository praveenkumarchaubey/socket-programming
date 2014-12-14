#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<fcntl.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<string.h>
#include<netinet/in.h>

#define MAX 1024
#define MYPORT 15256
#define COL 30

int string_concat(int count,  char *argv[], char buf[]);
int send_string(int sd, char buf[]);
int get_response(int);
int read_string(int confd, char databuf[]);
int tokenize(char line[],char *store_string[]);
int countlen_concat(char *tok[], int size, char buffer[]);
int sent_response(int confd, char buf[]);
int free_arr(char *tok[], int word);
