CC=g++
CFLAGS=-c -Wall -std=c++20
LDFLAGS=
SOURCES= main.h main.cpp command_line.cpp parser.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=Shell.exe

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o *.exe

