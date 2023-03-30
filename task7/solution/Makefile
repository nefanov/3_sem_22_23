TARGET = Server
TARGET1 = Client

CC = g++

CFLAGS = -ggdb3 -Wall -Wpedantic -Wextra -O0
SANITIZERES = -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr
# SANITIZERES =

PREF_SRC_CLIENT = ./SrcClient/
PREF_SRC_SERVER = ./SrcServer/

PREF_OBJ_CLIENT = ./ObjClient/
PREF_OBJ_SERVER = ./ObjServer/

SRC_CLIENT = $(wildcard $(PREF_SRC_CLIENT)*.cpp)
OBJ_CLIENT = $(patsubst $(PREF_SRC_CLIENT)%.cpp, $(PREF_OBJ_CLIENT)%.o, $(SRC_CLIENT))

SRC_SERVER = $(wildcard $(PREF_SRC_SERVER)*.cpp)
OBJ_SERVER = $(patsubst $(PREF_SRC_SERVER)%.cpp, $(PREF_OBJ_SERVER)%.o, $(SRC_SERVER))

$(TARGET) : $(OBJ_SERVER)
	$(CC) $(OBJ_SERVER) $(SANITIZERES) -o $(TARGET)

$(PREF_OBJ_SERVER)%.o : $(PREF_SRC_SERVER)%.cpp
	$(CC) -c $< $(CFLAGS) -o $@

$(TARGET1) : $(OBJ_CLIENT)
	$(CC) $(OBJ_CLIENT) $(SANITIZERES) -o $(TARGET1)

$(PREF_OBJ_CLIENT)%.o : $(PREF_SRC_CLIENT)%.cpp
	$(CC) -c $< $(CFLAGS) -o $@

clean : 
	rm $(TARGET) $(PREF_OBJ_CLIENT)*.o $(PREF_OBJ_SERVER)*.o 