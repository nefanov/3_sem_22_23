VERSION 	=release
CC			= gcc
DEBUG 		= -g
CFLAGS		= -Wall -Wextra -Wpedantic
SOURCES	 	=$(wildcard ./code/Src/*.c)
INCLUDES  	= -I ./code/Inc
LIB_NAMES 	=
LIB_PATH 	= -lm -pthread
OBJ			=$(patsubst %.c, %.o, $(SOURCES))
TARGET		= FIFO_tester

all: resiver sender


resiver: resiver.o
	gcc ./resiver.o $(CFLAGS) $(LIB_PATH) -o ./progect/resiver
	rm ./resiver.o

resiver.o:
	gcc ./code/Src/resiver.c $(CFLAGS) $(LIB_PATH) -c 

sender: sender.o
	gcc ./sender.o $(CFLAGS) $(LIB_PATH) $(INCLUDES) -o ./progect/sender
	rm ./sender.o

sender.o:
	gcc ./code/Src/sender.c $(CFLAGS) $(LIB_PATH) -c 

start: resiver sender
	time ./progect/resiver & ./progect/sender

clear: 
	rm ./progect/resiver
	rm ./progect/sender
