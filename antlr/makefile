.PHONY: run
all : test.g main.cpp
	java -jar ./antlr-3.4-complete.jar test.g 
	g++ -DDEBUG *.cpp *.c ./libantlr3c-3.4/.libs/libantlr3c.a -I. -o test 

run:
	./test input
