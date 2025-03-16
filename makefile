# compiler
CXX = g++

# compiler flags
CXXFLAGS = -Wall -Wextra -std=c++11

# linker flags
LDFLAGS = -lncruses

# source files
SRCS = main.cpp

# output exe
TARGET = simulator

# build target
all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS) $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

# clean
clean:
	rm -f $(TARGET)

# phony targets
.PHONY: all clean run