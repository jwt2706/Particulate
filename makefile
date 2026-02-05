all: clean build run

build:
	g++ main.cpp $(wildcard src/*.cpp) -o build/particulate -lncurses -std=c++17

run:
	./build/particulate
clean:
	rm -f ./build/particulate

# phony targets
.PHONY: all clean run
