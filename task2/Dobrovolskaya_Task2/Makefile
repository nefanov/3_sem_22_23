CC=g++
CFLAGS=-c -g -Wall -std=c++20
LDFLAGS=
SOURCES= main.h main.cpp duplex.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=duplex

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
