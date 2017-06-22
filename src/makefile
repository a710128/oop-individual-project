LIBS = -L libs -lgurobi_c++ -lgurobi70 -std=c++11
OBJS = MTPFSolver.o
HEADERS = MTPFSolver.h
CC = g++
MAKE = make
CFLAGS = -I . -std=c++11

all: libMTPFSolver.so test/test

libMTPFSolver.so: $(OBJS)
	$(CC) $(OBJS) -fPIC -shared -o libMTPFSolver.so $(LIBS)

$(OBJS): %.o: %.cpp $(HEADERS)
	$(CC) $(CFLAGS) $< -c -o $@ 

test/test: $(OBJS)
	$(MAKE) -C test

.PHONY: clean test
clean:
	rm $(OBJS)
	$(MAKE) clean -C test
test:
	echo "Test:"
