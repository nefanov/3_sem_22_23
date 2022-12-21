#files with source code
SOURCE= main.c duplex.c
OBJECTS=$(SOURCE:.c=.o)
#resulting file
EXECUTABLE= duplex_pipe
all: $(SOURCE) $(EXECUTABLE) clean

build_obj: $(SOURCE) $(EXECUTABLE)
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

.c.o:
	$(CC) -c $(CCFLAGS) -O3 $< -o $@
clean:
	rm -rf *.o
clean_all:
	rm -rf *.o $(EXECUTABLE)