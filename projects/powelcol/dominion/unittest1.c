/*
 * Colin Powell
 * CS362: Assignment 3
 * unittest1.c: scoreFor unit test
 */

#include "test_helpers.h"
#include "dominion.h"
#include "rngs.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define TRUE 1
#define FALSE 0

#define NUM_TESTS 9 
#define NUM_PILES 3
#define MAX_PILE_SIZE 6 

int get_pile_count(int cards[], int max_count);

int main() {

    int i, j, test;
    int score;//, expected_score;

    // cards in the game    
    int k[10] = {adventurer, council_room, feast, gardens, mine, remodel,
                 smithy, village, baron, great_hall};

    int pass = TRUE;    // flag for expected pass/fail state

    // test_params RULES:
    // - for each test, define a set of "piles" (card groups), and the 
    // - cards that should be in that pile.
    // pile idx 0: hand, pile idx 1: deck, pile idx 2: discard
    // to vary pile sizes, place -1 in final indices: must be at end.

    int test_params[NUM_TESTS][NUM_PILES][MAX_PILE_SIZE] = 
        {  
            // test no value in hand, varied values in deck + discard 
            {  {gold, gold, copper, copper, silver, silver},
               {curse, estate, duchy, province, estate, gardens}, 
               {curse, estate, duchy, province, province, gardens}  },

            // test one of each type in hand + discard, none in deck 
            {  {curse, estate, duchy, province, province, gardens}, 
               {gold, gold, copper, copper, silver, silver},
               {curse, estate, duchy, province, estate, gardens} },

            // test one of each type in deck, hand, discard 
            {  {curse, estate, duchy, province, great_hall, gardens},
               {curse, estate, duchy, province, great_hall, gardens}, 
               {curse, estate, duchy, province, great_hall, gardens}  },
            
            // test same, but reversed
            {  {gardens, great_hall, province, duchy, estate, curse},
               {gardens, great_hall, province, duchy, estate, curse},
               {gardens, great_hall, province, duchy, estate, curse} },

            // test with single garden in each pile, no other value cards
            {  {gardens, gold, copper, copper, silver, silver}, 
               {gardens, gold, copper, copper, silver, silver}, 
               {gardens, gold, copper, copper, silver, silver}, },

            // test with single garden in each pile ONLY
            {  {gardens, -1, -1, -1, -1, -1},
               {gardens, -1, -1, -1, -1, -1}, 
               {gardens, -1, -1, -1, -1, -1}    },
            
            // test with no gardens in any pile
            {  {duchy, gold, copper, copper, silver, silver}, 
               {province, gold, copper, copper, silver, silver}, 
               {estate, gold, copper, copper, silver, silver}, },

            // vary pile sizes: no value in hand, varied values in deck + discard
            {  {gold, gold, copper, copper, silver, silver}, 
               {gardens, great_hall, duchy, province, -1, -1}, 
               {duchy, province, great_hall, -1, -1, -1}    },

            // vary pile sizes: varied values in hand and deck, no value in discard 
            {  {duchy, province, duchy, duchy, -1, -1},
               {gardens, great_hall, duchy, -1, -1, -1}, 
               {copper, copper, copper, copper, gold, silver}    }
        };

    // set expected scores for each of the tests enumerated above
    int expected_scores[NUM_TESTS] = {27, 27, 33, 33, 3, 0, 10, 21, 20};

    struct gameState orig_state, test_state;

    int seed = 1;
    int num_players = 1;

    initializeGame(num_players, k, seed, &orig_state);

    /* test that individual card scores accurately counted */ 

    printf("FUNCTION: scoreFor()\n\n");

    printf("*TEST check that score accurately calculated for all cards*\n\n");
   
    // cycle through tests 
    for(test = 0; test < NUM_TESTS; test++) {

        // copy game state into test
        memcpy(&test_state, &orig_state, sizeof(struct gameState)); 

        printf("SUB-TEST: %d\n", test + 1);
        printf("------------\n");

        // set hand, deck, discard counts based on test params
        for(i = 0; i < num_players; i++) {

            test_state.handCount[i] = get_pile_count( test_params[test][0], MAX_PILE_SIZE ); 
            test_state.deckCount[i] = get_pile_count( test_params[test][1], MAX_PILE_SIZE ); 
            test_state.discardCount[i] = get_pile_count( test_params[test][2], MAX_PILE_SIZE ); 

            // set hand, deck, discard contents based on test params
            for(j = 0; j < MAX_PILE_SIZE; j++) {

                test_state.hand[i][j] = test_params[test][0][j]; 
                test_state.deck[i][j] = test_params[test][1][j]; 
                test_state.discard[i][j] = test_params[test][2][j];
            }

            // output card list for testing, then calc. score, report results
            output_card_list(i, &test_state);
            score = scoreFor(i, &test_state);

            printf("checking score against expected score of %d...\n", expected_scores[test]);
            if(score != expected_scores[test]){ 
                printf("--ERROR: result: %d\n", score);
                pass = FALSE;
            }
            else {
                printf("--OK: score matches\n");
            }

            printf("\n");
        }

    }
   
    // output overall test result 
    output_test_result(pass);

    return 0;

}


/*
 * get_pile_count(): looks through a specific pile in the test_params array, 
 * returns the number of cards in that pile.
 */

int get_pile_count(int cards[], int max_count) {

    int i = 0;
    while( (cards[i] >= 0) && (i < max_count) ) {
        i++;
    }

    return i;
}

