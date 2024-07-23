BIN_PATH=/usr/bin
all: compile install

compile:
	g++ -std=c++17 -I/usr/include/eigen3 main.cc -o bitc
install:
	install -Dm755 bitc $(BIN_PATH)/bitc
