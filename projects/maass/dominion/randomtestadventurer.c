#include "dominion.h"
#include "dominion_helpers.h"
#include "interface.h"
#include "rngs.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_TESTS 500

// testing adventurer card

int asserttrue(int statement);


int main(int argc, char* argv[]) {
	time_t t;
    // Intializes random number generator 
    srand((unsigned) time(&t));
    int execSuccess = 0;
    int cardSuccess = 0;
        
    char functionName[] = "adventurer card";    
	printf("____________________________________________________________________\n");
	printf("Testing %s\n\n", functionName);
	fflush(stdout);
    int i;
    for (i = 0; i < NUM_TESTS; i++) {  
        struct gameState state;
        int players = 2;    // must be 2
        int seed = rand();
        int choice1 = 0;    // not used for adventurer
        int choice2 = 0;    // not used for adventurer
        int choice3 = 0;    // not used for adventurer
        int handPos = 0;    // not used for adventurer
        int bonus = 0;      // not used for adventurer
    	int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse,
    		sea_hag, tribute, smithy};

        // init game
        initializeGame(players, k, seed, &state);
    
        // What is the expected behavior??
        //  +2 treasure if they exist
    
        int handCountBefore = numHandCards(&state);
    
        // printf("\tplay adventurer successfully executes:\n");
        // fflush(stdout); 
        int played =  (cardEffect(adventurer, choice1, choice2, choice3, &state, handPos, &bonus) == 0);
        execSuccess += asserttrue(played);
        int handCountAfter = numHandCards(&state);
        
        // printf("\tcorrect number of cards in hand:\n");
        // fflush(stdout);     
        cardSuccess += asserttrue((handCountAfter - handCountBefore) == 2);
    }
	// tallied results
    printf("\tplay adventurer successfully executed: %d of %d times.\n", execSuccess, NUM_TESTS);
    fflush(stdout); 	
    printf("\tcorrect number of cards in hand: %d of %d times.\n", cardSuccess, NUM_TESTS);
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