#VARIABLE DECLARATION
CC = gcc
FLAGS = -c -Wall

#DESTINATION
SRC = ./src/
HDRS = ./include/
OBJ = ./
BIN = ./

$(BIN)all : $(BIN)server $(BIN)client

$(BIN)server : $(OBJ)server.o
	$(CC) -o $(BIN)server $(OBJ)server.o -g

$(BIN)client : $(OBJ)client.o
	$(CC) -o $(BIN)client $(OBJ)client.o -g

$(OBJ)server.o : $(SRC)server.c $(HDRS)header.h
	$(CC) $(FLAGS) $(SRC)server.c -I $(HDRS)

$(OBJ)client.o : $(SRC)client.c $(HDRS)header.h
	$(CC) $(FLAGS) $(SRC)client.c -I $(HDRS)

clean :
	rm $(OBJ)*.o
	rm $(BIN)server
	rm $(BIN)client


