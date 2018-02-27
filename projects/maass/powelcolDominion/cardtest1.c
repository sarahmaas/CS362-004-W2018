#include "dominion.h"
#include "dominion_helpers.h"
#include "interface.h"
#include "rngs.h"
#include <stdio.h>
#include <stdlib.h>

// testing smithy card

void asserttrue(int statement);


int main(int argc, char* argv[]) {
    char functionName[] = "smithy card";
    struct gameState state;
    int players = 2;
    int seed = 100;
    int choice1 = 0;    // not used for smithy
    int choice2 = 0;    // not used for smithy
    int choice3 = 0;    // not used for smithy
    int handPos = 0;
    int bonus = 0;      // not used for smithy
	int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse,
		sea_hag, tribute, smithy};
	printf("____________________________________________________\n");
	printf("Testing %s\n\n", functionName);
	fflush(stdout);
    
    // init game
    initializeGame(players, k, seed, &state);

    // What is the expected behavior??
    //  +3 cards...from the deck
    //  check that hand has increased and deck has decreased  
    //  card is discarded at the end, so net +2
    int handCountBefore = numHandCards(&state);
    int deckCountBefore = state.deckCount[0];
    // int discardCountBefore = state.discardCount[0];
    printf("\tplay smithy successfully executes:\n");
    fflush(stdout); 
    int played =  (cardEffect(smithy, choice1, choice2, choice3, &state, handPos, &bonus) == 0);
    asserttrue(played);
    int handCountAfter = numHandCards(&state);
    int deckCountAfter = state.deckCount[0];
    // int discardCountAfter = state.discardCount[0];
    
    printf("\tcorrect number of cards in hand:\n");
    fflush(stdout);     
    // 2 because + 3 and we played 1.
    asserttrue((handCountAfter - handCountBefore) == 2);

    printf("\tcorrect number of cards in deck:\n");
    fflush(stdout);
    // 4 because we drew 3, would have to add for discard if 
    // that was being set - may be a bug for future investigation
    asserttrue((deckCountBefore - deckCountAfter) == 4);

	printf("____________________________________________________\n");
	
    return 0;
}



// adding own asserttrue as recommended
void asserttrue(int statement) {
    if (statement) {
        printf("\t\tTest successfully completed.\n");
        fflush(stdout);
    } else {
        printf("\t\tTest failed.\n");
        fflush(stdout);
    }
}