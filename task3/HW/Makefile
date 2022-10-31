CFLAGS = -DS_SIZE

task3:	
	gcc $(CFLAGS) ./FIFO/FIFO_receiver.c -o ./FIFO/FIFO_receiver.out -lrt
	gcc $(CFLAGS) ./FIFO/FIFO_sender.c -o ./FIFO/FIFO_sender.out -lrt
	gcc $(CFLAGS) ./MQ_SYSV/MQ_SYSV_receiver.c -o ./MQ_SYSV/MQ_SYSV_receiver.out -lrt
	gcc $(CFLAGS) ./MQ_SYSV/MQ_SYSV_sender.c -o ./MQ_SYSV/MQ_SYSV_sender.out -lrt
	gcc $(CFLAGS) ./SHM_POSIX/SHM_POSIX_server.c -o ./SHM_POSIX/SHM_POSIX_server.out -lrt
	gcc $(CFLAGS) ./SHM_POSIX/SHM_POSIX_client.c -o ./SHM_POSIX/SHM_POSIX_client.out -lrt