all: serial

serial:
	dmd runtest.d
	g++ -g -std=c++17 main.cpp
c:
	rm a.out
	rm runtest.o
	rm runtest
