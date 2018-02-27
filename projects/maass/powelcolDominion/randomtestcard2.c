#include "dominion.h"
#include "dominion_helpers.h"
#include "interface.h"
#include "rngs.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_TESTS 1000

// testing embargo card

int asserttrue(int statement);


int main(int argc, char* argv[]) {
	time_t t;
    // Intializes random number generator 
    srand((unsigned) time(&t));
    int execSuccess = 0;
    int tokenSuccess = 0;
    int cardSuccess = 0;
    int coinSuccess = 0;
    
    char functionName[] = "embargo card";
	printf("____________________________________________________________________\n");
	printf("Testing %s\n\n", functionName);
	fflush(stdout);
    int i;
    for (i = 0; i < NUM_TESTS; i++) {  	
        struct gameState state;
        int players = 2;
        int seed = rand();
        int choice1 = rand() % (treasure_map + 2);    
        int choice2 = 0;    // not used for embargo
        int choice3 = 0;    // not used for embargo
        int handPos = 0;    
        int bonus = 0;      // not used for embargo
    	int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse,
    		sea_hag, tribute, smithy};
    		
        // init game
        initializeGame(players, k, seed, &state);
    
        // What is the expected behavior??
        //  +1 embargo token
        //  -1 in hand (from trashing card)
        //  +2 coins 
        
        int embargoCountBefore = state.embargoTokens[choice1];
        int handCountBefore = numHandCards(&state);
        int cointCountBefore = state.coins;
    
        int played =  (cardEffect(embargo, choice1, choice2, choice3, &state, handPos, &bonus) == 0);
        execSuccess += asserttrue(played);
        int embargoCountAfter = state.embargoTokens[choice1];
        int handCountAfter = numHandCards(&state);
        int cointCountAfter = state.coins;
     
        tokenSuccess += asserttrue((embargoCountAfter - embargoCountBefore) == 1);
        
        cardSuccess += asserttrue((handCountBefore - handCountAfter) == 1);
    
        coinSuccess += asserttrue((cointCountAfter - cointCountBefore) == 2);
    }
	// tallied results
    printf("\tplay embargo successfully executes: %d of %d times.\n", execSuccess, NUM_TESTS);
    fflush(stdout); 	
    printf("\tcorrect number of embargo tokens: %d of %d times.\n", tokenSuccess, NUM_TESTS);
    fflush(stdout); 	
    printf("\tcorrect number of cards in hand: %d of %d times.\n", cardSuccess, NUM_TESTS);
    fflush(stdout); 	
    printf("\tcorrect number of coins: %d of %d times.\n", coinSuccess, NUM_TESTS);
    fflush(stdout); 	
 
	printf("____________________________________________________________________\n");
	
    return 0;
}



// adding own asserttrue as recommended
int asserttrue(int statement) {
    if (statement) {
        return TRUE;
    } else {
        return FALSE;
    }
}
