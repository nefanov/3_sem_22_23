#files with source code

OBJECTS=$(SOURCE:.c=.o)
#resulting file
EXEC_FLDR= executables/
SHM_FLDR = shared_memory/
MSQ_FLDR = message_queue/
FIFO_FLDR = fifo_queue/
CC = gcc
all: build_shm build_msq build_fifo clean

build_obj: $(SOURCE) $(EXECUTABLE)
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

build_shm:
	$(CC) $(CCFLAGS) $(SHM_FLDR)shm_sender.c $(SHM_FLDR)shared_memory.c -DBUF_SIZE=$(BUF_SIZE) -o $(EXEC_FLDR)shm_sender
	$(CC) $(CCFLAGS) $(SHM_FLDR)shm_reciever.c $(SHM_FLDR)shared_memory.c -DBUF_SIZE=$(BUF_SIZE) -o $(EXEC_FLDR)shm_reciever
build_msq:
	$(CC) $(CCFLAGS) $(MSQ_FLDR)msq_send.c $(MSQ_FLDR)message_queue.c -DBUF_SIZE=$(BUF_SIZE) -o $(EXEC_FLDR)msq_sender
	$(CC) $(CCFLAGS) $(MSQ_FLDR)msq_recieve.c $(MSQ_FLDR)message_queue.c -DBUF_SIZE=$(BUF_SIZE) -o $(EXEC_FLDR)msq_reciever
build_fifo:
	$(CC) $(CCFLAGS) $(FIFO_FLDR)fifo_queue.c $(FIFO_FLDR)fifo_send.c -DBUF_SIZE=$(BUF_SIZE) -o $(EXEC_FLDR)fifo_sender
	$(CC)  $(CCFLAGS) $(FIFO_FLDR)fifo_queue.c $(FIFO_FLDR)fifo_recieve.c -DBUF_SIZE=$(BUF_SIZE) -o $(EXEC_FLDR)fifo_reciever
.c.o:
	$(CC) -c $(CCFLAGS) -O3 $< -o $@
clean:
	rm  -rf *.o
clean_all:
	rm -rf *.o $(EXECUTABLE)