CC = gcc
CFLAGS= -Wall -Wextra -Wpedantic -fsanitize=address 
all: srv clt

clt: client.o
		$(CC) $(CFLAGS) -o clt client.o
srv: server.o
	$(CC) $(CFLAGS) -o srv server.o -pthread