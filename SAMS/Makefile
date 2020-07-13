# make_file

CC=gcc
file=main.o common.o

a.out : $(file)
	# make a.out
	${CC} -o $@ ${file} -lcurses

main.o : main.c
	${CC} -c main.c -o $@

common.o: common.c
	${CC} -c common.c -o $@

# #.PHONY: clear
clear: 
	rm *.o
	@echo "OK"
