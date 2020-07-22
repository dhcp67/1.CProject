# make_file

CC=gcc
file=common.o main.o

run : $(file)
	# make run
	${CC} -o $@ ${file} -lncursesw

main.o : main.c
	${CC} -c main.c -o $@

common.o: common.c
	${CC} -c common.c -o $@

#.PHONY: clear
clear: 
	rm *.o run
	@echo "OK"
