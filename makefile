CFLAGS= -O3 -Wall -ansi

zeruns: zeruns.o file.o eval.o puzzle.o solve.o utils.o stack.o hash.o list.o coord.o
	gcc $(CFLAGS) -o zeruns zeruns.o file.o eval.o puzzle.o solve.o utils.o stack.o hash.o list.o coord.o

zeruns.o: zeruns.c puzzle.h solve.h file.h utils.h 
	gcc $(CFLAGS) -c zeruns.c

solve.o: solve.c solve.h stack.h
	gcc $(CFLAGS) -c solve.c

file.o: file.c file.h
	gcc $(CFLAGS) -c file.c

eval.o: eval.c eval.h
	gcc $(CFLAGS) -c eval.c

puzzle.o: puzzle.c puzzle.h
	gcc $(CFLAGS) -c puzzle.c

hash.o:	hash.c hash.h list.h
	gcc $(CFLAGS) -c hash.c

list.o: list.c list.h
	gcc $(CFLAGS) -c list.c

stack.o: stack.c stack.h
	gcc $(CFLAGS) -c stack.c

utils.o: utils.c utils.h
	gcc $(CFLAGS) -c utils.c

coord.o: coord.c coord.h
	gcc $(CFLAGS) -c coord.c
clean:
	rm -f *.o *.~ zeruns *.gch
