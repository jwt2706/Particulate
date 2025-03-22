all: build run

build:
	g++ main.cpp -o main -lncurses -std=c++11

run:
	./main
clean:
	rm -f main

# phony targets
.PHONY: all clean run
