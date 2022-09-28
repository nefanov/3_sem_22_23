
SRC		= main.c MaShellInternal.c cmd_parse.c
EXEC 	= MaShell.exe
LIBS 	= 
OBJ		= $(SRC:.c=.o)

INC_DIR = .
SRC_DIR =

СXX = g++
CC  = gcc
CFLAGS  = -g -Wall -Wmissing-declarations
LDFLAGS =

all: $(SRC) $(EXEC)
	
$(EXEC): clean $(OBJ)
	$(CC) $(OBJ) -o $@

%.o: %.c
	$(CC) -c $(CFLAGS) $< -I $(INC_DIR)


clean:
	clear
	rm -rf *.o *.exe