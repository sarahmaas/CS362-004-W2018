#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <stdlib.h>

// testing isGameOver() function

void asserttrue(int statement);


int main(int argc, char* argv[]) {
    char functionName[] = "isGameOver()";
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
    
	// conditions that must be true, one or the other:
	//   province count must be zero
	//   3 supply piles are at zero

    // test province count
    state.supplyCount[province] = 0; 
    printf("\tNo provinces left:\n");
    fflush(stdout);
    asserttrue(isGameOver(&state));

    // reset changes
    state.supplyCount[province] = 1;

    // test supply pile count
    // set 3 to 0, set rest to more
    // from enum CARD in dominion.h
    int card;
    for(card = curse; card <= treasure_map; card++) {
        state.supplyCount[card] = ((card < 3) ? 0 : 5);
        //printf("Testing test: %d\n", state.supplyCount[card]);
    }
    printf("\t3 empty piles:\n");
    fflush(stdout);    
    asserttrue(isGameOver(&state));	
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