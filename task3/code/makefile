run_server:
	# time ./bin/shm/server
	# time ./bin/mq/server
	time ./bin/fifo/server

run_client:
	# time ./bin/shm/client
	# time ./bin/mq/client
	time ./bin/fifo/client
	# md5sum files/check
	# md5sum files/small
	# md5sum files/medium
	# md5sum files/big
	# md5sum files/out

build_server:
	# g++ src/sh_mem/server.cpp -o bin/shm/server -DSMALL
	# g++ src/sh_mem/server.cpp -o bin/shm/server -DMEDIUM
	# g++ src/sh_mem/server.cpp -o bin/shm/server -DBIG

	# g++ src/mq/server.cpp -o bin/mq/server -DSMALL
	# g++ src/mq/server.cpp -o bin/mq/server -DMEDIUM
	# g++ src/mq/server.cpp -o bin/mq/server -DBIG

	# g++ src/fifo/server.cpp -o bin/fifo/server -DSMALL
	# g++ src/fifo/server.cpp -o bin/fifo/server -DMEDIUM
	g++ src/fifo/server.cpp -o bin/fifo/server -DBIG

build_client:
	# g++ src/sh_mem/client.cpp -o bin/shm/client -DSMALL
	# g++ src/sh_mem/client.cpp -o bin/shm/client -DMEDIUM
	# g++ src/sh_mem/client.cpp -o bin/shm/client -DBIG

	# g++ src/mq/client.cpp -o bin/mq/client -DSMALL
	# g++ src/mq/client.cpp -o bin/mq/client -DMEDIUM
	# g++ src/mq/client.cpp -o bin/mq/client -DBIG

	# g++ src/fifo/client.cpp -o bin/fifo/client -DSMALL
	# g++ src/fifo/client.cpp -o bin/fifo/client -DMEDIUM
	g++ src/fifo/client.cpp -o bin/fifo/client -DBIG

gen_small:
	time dd if=/dev/urandom of=files/small bs=10000 count=500

gen_medium:
	time dd if=/dev/urandom of=files/medium bs=100000 count=2000

gen_big:
	time dd if=/dev/urandom of=files/big bs=1000000 count=2000

# run:
# 	time ./bin/prog
# 	md5sum io/input
# 	md5sum io/output