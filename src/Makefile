CC = g++
CFLAGS = -std=c++11 -Wall
DEBUG_FLAGS = -g -O0
RELEASE_FLAGS = -O2

SRC = A1.cpp
OBJS = $(SRC:.cpp=.o)

DEBUG_TARGET = debug_program
RELEASE_TARGET = optimized_execution

all: $(DEBUG_TARGET) $(RELEASE_TARGET)

$(DEBUG_TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) -o $@ $^

$(RELEASE_TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) -o $@ $^

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(DEBUG_TARGET) $(RELEASE_TARGET)
