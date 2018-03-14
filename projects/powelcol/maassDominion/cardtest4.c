/* 
 * Colin Powell
 * CS362: Assignment 3
 * cardtest4: Village unit tests
 */

#include "test_helpers.h"
#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"

#include <string.h>
#include <stdio.h>
#include <assert.h>

int village_test(int plyr, int drawn, struct gameState *state);

int main() {

    int i;          // counter
    int seed = 1;
    int numPlayers = 2;
    int plyr = 0;   // current player

    int cards[10] = { adventurer, council_room, feast, gardens, mine, remodel, smithy,
                  village, baron, great_hall };

    // test_state: test game state
    // orig_state: original game state 
    struct gameState test_state, orig_state;

    // init. game should give each player deck of > 3 cards
    initializeGame(numPlayers, cards, seed, &orig_state);
    orig_state.hand[plyr][0] = village;    // set village in pos. 0

    assert(orig_state.deckCount[plyr] == 5);

    // village contract: 
    // +1 card from deck (delta cards +0)
    // +2 actions (delta-action + 1)
    // all other players: state remains the same 

    printf("VILLAGE:\nALL TESTS: player hand size = %d\n\n", orig_state.handCount[plyr]);

    /* TEST: playing village with deck size = 5 */ 

    printf("*TEST: playing village with deck size == 5*\n");
    memcpy(&test_state, &orig_state, sizeof(struct gameState));

    village_test(0, 1, &test_state);


    /* TEST: playing village with deck size = 0, discard size = 0 */ 

    printf("\n*TEST: playing village with empty deck, empty discard*\n");
    memcpy(&test_state, &orig_state, sizeof(struct gameState));

    test_state.deckCount[plyr] = 0;
    test_state.discardCount[plyr] = 0;

    village_test(0, 0, &test_state);


    /* TEST: playing village with deck size = 0, discard size = 5 */ 

    printf("\n*TEST: playing village with empty deck, discard size == 5*\n");
    memcpy(&test_state, &orig_state, sizeof(struct gameState));

    test_state.deckCount[plyr] = 0;
    test_state.discardCount[plyr] = 5; 

    // fill discard with copper cards
    for(i = 0; i < test_state.discardCount[plyr]; i++) {
        test_state.discard[plyr][i] = copper;
    }

    village_test(0, 1, &test_state);
    return 0;
}

/*
 * village_test: unit testing engine for village card.
 * -receives: plyr, the idx of the current player,
 *            drawn, the number of cards expected to be drawn
 *            state, a pointer to the test game state
 * -returns: a series of messages printed to stdout 
 *           representing the status of the test (pass/fail)
 *           on exit, returns 0 for a failed test, and 
 *           1 on a successful test.
 */

int village_test(int plyr, int drawn, struct gameState *state) {

    int i = 0;  // counter
    int pass = TRUE;   // tracks test status

    int hand_pos = 0;    // pos of card to play
    int bonus = 0;      // not part of card's effect; needed for cardEffect

    struct gameState st_test;   // used to store copy of game state for testing
    memcpy(&st_test, state, sizeof(struct gameState)); 

    // expected actions after playing card: +2 from card effect
    // action deduction for playing card happens outside of cardEffect / villageEffect
    int exp_actions = state->numActions + 2;

    // store expected hand, deck, discard, supply, played counts:
    int exp_hands[state->numPlayers];
    int exp_decks[state->numPlayers];
    int exp_discard[state->numPlayers];

    int exp_play_count = 1; // only expect to play villager

    // copy initial hand and deck count states into temp arrays
    for(i = 0; i < state->numPlayers; i++) {
        exp_hands[i] = state->handCount[i];
        exp_decks[i] = state->deckCount[i];
        exp_discard[i] = state->discardCount[i];
    }

    // one (at most) card drawn; accommodate possible reshuffling for empty deck
    // no changes to discard pile 
    exp_hands[plyr] = state->handCount[plyr] + drawn - 1; 
    exp_decks[plyr] = (state->deckCount[plyr] - drawn >= 0) ? 
                      state->deckCount[plyr] - drawn : 
                      state->deckCount[plyr] + state->discardCount[plyr] - drawn;    

    // if deck > 0 cards, no reshuffling necessary; otherwise, discard pile empty
    exp_discard[plyr] = state->deckCount[plyr] > 0 ? state->discardCount[plyr] : 0; 

    /* TESTING */

    printf("current turn: %d\n", plyr);
    cardEffect(village, 0, 0, 0, &st_test, hand_pos, &bonus);

    /* if function was successful, check state against expected changes */

    // check that all players have expected hand, deck, discard deck sizes
    if(test_fulldeck_counts(exp_decks, exp_hands, exp_discard, &st_test) == FALSE) {
        pass = FALSE;
    }

    // check that played card count has not changed for all cards 
    if(test_playedcard_count(exp_play_count, &st_test) == FALSE) {
        pass = FALSE;
    }

    // check that supply count has changed only for gained card 
    if(test_supply_counts(state->supplyCount, &st_test) == FALSE) {
        pass = FALSE;
    }

    // check that 2 actions added
    printf("verifying that two actions have been added:\n");
    if( exp_actions != st_test.numActions ) {
        pass = FALSE;
    }
    printf("--number of total actions: %d, expected: %d\n", exp_actions, st_test.numActions);

    // output results
    output_test_result(pass);
    return pass;
}
