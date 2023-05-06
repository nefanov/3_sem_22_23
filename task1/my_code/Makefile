VERSION 	=release
CC			= gcc
DEBUG 		= -g
CFLAGS		= -Wall -Wextra -Wpedantic
SOURCES	 	=$(wildcard ./code/Src/*.c)
INCLUDES  	= -I ./code/Inc
LIB_NAMES 	=
LIB_PATH 	= -lm
OBJ			=$(patsubst %.c, %.o, $(SOURCES))
TARGET		=shell-wrapper

#links
$(TARGET):$(OBJ)
	@mkdir -p progect
	$(CC) $(OBJ) $(LIB_PATH) $(LIB_NAMES) -o ./progect/$(TARGET)_$(VERSION)
	@rm -rf $(OBJ)
	
#compile
%.o: %.c
	$(CC) $(INCLUDES) $(DEBUG) -c $(CFLAGS) $< -o $@

start: ./progect/$(TARGET)_$(VERSION)
	./progect/$(TARGET)_$(VERSION)

mem_test: ./progect/$(TARGET)_$(VERSION)
	valgrind ./progect/$(TARGET)_$(VERSION)

debug: ./progect/$(TARGET)_$(VERSION)
	gdb ./progect/$(TARGET)_$(VERSION)

.PHONY:clear
clear:
	@echo "Remove linked and compiled files......"
	rm -rf $(OBJ) $(TARGET) output 

