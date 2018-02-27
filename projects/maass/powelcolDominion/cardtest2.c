#include "dominion.h"
#include "dominion_helpers.h"
#include "interface.h"
#include "rngs.h"
#include <stdio.h>
#include <stdlib.h>

// testing adventurer card

void asserttrue(int statement);


int main(int argc, char* argv[]) {
    char functionName[] = "adventurer card";
    struct gameState state;
    int players = 2;
    int seed = 100;
    int choice1 = 0;    // not used for adventurer
    int choice2 = 0;    // not used for adventurer
    int choice3 = 0;    // not used for adventurer
    int handPos = 0;    // not used for adventurer
    int bonus = 0;      // not used for adventurer
	int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse,
		sea_hag, tribute, smithy};
	printf("____________________________________________________\n");
	printf("Testing %s\n\n", functionName);
	fflush(stdout);
    
    // init game
    initializeGame(players, k, seed, &state);

    // What is the expected behavior??
    //  +2 treasure if they exist

    int handCountBefore = numHandCards(&state);

    printf("\tplay adventurer successfully executes:\n");
    fflush(stdout); 
    int played =  (cardEffect(adventurer, choice1, choice2, choice3, &state, handPos, &bonus) == 0);
    asserttrue(played);
    int handCountAfter = numHandCards(&state);
    
    printf("\tcorrect number of cards in hand:\n");
    fflush(stdout);     
    asserttrue((handCountAfter - handCountBefore) == 2);

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