/*
 * Colin Powell
 * CS362: Assignment 3
 * unittest3.c: gainCard unit test
 */

#include "test_helpers.h"
#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define TO_DISCARD 0
#define TO_DECK 1
#define TO_HAND 2

#define TRUE 1
#define FALSE 0

void init_exp_arrays(int exp_decks[], int exp_hands[], int exp_discard[], struct gameState *st);
void output_ret_test(int ret, int exp_ret);

int main() {

    int test, flag;   // counters
    int seed = 1;
    int num_players = 2;
    int pass = TRUE;
    int ret, exp_ret;   // return value, expected return value

    // cards in the game    
    int k[10] = {adventurer, council_room, feast, gardens, mine, remodel,
                 smithy, village, baron, great_hall};

    struct gameState orig_state, test_state;
    initializeGame(num_players, k, seed, &orig_state);

    // expected deck count, hand count, and discard count
    int exp_decks[orig_state.numPlayers];
    int exp_hands[orig_state.numPlayers];
    int exp_discard[orig_state.numPlayers];

    printf("FUNCTION: gainCard()\n\n");

    /* test 1: error checking: return -1 if card not in supply */     
    printf("*TEST: error checking: should return -1, not affect state*\n");
    printf("-------------------------------------------------\n");
    exp_ret = -1;

    // cycle through each test
    for(test = 0; test < 3; test++) {
    
         // reset initial state / test flags / expected card counts
        pass = TRUE;
        memcpy(&test_state, &orig_state, sizeof(struct gameState));
        init_exp_arrays(exp_decks, exp_hands, exp_discard, &test_state);

        if(test == 0) {
            printf("SUB_TEST: testing with card not in game...\n");
            ret = gainCard(treasure_map, &test_state, TO_DISCARD, test_state.whoseTurn);
        }
        else if(test == 1) {
            printf("SUB_TEST: testing with card having empty supply...\n");
            test_state.supplyCount[estate] = 0;
            ret = gainCard(estate, &test_state, TO_DISCARD, test_state.whoseTurn);
        }
        else {
            printf("SUB_TEST: testing with invalid TO flag...\n");
            ret = gainCard(estate, &test_state, 3, test_state.whoseTurn);
        }

        // check that return value matches expected - fail test otherwise
        pass = (ret == exp_ret) ? pass : FALSE;
        output_ret_test(ret, exp_ret);

        // assert that deck, discard, hand counts match expectations 
        if(test_fulldeck_counts(exp_decks, exp_hands, exp_discard, &test_state) == FALSE) {
            pass = FALSE;
        }

        output_test_result(pass);
        printf("\n");
    }

    /* test 2: toFlag = 0: card added to discard, no other changes  */     

    printf("\n*TEST: valid cards gained to deck, hand, discard*\n");
    printf("-----------------------------------------------\n");
    exp_ret = 0;

    // cycle through each "to_flag" value
    for(flag = 0; flag < 3; flag++) {

        // reset test state for each run
        memcpy(&test_state, &orig_state, sizeof(struct gameState));
        init_exp_arrays(exp_decks, exp_hands, exp_discard, &test_state);
        pass = TRUE;

        if(flag == 0) {
            printf("SUB_TEST: gain to discard\n");
            exp_discard[test_state.whoseTurn]++;
        }
        else if(flag == 1) {
            printf("SUB_TEST: gain to deck\n");
            exp_decks[test_state.whoseTurn]++;
        }
        else if(flag == 2) {
            printf("SUB_TEST: gain to hand\n");
            exp_hands[test_state.whoseTurn]++;
        }

        // compare return value to expected
        ret = gainCard(adventurer, &test_state, flag, test_state.whoseTurn);
        pass = (ret == exp_ret) ? pass : FALSE;
        output_ret_test(ret, exp_ret);

        // assert that deck, discard, hand counts match expectations 
        if(test_fulldeck_counts(exp_decks, exp_hands, exp_discard, &test_state) == FALSE) {
            pass = FALSE;
        }
        // print overall results
        output_test_result(pass);
        printf("\n");
    }
     
    return 0;

}

/*
 * init_exp_arrays: initialize "expected card count" arrays, which contain
 * expected count of cards in deck, hand, and discard piles for each
 * player. initialize to current content of deck / hand / discard for
 * each player.
 */

void init_exp_arrays(int exp_decks[], int exp_hands[], int exp_discard[], struct gameState *st) {

    int i = 0;
    for(i = 0; i < st->numPlayers; i++) {
        exp_decks[i] = st->deckCount[i];
        exp_hands[i] = st->handCount[i];
        exp_discard[i] = st->discardCount[i];
    }

}


/*
 * print_ret_test: prints the result of checking the return value of the function
 */

void output_ret_test(int ret, int exp_ret) {

    if(ret != exp_ret) {
        printf("--ERROR: return value: %d, expected: %d\n", ret, exp_ret);
    }
    else {
        printf("--OK: return value matches\n");
    }

}
