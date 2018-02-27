#include "dominion.h"
#include "dominion_helpers.h"
#include "interface.h"
#include "rngs.h"
#include <stdio.h>
#include <stdlib.h>

// testing embargo card

void asserttrue(int statement);


int main(int argc, char* argv[]) {
    char functionName[] = "embargo card";
    struct gameState state;
    int players = 2;
    int seed = 100;
    int choice1 = 0;    
    int choice2 = 0;    // not used for embargo
    int choice3 = 0;    // not used for embargo
    int handPos = 0;    
    int bonus = 0;      // not used for embargo
	int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse,
		sea_hag, tribute, smithy};
	printf("____________________________________________________\n");
	printf("Testing %s\n\n", functionName);
	fflush(stdout);
    
    // init game
    initializeGame(players, k, seed, &state);

    // What is the expected behavior??
    //  +1 embargo token
    //  -1 in hand (from trashing card)
    //  +2 coins 
    
    int embargoCountBefore = state.embargoTokens[choice1];
    int handCountBefore = numHandCards(&state);
    int cointCountBefore = state.coins;

    printf("\tplay embargo successfully executes:\n");
    fflush(stdout); 
    int played =  (cardEffect(embargo, choice1, choice2, choice3, &state, handPos, &bonus) == 0);
    asserttrue(played);
    int embargoCountAfter = state.embargoTokens[choice1];
    int handCountAfter = numHandCards(&state);
    int cointCountAfter = state.coins;
 
    printf("\tcorrect number of embargo tokens:\n");
    fflush(stdout);     
    asserttrue((embargoCountAfter - embargoCountBefore) == 1);
    
    printf("\tcorrect number of cards in hand:\n");
    fflush(stdout);     
    asserttrue((handCountBefore - handCountAfter) == 1);

    printf("\tcorrect number of coins:\n");
    fflush(stdout);     
    asserttrue((cointCountAfter - cointCountBefore) == 2);

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