DEBUG = True
# DEBUG = 

TARGET = Integrator
CC = g++

ifdef DEBUG
	CFLAGS = '-D _DEBUG -ggdb3 -Wall -Werror -Wpedantic -Wextra -fsanitize=undefined,address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero \
	integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,\
	undefined,unreachable,vla-bound,vptr'
else 
	CFLAGS = 
endif

PREF_SRC = ./src/
PREF_OBJ = ./obj/

SRC = $(wildcard $(PREF_SRC)*.cpp)
OBJ = $(patsubst $(PREF_SRC)%.cpp, $(PREF_OBJ)%.o, $(SRC))

$(TARGET) : $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)

$(PREF_OBJ)%.o : $(PREF_SRC)%.cpp
	$(CC) -c $< -o $@

clean : 
	rm $(TARGET) $(PREF_OBJ)*.o 
	clear