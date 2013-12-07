all:thread_test.o 
thread_test.o : thread_test.c /usr/include/apue.h
	gcc -c thread_test.c
clean:
	rm thread_test.o
