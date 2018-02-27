#########
# Colin Powell
# CS362: Assignment 3
# Makefile
#########


CFLAGS = -Wall -fpic -coverage -lm
GCOV = gcov -bf

rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g  $(CFLAGS)

dominion.o: dominion.h dominion.c rngs.o
	gcc -c dominion.c -g  $(CFLAGS)

playdom: dominion.o playdom.c
	gcc -o playdom playdom.c -g dominion.o rngs.o $(CFLAGS)
#To run playdom you need to entere: ./playdom <any integer number> like ./playdom 10*/
testDrawCard: testDrawCard.c dominion.o rngs.o
	gcc  -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

badTestDrawCard: badTestDrawCard.c dominion.o rngs.o
	gcc -o badTestDrawCard -g  badTestDrawCard.c dominion.o rngs.o $(CFLAGS)

testBuyCard: testDrawCard.c dominion.o rngs.o
	gcc -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

testAll: dominion.o testSuite.c
	gcc -o testSuite testSuite.c -g  dominion.o rngs.o $(CFLAGS)

interface.o: interface.h interface.c
	gcc -c interface.c -g  $(CFLAGS)

runtests: testDrawCard 
	./testDrawCard &> unittestresult.out
	gcov dominion.c >> unittestresult.out
	cat dominion.c.gcov >> unittestresult.out

random_test_helpers.o: random_test_helpers.c random_test_helpers.h
	gcc -c random_test_helpers.c -g $(CFLAGS)

randomtestadventurer.out: random_test_helpers.o rngs.o dominion.o randomtestadventurer.c 
	gcc -o rtadv randomtestadventurer.c dominion.o rngs.o random_test_helpers.o -g ${CFLAGS}
	printf "Result for random tests:\n"  > randomtestadventurer.out
	./rtadv >> randomtestadventurer.out
	printf "\ngcov function coverage results:\n\n"  >> randomtestadventurer.out
	$(GCOV)	dominion.c | grep "adventurerEffect" -A 4 >> randomtestadventurer.out
	printf "\nOverall test coverage results:\n\n" >> randomtestadventurer.out
	cat dominion.c.gcov >> randomtestadventurer.out

randomtestcard1.out: random_test_helpers.o rngs.o dominion.o randomtestcard1.c 
	gcc -o rtc1 randomtestcard1.c dominion.o rngs.o random_test_helpers.o -g ${CFLAGS}
	printf "Result for random tests:\n"  > randomtestcard1.out
	./rtc1 >> randomtestcard1.out
	printf "\ngcov function coverage results:\n\n"  >> randomtestcard1.out
	$(GCOV)	dominion.c | grep "smithyEffect" -A 4 >> randomtestcard1.out
	printf "\nOverall test coverage results:\n\n" >> randomtestcard1.out
	cat dominion.c.gcov >> randomtestcard1.out

randomtestcard2.out: random_test_helpers.o rngs.o dominion.o randomtestcard2.c 
	gcc -o rtc2 randomtestcard2.c dominion.o rngs.o random_test_helpers.o -g ${CFLAGS}
	printf "Result for random tests:\n"  > randomtestcard2.out
	./rtc2 >> randomtestcard2.out
	printf "\ngcov function coverage results:\n\n"  >> randomtestcard2.out
	$(GCOV)	dominion.c | grep "villageEffect" -A 4 >> randomtestcard2.out
	printf "\nOverall test coverage results:\n\n" >> randomtestcard2.out
	cat dominion.c.gcov >> randomtestcard2.out

player: player.c interface.o
	gcc -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)

all: playdom player testDrawCard testBuyCard badTestDrawCard

clean:
	rm -f *.o playdom.exe playdom player player.exe  *.gcov *.gcda *.gcno *.so *.out testDrawCard testDrawCard.exe
