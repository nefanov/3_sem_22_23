CC=g++
CFLAGS=-c -Wall -std=c++20
LDFLAGS=
SOURCES= KV_storage.h main.c
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=KV_storage.exe

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o *.exe
