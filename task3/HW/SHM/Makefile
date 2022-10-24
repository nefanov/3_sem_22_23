CC = gcc
CFLAGS= -Wall -Wextra -Wpedantic -fsanitize=address 
all: snd rcv

snd: shm_send.o
		$(CC) $(CFLAGS) -o snd shm_send.o -lrt
rcv: shm_receive.o
	$(CC) $(CFLAGS) -o rcv shm_receive.o -lrt