
SRC		= main.c duplexPipe.c log.c
EXEC 	= MaDuplex.exe
LIBS 	= 
OBJ		= $(SRC:.c=.o)

INC_DIR = .
SRC_DIR =

СXX = g++
CC  = gcc
CFLAGS  = -g -w
LDFLAGS =

all: $(SRC) $(EXEC)
	
$(EXEC): clean $(OBJ)
	$(CC) $(OBJ) -o $@

%.o: %.c
	$(CC) -c $(CFLAGS) -O2 $< -I $(INC_DIR)

valgrind:
	valgrind --leak-check=full --show-leak-kinds=all -s ./$(EXEC) 

big_test.txt:
	time dd if=/dev/urandom of=big_test.txt bs=1048576 count=4096

test: $(EXEC) big_test.txt
	./$(EXEC) big_test.txt

clean:
	rm -rf *.o *.exe