CC = gcc
CFLAGS= -Wall -Wextra -Wpedantic -fsanitize=address 
all: snd rcv

snd: fifo_sender.o
		$(CC) $(CFLAGS) -o snd fifo_sender.o
rcv: fifo_receiver.o
	$(CC) $(CFLAGS) -o rcv fifo_receiver.o