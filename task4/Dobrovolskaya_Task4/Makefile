CC= g++
CFLAGS= -c -g -Wall -std=c++20
LDFLAGS=
SOURCES= Creator.h Creator.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE= Integral.exe

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o *.exe