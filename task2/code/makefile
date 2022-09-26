CXX = g++
CXXFLAGS = -Wall -Wextra
SRC = $(wildcard src/*.cpp)
TARGET = bin/prog


$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@


run:
	time ./bin/prog
	md5sum io/input
	md5sum io/output

gen:
# time dd if=/dev/urandom of=io/input bs=1048576 count=4096
	time dd if=/dev/urandom of=io/input bs=20000 count=100000