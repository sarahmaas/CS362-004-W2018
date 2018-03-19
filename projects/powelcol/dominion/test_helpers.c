/*
 * Colin Powell
 * CS362: Assignment 3
 * test_helpers.c: helper functions for unit tests
 * used to build unit test reports
 */


#include "test_helpers.h"

/*
 * output_test_result: outputs the result of a specific 
 * test; pass is an int representing the overall state
 * of the test - 0 if the test failed, and 1 if the 
 * test passed.
 */

void output_test_result(int pass) {
    
    if( pass ) { 
        printf(">>> TEST RESULT: SUCCESS\n");
    }
    else {
        printf(">>> TEST RESULT: FAILED\n");
    }

}

/*
 * test_supply_count: test supply against expected, output to stdout. 
 * expected[] is an array of expected supply cards, st is a pointer to
 * the state to test against expectations.
 */

int test_supply_counts(int expected [], struct gameState *st) {

    int i = 0;
    int good_supply = TRUE;
    int supp_size = sizeof(st->supplyCount) / sizeof(st->supplyCount[0]);

    // no unexpected should occur to any kingdom/victory card piles
    printf("verifying supply state matches expectations...\n");

    for(i = 0; i < supp_size; i++ ) {
        if(st->supplyCount[i] != expected[i]) {
            printf("--supply pos %i: observed, %d, expected: %d\n", 
                    i, st->supplyCount[i], expected[i]);
            good_supply = FALSE;
        }
    }

    if(good_supply){
        printf("--no unexpected changes to supply - OK\n");
    }

    return good_supply;

}

/*
 * test_fulldeck_counts
 * -test each player's hand count, deck count, discard count
 * -against expected values, report TRUE (1) on success, 
 * -and FALSE (0) on failure. exp_decks[] contains expected
 * -deck counts for each player, exp_hands[] contains expected
 * -hand counts for each player, and exp_discard[] contains
 * -expected discard counts for each player. st is a pointer to
 * -the game state to test against. 
 */

int test_fulldeck_counts(int exp_decks[], int exp_hands[], 
                         int exp_discard[], struct gameState *st) {

    int i = 0;
    int pass = TRUE;
    
    // test conditions for other players: should be no change
    printf("all players - checking hand/deck counts:\n");
    for(i = 0; i < st->numPlayers; i++){

        printf("-player %d:\n", i);
        // ensure that hand count matches expectations 
        if( st->handCount[i] != exp_hands[i] ) {
            pass = FALSE;
        }
        printf("--hand count: %d, expected %d\n",
                st->handCount[i], exp_hands[i]);

        // ensure that deck count matches expectations 
        if( st->deckCount[i] != exp_decks[i] ) {
            pass = FALSE;
        }
        printf("--deck count: %d, expected %d\n",
                st->deckCount[i], exp_decks[i]);

        if( st->discardCount[i] != exp_discard[i] ) {
            pass = FALSE;
        }
        // ensure that discard count matches expectations 
        printf("--discard count: %d, expected %d\n",
                st->discardCount[i], exp_discard[i]);

    } 

    return pass;
}

/*
 * test_playedcard_count():
 * -test played cards against expected played card count (exp_count), using st 
 * -as the means of comparison. return TRUE (1) if exp_count matches, 
 * -and FALSE (0) if there is a discrepancy. 
 */

int test_playedcard_count(int exp_count, struct gameState *st) {

//    int i;
    int pass = TRUE;

    printf("checking played card count:\n");
    if( exp_count != st->playedCardCount ) {
        pass = FALSE;
        printf("--mismatch: played cards: %d, expected %d\n", st->playedCardCount, exp_count);
    }
    else  {
        printf("--played count matches - OK\n");
    }

    return pass;
}

/*
 * output_card_list(): 
 * -output the IDs of the cards in deck, hand, and discard for
 * -the player ID passed as a parameter. 
 * -st is a pointer to the game state, and plyr is the player's ID
 */

void output_card_list(int plyr, struct gameState *st) {

    int i = 0;

    printf("player %d\n", plyr);
    
    // output hand, deck, discard contents (as integers)

    printf("--hand: ");
    for(i = 0; i < st->handCount[plyr]; i++) {
        printf("(%d)", st->hand[plyr][i]);
    }

    printf("\n--deck: ");
    for(i = 0; i < st->deckCount[plyr]; i++) {
        printf("(%d)", st->deck[plyr][i]);
    }
    printf("\n--discard: ");
    for(i = 0; i < st->discardCount[plyr]; i++) {
        printf("(%d)", st->discard[plyr][i]);
    }

    printf("\n"); // output newline for convenience
}


/*
 * output_supply: 
 * -outputs cards / counts for the given game state. 
 * -k is an array containing the kingdom cards in the game
 * -num_k is the count of kingdon cards
 * -st is a pointer to the game state
 */

void output_supply(int k[], int num_k, struct gameState *st) {

    int i = 0;

    // print treasure cards
    printf("cards in game:\t");
    for(i = 0; i < adventurer; i++) {
        printf("(%2d)", i);
    }

    // print kingdom cards in game
    for(i = 0; i < num_k; i++) {
        printf("(%2d)", k[i]);
    }
    
    // print treasure card amounts
    printf("\ncard amounts:\t");
    for(i = 0; i < adventurer; i++) {
        printf("(%2d)", st->supplyCount[i]);
    }

    // print kingdom card amounts
    for(i = 0; i < num_k; i++) {
        printf("(%2d)", st->supplyCount[ k[i] ]);
    }
    
    printf("\n");
}


