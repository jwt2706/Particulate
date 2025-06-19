all: clean build run

build:
	g++ main.cpp $(wildcard source/*.cpp) -o main -lncurses -std=c++17

run:
	./main
clean:
	rm -f main

# phony targets
.PHONY: all clean run
