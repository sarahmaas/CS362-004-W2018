CFLAGS = -Wall -fpic -coverage -lm

testme: testme.c
	gcc -o testme testme.c $(CFLAGS)

all: testme

default: testme

clean:
	rm -f *.o testme  *.gcov *.gcda *.gcno *.so *.out 
