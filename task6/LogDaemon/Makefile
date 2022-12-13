INC_DIR = .
SRC_DIR =
OBJ_DIR = ./obj

SRC		= $(shell find $(SOURCEDIR) -name '*.c')
EXEC 	= LogDaemon.exe
LIBS 	= 
OBJ		= $(SRC:%.c=$(OBJ_DIR)/%.o)

СXX = g++
CC  = gcc
CFLAGS  = -g -w
LDFLAGS =

all: $(SRC) $(EXEC)
	
$(EXEC): clean $(OBJ)
	$(CC) $(OBJ) -o $@

$(OBJ): $(OBJ_DIR)/%.o: %.c
	mkdir -p $(OBJ_DIR)
	$(CC) -c $(CFLAGS) -O2 $< -o $@ -I $(INC_DIR)

valgrind:
	valgrind --leak-check=full --show-leak-kinds=all -s ./$(EXEC) 

clean:
	rm -rf $(OBJ_DIR) *.rej *.exe
	rm -rf daemon_closet/ daemon_diff/ daemon_sample/ info/
