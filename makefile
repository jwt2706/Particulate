all: clean build run

build:
	mkdir -p ./build
	g++ -I ./include src/*.cpp src/**/*.cpp -o ./build/particulate -lncurses -std=c++17

run:
	./build/particulate

clean:
	rm -rf ./build

# phony targets
.PHONY: all clean run

