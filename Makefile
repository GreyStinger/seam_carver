CC=clang++
CARGS=--std=c++17
SOURCES=$(wildcard *.cpp)
OBJECTS=$(SOURCES:.cpp=.o)

all: main

main: $(OBJECTS)
	$(CC) $(CARGS) $^ -o $@

%.o: %.cpp
	$(CC) $(CARGS) -c $< -o $@

.PHONY: clean

clean:
	rm -f $(OBJECTS) main

