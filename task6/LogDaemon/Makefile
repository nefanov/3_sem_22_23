SRC		= $(shell find $(SOURCEDIR) -name '*.c')
EXEC 	= LogDaemon.exe
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

clean:
	rm -rf *.o *.rej *.exe
	rm -rf daemon_closet/ daemon_diff/ daemon_sample/