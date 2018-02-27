#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <stdlib.h>

// testing kingdomCards() function

void asserttrue(int statement);


int main(int argc, char* argv[]) {
    char functionName[] = "kingdomCards()";
	int control[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	printf("____________________________________________________\n");
	printf("Testing %s\n\n", functionName);
	fflush(stdout);

    // What is the expected behavior??
    //  returns an int array that matches what was given
    int* test = kingdomCards(0, 1, 2, 3, 4, 5, 6, 7, 8, 9);
    // test all positions
    int i;
    for (i = 0; i < 10; i++) {
        printf("\ttest position %d:\n", i);
        fflush(stdout);
        asserttrue(control[i] == *(test + i));  
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