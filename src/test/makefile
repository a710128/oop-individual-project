LIBS = -L ../ -lMTPFSolver -std=c++11
OBJS = test.o utils/utils.o
HEADERS = ../MTPFSolver.h utils/utils.h
CC = g++
MAKE = make

all: test

test: $(OBJS)
	$(CC) $(OBJS) -o test $(LIBS)

$(OBJS): %.o: %.cpp $(HEADERS)
	$(CC) $(CFLAGS) $< -c -o $@ -I .. -std=c++11


.PHONY: clean
clean:
	rm $(OBJS) test