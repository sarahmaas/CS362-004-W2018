/*
 * Colin Powell
 * CS362: Assignment 3
 * cardtest1: Smithy unit tests
 */

#include "test_helpers.h"
#include "dominion.h"
#include "dominion_helpers.h"

#include "rngs.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>


int smithyTest(int plyr, int draw, struct gameState *state);

int main() {

    int i;
    int seed = 1;
    int numPlayers = 2;
    int plyr = 0;   // current player
    int draw;       // number of drawn cards expected

    int k[10] = { adventurer, council_room, feast, gardens, mine, remodel, smithy,
                  village, baron, great_hall };

    // test_state: test_state-test game state
    // orig_state: post-test game state 
    struct gameState test_state, orig_state; 

    // init. game should give each player deck of > 3 cards
    initializeGame(numPlayers, k, seed, &orig_state);

    // place smithy into first position in hand
    orig_state.hand[plyr][0] = smithy;
    
    // initializeGame should produce sufficient deck sizes
    for(i = 0; i < orig_state.numPlayers; i++){
        assert(orig_state.deckCount[i] > 3);
    }

    printf("SMITHY:\nALL TESTS: player hand size = %d\n", orig_state.handCount[plyr]);

    /* ---- TEST: playing smithy with deck size > 3 cards ---- */
    printf("\n*TEST: playing smithy with deck = 5 cards*\n");
    // make a copy of our test state
    memcpy(&test_state, &orig_state, sizeof(struct gameState));
    draw = 3;

    smithyTest(plyr, draw, &test_state);

    /* ---- TEST: playing smithy with deck size == 2 cards AND no discard pile ---- */
    printf("\n*TEST: playing smithy with deck == 2 cards, empty discard pile*\n");
    memcpy(&test_state, &orig_state, sizeof(struct gameState));
    draw = 2;

    // set current player's deck to contain only two cards 
    test_state.deckCount[plyr] = 2;
    smithyTest(plyr, draw, &test_state);

    /* ---- TEST: playing smithy with deck size == 1 card AND no discard pile ---- */
    printf("\n*TEST: playing smithy with deck == 1 card, empty discard pile*\n");
    memcpy(&test_state, &orig_state, sizeof(struct gameState));
    draw = 1;

    // set current player's deck to contain only one card
    test_state.deckCount[plyr] = 1;
    smithyTest(plyr, draw, &test_state);

    /* ---- TEST: playing smithy with deck size == 1 card AND discard pile with 5 cards ---- */
    printf("\n*TEST: playing smithy with deck == 1 card, 5 cards in discard pile*\n");
    memcpy(&test_state, &orig_state, sizeof(struct gameState));
    draw = 3;
   
    test_state.deckCount[plyr] = 1;    // expect to draw from discard when deck emptied
    test_state.discardCount[plyr] = 5; // set discard pile to have five cards
    // fill discard pile with copper coins
    for(i = 0; i < test_state.discardCount[plyr]; i++) {
        test_state.discard[plyr][i] = copper;
    } 
    smithyTest(plyr, draw, &test_state);
     
    return 0; 
}

/*
 * smithyTest: smithy unit test engine. 
 * arguments: plyr, the current player. drawn, the number of cards 
 * expected to be drawn. state, a pointer to the pre-test game state.
 * checks:
 *  - current player's hand and draw count are appropriate for Smithy.
 *  - other player's hands and draw counts do not change
 *  - supply (kingdon / victory / treasure cards ) is unchanged
 * returns: TRUE (1) if tests pass, and 0 (FALSE) if tests fail.
 */

int smithyTest(int plyr, int drawn, struct gameState *state) {

    int i = 0;  // counter
    int pass = TRUE;   // tracks test status

    int hand_pos = 0;    // pos of card to play
    int bonus = 0;      // not part of card function; needed for cardEffect
    int disc = 1;       // expect to discard only played card

    struct gameState st_test;   // used to store copy of game state for testing
    memcpy(&st_test, state, sizeof(struct gameState)); 

    // store expected hand, deck, discard, supply, and played counts
    int exp_hands[state->numPlayers];
    int exp_decks[state->numPlayers];
    int exp_discard[state->numPlayers];

    int exp_play_count = 1; // just playing smithy

    // copy initial hand and deck count states into temp arrays
    for(i = 0; i < state->numPlayers; i++) {
        exp_hands[i] = state->handCount[i];
        exp_decks[i] = state->deckCount[i];
        exp_discard[i] = state->discardCount[i];
    }

    // one (at most) card drawn, one discarded
    // accommodate 'reshuffling' necessary if deck pile emptied
    exp_hands[plyr] = state->handCount[plyr] + drawn - disc; 
    exp_decks[plyr] = (state->deckCount[plyr] - drawn >= 0) ? 
                      state->deckCount[plyr] - drawn : 
                      state->deckCount[plyr] + state->discardCount[plyr] - drawn;    

    // if deck > 3 cards, no reshuffling necessary; otherwise, discard pile empty
    exp_discard[plyr] = state->deckCount[plyr] >= 3 ? state->discardCount[plyr] : 0; 

    /* TESTING */

    printf("current turn: %d\n", plyr);
    cardEffect(smithy, 0, 0, 0, &st_test, hand_pos, &bonus);

    /* if function was successful, check state against expected changes */

    // check that all players have expected hand, deck, discard deck sizes
    if(test_fulldeck_counts(exp_decks, exp_hands, exp_discard, &st_test) == FALSE) {
        pass = FALSE;
    }
    
    // check that played card count has not changed for all cards 
    if(test_playedcard_count(exp_play_count, &st_test) == FALSE) {
        pass = FALSE;
    }

    // check that supply count has not changed for all cards 
    if(test_supply_counts(state->supplyCount, &st_test) == FALSE) {
        pass = FALSE;
    }

    // output results
    output_test_result(pass);
    return pass;

}

