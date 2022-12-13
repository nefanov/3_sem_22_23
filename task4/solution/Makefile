DEBUG = True
# DEBUG = 

TARGET = Integrator
CC = g++

CFLAGS = ' -g -ggdb3 -Wall -Werror -Wpedantic -Wextra -fsanitize=undefined,address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,\
integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,\
undefined,unreachable,vla-bound,vptr'

PREF_SRC = ./src/
PREF_OBJ = ./obj/

SRC = $(wildcard $(PREF_SRC)*.cpp)
OBJ = $(patsubst $(PREF_SRC)%.cpp, $(PREF_OBJ)%.o, $(SRC))

$(TARGET) : $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) -lm -lpthread

$(PREF_OBJ)%.o : $(PREF_SRC)%.cpp
	$(CC) $(CFLAGS) -c $< -o $@ 

clean : 
	rm $(TARGET) $(PREF_OBJ)*.o 
	clear