/*
 * Colin Powell
 * CS362: Random Test Helpers header file
 */

#ifndef random_test_hdr
#define random_test_hdr

#include "dominion.h"

#define NUM_KING_CARDS 10
#define TRUE 1
#define FALSE 0

typedef struct test_params {

    int test_card; // card under test
    int k[10];  // set of kingdom cards
    
    int total_cards;        // total number of cards in game

    int king_card_start;    // starting idx of kingdom cards

    int max_deck_sz;
    int max_hand_sz;

    int max_supply;
    
} test_params;

/*
 * helper functions for random testing
 */

void print_player_cards(int plyr, struct gameState *st);
void print_diffs(test_params *tparams, struct gameState *expect, struct gameState *observe);
void pick_king_cards(test_params *tparams);
int setup_random_game(test_params *tparams, struct gameState *st);
int card_in_game(int card, test_params *tparams);

#endif
