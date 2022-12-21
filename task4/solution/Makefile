SOURCE = integrator.c

all: build_int clean
build_int: 
	gcc $(CCFLAGS) -DTHREADS_NUM=$(T_NUM) -lpthread $(SOURCE) -lm -o integrate
.c.o:
	gcc $(CCFLAGS) -O3 $< -o $@
clean:
	rm  -rf *.o
clean_all:
	rm -rf *.o $(EXECUTABLE)