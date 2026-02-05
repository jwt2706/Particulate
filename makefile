all: clean build run

build:
	g++ main.cpp $(wildcard src/*.cpp) -o particulate -lncurses -std=c++17

run:
	./particulate
clean:
	rm -f particulate

# phony targets
.PHONY: all clean run
