CC=g++
CFLAGS=-c -Wall -std=c++20
LDFLAGS=
SOURCES= main.c Inotify_service.h Demon.h Directory_service.h User_service.h
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=Demon.exe

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o *.exe
