#######
# Colin Powell
# CS362: Assignment 3
# Makefile
#######

## NOTE: gcov is cumulative, and so my method for creating unittestresults.out is as follows:
# - perform each unit test, then call gcov, piping the overall result for the 
#	function ONLY to unittestresults.out
# - remove the .gcda file associated with dominion.c to destroy any accumulated coverage data
# - repeat for each unit test
# - finally, run each unit test again, collecting cumulative coverage data from gcov, redirecting 
# - any output from these unit tests to dev/null 
# - call gcov on dominion.c, outputting the entire, cumulative test.
# This method was developed from posts on the class Piazza as well as discussion on Slack, and 
# is intended to show the specific coverage resulting from each individual unit test, as well
# as the cumulative coverage from all unit tests - both are useful in evaluating coverage. 
## 


CFLAGS = -Wall -fpic -coverage -lm -O0
func_unit_tests = unittest1.c unittest2.c unittest3.c unittest4.c
card_unit_tests = cardtest1.c cardtest2.c cardtest3.c cardtest4.c
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

player: player.c interface.o
	gcc -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)

test_helpers.o: 
	gcc -c test_helpers.c -g $(CFLAGS)

unittestresults.out: $(func_unit_tests) $(card_unit_tests) dominion.o rngs.o test_helpers.o
	gcc -o ut1 unittest1.c dominion.o rngs.o test_helpers.o -g $(CFLAGS) 	
	gcc -o ut2 unittest2.c dominion.o rngs.o test_helpers.o -g $(CFLAGS)
	gcc -o ut3 unittest3.c dominion.o rngs.o test_helpers.o -g $(CFLAGS)
	gcc -o ut4 unittest4.c dominion.o rngs.o test_helpers.o -g $(CFLAGS)
	gcc -o ct1 cardtest1.c dominion.o rngs.o test_helpers.o -g $(CFLAGS)
	gcc -o ct2 cardtest2.c dominion.o rngs.o test_helpers.o -g $(CFLAGS)
	gcc -o ct3 cardtest3.c dominion.o rngs.o test_helpers.o -g $(CFLAGS)
	gcc -o ct4 cardtest4.c dominion.o rngs.o test_helpers.o -g $(CFLAGS)

	printf "Result for unit tests:\n" > unittestresults.out

	printf "\nunittest1.c:\n\n" >> unittestresults.out
	./ut1 >> unittestresults.out
	printf "\ngcov function coverage results:\n" >> unittestresults.out
	$(GCOV) dominion.c | grep "scoreFor" -A 4 >> unittestresults.out	
	rm -f dominion.c.gcda
 
	printf "\n\nunittest2.c:\n\n" >> unittestresults.out
	./ut2 >> unittestresults.out
	printf "\ngcov function coverage results:\n" >> unittestresults.out
	$(GCOV) dominion.c | grep "isGameOver" -A 4 >> unittestresults.out	
	rm -f dominion.c.gcda

	printf "\n\nunittest3.c:\n\n" >> unittestresults.out
	./ut3 >> unittestresults.out
	printf "\ngcov function coverage results:\n" >> unittestresults.out
	$(GCOV) dominion.c | grep "gainCard" -A 4 >> unittestresults.out	
	rm -f dominion.c.gcda

	printf "\n\nunittest4.c:\n\n" >> unittestresults.out
	./ut4 >> unittestresults.out
	printf "\ngcov function coverage results:\n" >> unittestresults.out
	$(GCOV) dominion.c | grep "fullDeckCount" -A 4 >> unittestresults.out	
	rm -f dominion.c.gcda

	printf "\n\ncardtest1.c:\n\n" >> unittestresults.out
	./ct1 >> unittestresults.out
	printf "\ngcov function coverage results:\n" >> unittestresults.out
	$(GCOV) dominion.c | grep "smithyEffect" -A 4 >> unittestresults.out	
	rm -f dominion.c.gcda

	printf "\n\ncardtest2.c:\n\n" >> unittestresults.out
	./ct2 >> unittestresults.out
	printf "\ngcov function coverage results:\n" >> unittestresults.out
	$(GCOV) dominion.c | grep "adventurerEffect" -A 4 >> unittestresults.out	
	rm -f dominion.c.gcda

	printf "\n\ncardtest3.c:\n\n" >> unittestresults.out
	./ct3 >> unittestresults.out
	printf "\ngcov function coverage results:\n" >> unittestresults.out
	$(GCOV) dominion.c | grep "mineEffect" -A 4 >> unittestresults.out	
	rm -f dominion.c.gcda

	printf "\n\ncardtest4.c:\n\n" >> unittestresults.out
	./ct4 >> unittestresults.out
	printf "\ngcov function coverage results:\n" >> unittestresults.out
	$(GCOV) dominion.c | grep "villageEffect" -A 4 >> unittestresults.out	
	rm -f dominion.c.gcda

	./ut1 >> /dev/null
	./ut2 >> /dev/null
	./ut3 >> /dev/null
	./ut4 >> /dev/null
	./ct1 >> /dev/null
	./ct2 >> /dev/null
	./ct3 >> /dev/null
	./ct4 >> /dev/null

	printf "\n\noverall gcov results:\n\n" >> unittestresults.out
	$(GCOV) dominion.c >> unittestresults.out
	cat dominion.c.gcov >> unittestresults.out

all: playdom player testDrawCard testBuyCard badTestDrawCard

clean:
	rm -f *.o playdom.exe playdom player player.exe  *.gcov *.gcda *.gcno *.so *.out testDrawCard testDrawCard.exe
