#VARIABLE DECLARATION
CC = gcc
FLAGS = -c -Wall

all : server client

server : threadedfs.o
	$(CC) -o server threadedfs.o -g -lpthread

client : fileclient.o
	$(CC) -o client fileclient.o -g

threadedfs.o : threadedfs.c
	$(CC) $(FLAGS) threadedfs.c

fileclient.o : fileclient.c
	$(CC) $(FLAGS) fileclient.c

clean :
	rm *.o
	rm server
	rm client

