CC=g++
CFLAGS=-c -Wall -I/Users/fuzzyllama/dev/lib/MyStanfordCPPLib
LDFLAGS= -lncurses -lStanfordCPPLib -L/Users/fuzzyllama/dev/lib/MyStanfordCPPLib -I/Users/fuzzyllama/dev/lib/MyStanfordCPPLib
SOURCES=snake.cpp main.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=snake

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

