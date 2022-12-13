DEBUG = True
# DEBUG = 

TARGET_1 = shm_client
TARGET_2 = shm_server
CC = g++

ifdef DEBUG
CFLAGS = '-D _DEBUG -ggdb3 -Wall -Werror -Wpedantic -Wextra -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero \
integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,\
undefined,unreachable,vla-bound,vptr'
else
CFLAGS = 
endif

PREF_SRC = ./src/
PREF_OBJ = ./obj/

SRC_1 = $(PREF_SRC)shm_client.cpp
OBJ_1 = $(PREF_OBJ)shm_client.o

SRC_2 = $(PREF_SRC)shm_server.cpp
OBJ_2 = $(PREF_OBJ)shm_server.o

$(TARGET_1) : $(OBJ_1)
	$(CC) $(CFLAGS) $(OBJ_1) -o $(TARGET_1)

$(TARGET_2) : $(OBJ_2)
	$(CC) $(CFLAGS) $(OBJ_2) -o $(TARGET_2)

$(PREF_OBJ)%.o : $(PREF_SRC)%.cpp
	$(CC) -c $< -o $@

clean : 
	rm $(TARGET_1) $(TARGET_2) $(PREF_OBJ)*.o 
	clear