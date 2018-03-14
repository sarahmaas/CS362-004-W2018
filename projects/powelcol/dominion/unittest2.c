/* 
 * Colin Powell
 * CS362: Assignment 3
 * unitttest.2: isGameOver unit tests
 */

#include "test_helpers.h"
#include "rngs.h"
#include "dominion.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>


#define TRUE 1
#define FALSE 0

int do_game_over_test(int exp_ret, struct gameState *st);

int main() {
    
    int i, test;
    int num_players = 2;
    int seed = 1;

    int exp_ret;    // expected / actual return value for isGameOver
   
    int num_k = 10;
    int k[10] = {adventurer, council_room, feast, gardens, mine, remodel,
                 smithy, village, baron, great_hall};
    
    int pass = TRUE;    

    struct gameState orig_state, test_state;
    initializeGame(num_players, k, seed, &orig_state);

    printf("FUNCTION: isGameOver()\n\n");

    printf("*TEST: Game Over accurately recognized*\n");
    exp_ret = 1;

    // testing that 1 is properly returned, signaling game over
    for(test = 0; test < 3; test++) {
    
        memcpy(&test_state, &orig_state, sizeof(struct gameState));
        pass = TRUE;
    
        // make sure that 3 valid empty non-province cards end game 
        if(test == 0) {
            printf("\nSUB_TEST: supply of 3 in-game cards are empty, NOT province\n");
            test_state.supplyCount[council_room] = 0;
            test_state.supplyCount[great_hall] = 0;    
            test_state.supplyCount[curse] = 0;

            output_supply(k, num_k, &test_state);    
        }
        // make sure that an empty province ends the game 
        else if(test == 1) {
            printf("\nSUB_TEST: ONLY province is empty\n");
            test_state.supplyCount[province] = 0;

            output_supply(k, num_k, &test_state);    
        }
        // make sure that empty treasure map is recognized
        else if(test == 2) {
            printf("\nSUB_TEST: empty treasure map card is recognized when use in game\n");
            // put treasure map into game, set its supply count to 0.
            k[9] = treasure_map;

            test_state.supplyCount[treasure_map] = 0;
            test_state.supplyCount[curse] = 0;
            test_state.supplyCount[estate] = 0;
    
            // output supply, then reset to previous kingdom card
            output_supply(k, num_k, &test_state);    
            k[9] = great_hall;
        }

        pass = do_game_over_test(exp_ret, &test_state);
        output_test_result(pass);

    }

    /* test 2 valid supply cards empty,  */
    printf("\n*TEST: supply of 2 cards are empty, NOT province*\n");
    memcpy(&test_state, &orig_state, sizeof(struct gameState));
    exp_ret = 0;
    
    test_state.supplyCount[curse] = 0;
    test_state.supplyCount[adventurer] = 0;

    output_supply(k, num_k, &test_state);    
    pass = do_game_over_test(exp_ret, &test_state);
    output_test_result(pass);

    /* test 3 supply cards NOT IN GAME are empty,  */
    printf("\n*TEST: supply of 3 cards NOT in game are empty*\n");
    memcpy(&test_state, &orig_state, sizeof(struct gameState));
    exp_ret = 0;

    // initializeGame will set supply count of unused cards to -1
    // but we can't assume that it is called in every game:
    // so, manually setup supplyCount:

    for(i = 0; i < treasure_map; i++) {
        test_state.supplyCount[i] = 0;
    }
   
   // set up "dummy" values for supply-all card types used in this game
    for(i = 0; i <= great_hall; i++) {
       test_state.supplyCount[i] = 1; 
    }

    output_supply(k, num_k, &test_state);    
    pass = do_game_over_test(exp_ret, &test_state);
    output_test_result(pass);


    return 0;
}

/*
 * do_game_over: performs a single unit test on the isGameOver
 * function. receives exp_ret, the expected return value, and
 * st, a pointer to the test game state. returns TRUE on success, 
 * FALSE on failure.
 */

int do_game_over_test(int exp_ret, struct gameState *st) {

    int pass = TRUE;
    int ret; 
  
    printf("calling isGameOver: expected return value %d\n", exp_ret);
    ret = isGameOver(st);
    if(ret != exp_ret) {
        pass = FALSE; 
        printf("--ERROR: return value %d\n", ret); 
    }
    else {
        printf("--OK: return value matches. OK\n");
    }

    return pass;
}


