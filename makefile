all:
	make -C src

.PHONY: clean test

test:
	make -C src
	make -C testcase
	cp src/libMTPFSolver.so testcase/
	make test -C testcase

clean:
	make clean -C testcase
	make clean -C src
