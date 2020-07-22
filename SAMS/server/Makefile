#------------------------------------------------------#
#- Author: Allen Lee
#- Mail: trainlee1024@gmail.com
#------------------------------------------------------#

# make_file

CC=gcc
file=server.o common.o

server : $(file)
	# make run
	${CC} -o $@ ${file} -lpthread

#.PHONY: debug
debug: ${file}
	${CC} -g -DDEBUG -o $@ ${file} -lpthread

test.o : test.c
	${CC} -c test.c -o $@

common.o: common.c
	${CC} -c common.c -o $@

#.PHONY: clear
clear: 
	rm *.o server debug
	@echo "OK"
