#VARIABLE DECLARATION
CC = gcc
FLAGS = -c -Wall -g
VFLAGS = --tool=memcheck --leak-check=full --show-reachable=yes

#DESTINATION
SRC = ./src/
HDRS = ./include/
OBJ = ./
BIN = ./

#CONNECTION INFORMATION
IP = 127.0.0.1
PORT = 1151

$(BIN)all : $(BIN)server $(BIN)client

$(BIN)server : $(OBJ)server.o
	$(CC) -o $(BIN)server $(OBJ)server.o

$(BIN)client : $(OBJ)client.o
	$(CC) -o $(BIN)client $(OBJ)client.o

$(OBJ)server.o : $(SRC)server.c $(HDRS)header.h
	$(CC) $(FLAGS) $(SRC)server.c -I $(HDRS)

$(OBJ)client.o : $(SRC)client.c $(HDRS)header.h
	$(CC) $(FLAGS) $(SRC)client.c -I $(HDRS)

clean :
	rm $(OBJ)*.o
	rm $(BIN)server
	rm $(BIN)client

server : $(BIN)server
	valgrind $(VFLAGS) $(BIN)server

client : $(BIN)client 
	valgrind $(VFLAGS) $(BIN)client $(IP) $(PORT)
