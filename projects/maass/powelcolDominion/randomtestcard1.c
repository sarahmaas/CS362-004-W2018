#include "dominion.h"
#include "dominion_helpers.h"
#include "interface.h"
#include "rngs.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_TESTS 1000

// testing smithy card

int asserttrue(int statement);


int main(int argc, char* argv[]) {
	time_t t;
    // Intializes random number generator 
    srand((unsigned) time(&t));
    int handSuccess = 0;
    int deckSuccess = 0;
    
    char functionName[] = "smithy card";
	printf("____________________________________________________________________\n");
	printf("Testing %s\n\n", functionName);
	fflush(stdout);
    int i;
    for (i = 0; i < NUM_TESTS; i++) {    
        struct gameState state;
        int players = 2;
        int seed = rand();
        int choice1 = 0;    // not used for smithy
        int choice2 = 0;    // not used for smithy
        int choice3 = 0;    // not used for smithy
        int handPos = 0;
        int bonus = 0;      // not used for smithy
    	int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse,
    		sea_hag, tribute, smithy};
        // init game
        initializeGame(players, k, seed, &state);
    
        // What is the expected behavior??
        //  +3 cards...from the deck
        //  check that hand has increased and deck has decreased  
        //  card is discarded at the end, so net +2
        int handCountBefore = numHandCards(&state);
        int deckCountBefore = state.deckCount[0];
        // int discardCountBefore = state.discardCount[0];
        int played =  (cardEffect(smithy, choice1, choice2, choice3, &state, handPos, &bonus) == 0);
        asserttrue(played);
        int handCountAfter = numHandCards(&state);
        int deckCountAfter = state.deckCount[0];
        // int discardCountAfter = state.discardCount[0];
        
        // 2 because + 3 and we played 1.
        handSuccess += asserttrue((handCountAfter - handCountBefore) == 2);
    
        // 4 because we drew 3, would have to add for discard if 
        // that was being set - may be a bug for future investigation
        deckSuccess += asserttrue((deckCountBefore - deckCountAfter) == 4);
    }
	// tallied results
    printf("\tcorrect number of cards in hand: %d of %d times.\n", handSuccess, NUM_TESTS);
    fflush(stdout); 	
    printf("\tcorrect number of cards in deck: %d of %d times.\n", deckSuccess, NUM_TESTS);
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