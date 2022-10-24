CC = gcc
CFLAGS= -Wall -Wextra -Wpedantic -fsanitize=address 
all: snd rcv

snd: mq_sender.o
		$(CC) $(CFLAGS) -o snd mq_sender.o
rcv: mq_receiver.o
	$(CC) $(CFLAGS) -o rcv mq_receiver.o