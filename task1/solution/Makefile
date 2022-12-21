#files with source code
SOURCE= main.c executer.c parser.c
OBJECTS=$(SOURCE:.c=.o)
#resulting file
EXECUTABLE= terminal
all: $(SOURCE) $(EXECUTABLE) clean

build_obj: $(SOURCE) $(EXECUTABLE)
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

.c.o:
	$(CC) -c $(CCFLAGS) $< -o $@
clean:
	rm -rf *.o
clean_all:
	rm -rf *.o $(EXECUTABLE)