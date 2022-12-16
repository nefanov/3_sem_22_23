CC = gcc
CFLAGS= -Wall -Wextra -fsanitize=address 
all: interface daemon

interface: interface.o
		$(CC) $(CFLAGS) -o interface interface.o
daemon: daemon.o
	$(CC) $(CFLAGS) -o daemon daemon.o

	