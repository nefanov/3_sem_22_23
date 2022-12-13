SRC	    = snd.c rcv.c
PROG    = FIFO
EXEC    = $(SRC:.c=)
LIBS    = -pthread -lrt
OBJ	    = $(SRC:.c=.o)

INC_DIR = .
SRC_DIR =

CC  = gcc
CFLAGS  = -g -Wall

all: $(EXEC)
	
rcv: rcv.o log.o
	$(CC) $@.o log.o -o $@.exe $(LIBS)

snd: snd.o log.o
	$(CC) $@.o log.o -o $@.exe $(LIBS)

%.o: %.c
	$(CC) -c $(CFLAGS) -O2 $< -I $(INC_DIR)

SIZE = 5
test_file:
	time dd if=/dev/urandom of=input.txt bs=1048576 count=$(SIZE)

clean:
	rm -rf *.o *.exe *.txt