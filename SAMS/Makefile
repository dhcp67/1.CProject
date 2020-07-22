#------------------------------------------------------#
#- Author: Allen Lee
#- Mail: trainlee1024@gmail.com
#------------------------------------------------------#


# make_file

CC=gcc
file=test.o

run : $(file)
	# make run
	${CC} -o $@ ${file}

#.PHONY: debug
debug: ${file}
	${CC} -g -DDEBUG -o $@ test.c

test.o : test.c
	${CC} -c test.c -o $@

common.o: common.c
	${CC} -c common.c -o $@

#.PHONY: clear
clear: 
	rm *.o run debug
	@echo "OK"
