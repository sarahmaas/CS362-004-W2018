#include "dominion.h"
#include "dominion_helpers.h"
#include "interface.h"
#include "rngs.h"
#include <stdio.h>
#include <stdlib.h>

// testing great hall card

void asserttrue(int statement);


int main(int argc, char* argv[]) {
    char functionName[] = "great hall card";
    struct gameState state;
    int players = 2;
    int seed = 100;
    int choice1 = 0;    // not used for great hall
    int choice2 = 0;    // not used for great hall
    int choice3 = 0;    // not used for great hall
    int handPos = 0;    
    int bonus = 0;      // not used for great hall
	int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse,
		sea_hag, tribute, smithy};
	printf("____________________________________________________\n");
	printf("Testing %s\n\n", functionName);
	fflush(stdout);
    
    // init game
    initializeGame(players, k, seed, &state);

    // What is the expected behavior??
    //  +1 card
    //  +1 action
    
    int handCountBefore = numHandCards(&state);
    int actionCountBefore = state.numActions;

    printf("\tplay great hall successfully executes:\n");
    fflush(stdout); 
    int played =  (cardEffect(great_hall, choice1, choice2, choice3, &state, handPos, &bonus) == 0);
    asserttrue(played);
    int handCountAfter = numHandCards(&state);
    int actionCountAfter = state.numActions;
    
    printf("\tcorrect number of cards in hand:\n");
    fflush(stdout);
    // +1 new card, -1 discard, net 0
    asserttrue((handCountBefore - handCountAfter) == 0);

    printf("\tcorrect number of actions:\n");
    fflush(stdout);     
    asserttrue((actionCountAfter - actionCountBefore) == 1);

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