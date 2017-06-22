LIBS = -std=c++11
OBJS = checker.o main.o
HEADERS = checker.h
CC = g++
MAKE = make

all: main trans

main: $(OBJS)
	$(CC) $(OBJS) -o main $(LIBS)

$(OBJS): %.o: %.cpp $(HEADERS)
	$(CC) $(CFLAGS) $< -c -o $@ -I . -std=c++11

trans: trans.o ../src/test/utils/utils.o
	$(CC) trans.o ../src/test/utils/utils.o -o trans

trans.o: trans.cpp ../src/test/utils/utils.h
	$(CC) trans.cpp -c -o trans.o -I ../src/ -I ../src/test/


.PHONY: clean test
test: main trans
	./main ../src/test/test
	./trans
clean:
	rm $(OBJS)
	rm main
