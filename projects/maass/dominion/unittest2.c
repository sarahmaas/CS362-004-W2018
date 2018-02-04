#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <stdlib.h>

// testing gainCard() function

void asserttrue(int statement);


int main(int argc, char* argv[]) {
    char functionName[] = "gainCard()";
    struct gameState state;
    int players = 2;
    int seed = 100;
	int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse,
		sea_hag, tribute, smithy};
	printf("____________________________________________________\n");
	printf("Testing %s\n\n", functionName);
	fflush(stdout);
    
    // init game
    initializeGame(players, k, seed, &state);

    // What is the expected behavior??
    // dependent on:
    //  number of buys for the player
    //  cards left
    //  player's money
    // if successful
    //  it should go into the player's hand
    //  the card supply should decrease by one - but do we test this here? I think so.
    //  the player's money should decrease correctly
    //  the player's buys should decrease correctly
    

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