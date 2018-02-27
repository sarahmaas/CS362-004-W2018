#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <stdlib.h>

// testing supplyCount() function

void asserttrue(int statement);


int main(int argc, char* argv[]) {
    char functionName[] = "supplyCount()";
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
    //  returned value should match 
    
    // run for all supplies
    int i;
    for(i = 0; i < (treasure_map + 1); i++) {
        state.supplyCount[i] = i + 2;
        printf("\tSupply count card supply stack %d:\n", i + 1);
        asserttrue(supplyCount(i, &state) == (i + 2));
    }
    
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