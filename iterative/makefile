#VARIABLE DECLARATION
CC = gcc
FLAGS = -c -Wall

all : server client

server : server.o
	$(CC) -o server server.o -g

client : client.o
	$(CC) -o client client.o -g

server.o : server.c
	$(CC) $(FLAGS) server.c

client.o : client.c
	$(CC) $(FLAGS) client.c

clean :
	rm *.o
	rm server
	rm client

