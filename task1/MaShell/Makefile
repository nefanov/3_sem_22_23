
SRC		= main.c MaShellInternal.c
EXEC 	= MaShell.exe
LIBS 	= 
OBJ		= $(SRC:.c=.o)

INC_DIR = .
SRC_DIR =

СXX = g++
CC  = gcc
CFLAGS  = -Wall-Wmissing-declarations -fsanitize=address
LDFLAGS =

all: $(SRC) $(EXEC)
	
$(EXEC): clean $(OBJ)
	$(CC) $(OBJ) -o $@

%.o: %.c
	$(CC) -c $< -I $(INC_DIR)


clean:
	clear
	rm -rf *.o *.exe