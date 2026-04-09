all: clean build run

VERSION ?= dev-env

build:
	mkdir -p ./build
	g++ -DVERSION=\"$(VERSION)\" -I ./include src/*.cpp src/**/*.cpp -o ./build/particulate -lncurses -std=c++17

run:
	./build/particulate

clean:
	rm -rf ./build

# phony targets
.PHONY: all clean run

