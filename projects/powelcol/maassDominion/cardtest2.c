/*
 * Colin Powell
 * CS362: Assignment 3
 * cardtest2: Adventurer unit tests
 */

#include "test_helpers.h"

#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>

int adv_test(int plyr, int drawn, int disc, struct gameState *state);

int main() {

    int i;
    int seed = 1;
    int numPlayers = 2;
    int plyr = 0;   // current player
    int draw; 
    int disc;
    int top_deck;

    int cards[10] = { adventurer, council_room, feast, gardens, mine, remodel, smithy,
                  village, baron, great_hall };

    // test_state: test_state-test game state
    // orig_state: orig_state-test game state 
    struct gameState test_state, orig_state;

    // init. game should give each player deck of > 3 cards
    // place adventurer in player's hand at pos. 0
    initializeGame(numPlayers, cards, seed, &orig_state);
    orig_state.deck[plyr][0] = adventurer;

    // assert that state matches expectations
    assert(orig_state.deckCount[plyr] == 5);
    for(i = 0; i < orig_state.numPlayers; i++) {
        assert(orig_state.deckCount[i] >= 5);
    }

    printf("ADVENTURER:\nALL TESTS: player hand size = %d, deck size = %d\n\n", orig_state.handCount[plyr], orig_state.deckCount[plyr]);

    /* TEST: playing adventurer with > 2 treasure cards in deck, same type, top of deck */
    printf("*TEST: play adventurer w/ 2 treas. cards at top of deck*\n");
    memcpy(&test_state, &orig_state, sizeof(struct gameState));

    draw = 2;   // expect to draw two cards
    disc = 0;   // none discarded 

    // place 1 copper and 1 gold at top of deck
    top_deck = test_state.deckCount[plyr] - 1;
    test_state.deck[plyr][top_deck] = copper;
    test_state.deck[plyr][top_deck - 1] = gold;

    adv_test(plyr, draw, disc, &test_state);

    /* TEST: playing adventurer w/ 2 treasure cards at very bottom of deck */
    printf("\n*TEST: play adventurer w/ 2 treas. cards at bottom of deck*\n");
    memcpy(&test_state, &orig_state, sizeof(struct gameState));

    draw = 5;   // player has to draw entire deck
    disc = 3;   // will have to discard three of these

    // place treasure cards at bottom of deck - 
    // use silver to ensure that it is recognized by adventurer effect
    test_state.deck[plyr][0] = copper;
    test_state.deck[plyr][1] = silver;
   
    // fill rest of deck with non-treasure cards 
    for(i = 2; i < test_state.deckCount[plyr]; i++) {
        test_state.deck[plyr][i] = estate; 
    }

    adv_test(plyr, draw, disc, &test_state);

    /* TEST: playing adventurer w/ 1 treasure card in deck, empty discard */

    printf("\n*TEST: play adv. w/ 1 treas. card in deck, none in discard*\n");
    memcpy(&test_state, &orig_state, sizeof(struct gameState));

    draw = 5;   // player has to draw entire deck
    disc = 4;   // will have to discard four of these

    // place treasure cards at bottom of deck - 
    // use silver to ensure that it is recognized by adventurer effect
    test_state.deck[plyr][0] = silver;
   
    // fill rest of deck with non-treasure cards 
    for(i = 1; i < test_state.deckCount[plyr]; i++) {
        test_state.deck[plyr][i] = estate; 
    }

    adv_test(plyr, draw, disc, &test_state);

   
    /* TEST: playing adventurer w/ 0 treasure cards in deck, empty discard */   

    printf("\n*TEST: play adv. w/ 0 treas. card in deck, none in discard*\n");
    memcpy(&test_state, &orig_state, sizeof(struct gameState));

    draw = 5;   // player has to draw entire deck
    disc = 5;   // will have to discard all of these

    // fill deck with non-treasure cards 
    for(i = 0; i < test_state.deckCount[plyr]; i++) {
        test_state.deck[plyr][i] = estate; 
    }

    adv_test(plyr, draw, disc, &test_state);
    return 0;
}

/* 
 * adv_test: unit testing enting for the adventurer card
 * -receives: plyr, the idx of the current player
 * --drawn: the number of cards expected to be drawn
 * --disc: the number of cards expected to be discarded
 * --state: a pointer to the current game state
 * -returns: a series of messages printed to stdout
 *           representing the status of the test (pass/fail)
 *           on exit, returns 0 for fail, 1, for pass.
 */

int adv_test(int plyr, int drawn, int disc, struct gameState *state) {

    int i = 0;  // counter
    int pass = TRUE;   // tracks test status

    int good_cards = TRUE;   // tracks if non-treasure cards are drawn
    
    int hand_pos = 0;    // pos of card to play
    int bonus = 0;      // not part of adv.; needed for cardEffect
    int top_hand;       // idx of top card in player's hand


    // store expected hand, deck, discard counts:
    int exp_hands[state->numPlayers];
    int exp_decks[state->numPlayers];
    int exp_discard[state->numPlayers];

 //   int exp_played[MAX_DECK];
    int exp_played_count = 1;   // num. of cards expected to be played

    // copy initial hand and deck count states into temp arrays
    for(i = 0; i < state->numPlayers; i++) {
        exp_hands[i] = state->handCount[i];
        exp_decks[i] = state->deckCount[i];
    }

    // set current player's expected hand / deck counts
    // expected hand: drawn - discarded - played
    exp_hands[plyr] = state->handCount[plyr] + drawn - disc - 1;
    exp_decks[plyr] = (state->deckCount[plyr] - drawn >= 0) ? 
                      state->deckCount[plyr] - drawn : 
                      state->deckCount[plyr] + state->discardCount[plyr] - drawn;    
    exp_discard[plyr] = disc;
/*
    for(i = 0; i < MAX_DECK; i++) {
        exp_played[i] = state->playedCards[i];
    } 
    
    // expect adventurer to be next card played
    exp_played[state->playedCardCount] = adventurer; 
*/
    struct gameState st_test;   // used to store copy of game state for testing
    memcpy(&st_test, state, sizeof(struct gameState)); 

    /* TESTING */

    cardEffect(adventurer, 0, 0, 0, &st_test, hand_pos, &bonus); 
    
    printf("current turn: %d\n", plyr);

    // check that all players have expected hand / deck counts
    if(test_fulldeck_counts(exp_decks, exp_hands, exp_discard, &st_test) == FALSE) {
        pass = FALSE;
    }

     // check that played cards match expectations 
    if(test_playedcard_count(exp_played_count, &st_test) == FALSE) {
        pass = FALSE;
    }

    // check that supply count has not changed for any card in supply 
    if(test_supply_counts(state->supplyCount, &st_test) == FALSE) {
        pass = FALSE;
    }

    // adventurer-specific test: ensure that only treasures were 
    // drawn from the draw pile:
    top_hand = st_test.handCount[plyr] - 1;
    printf("checking that only treasures were drawn (if any):\n");
   
    // if any cards were drawn (delta > 0): 
    if(drawn - disc > 0) {
        // if drawn cards are not treasures, output the mismatch
        for(i = top_hand; i > top_hand - (drawn - disc); i--) {
            if(st_test.hand[plyr][i] > gold || st_test.hand[plyr][i] < copper) {
                printf("-ERROR: hand pos %d, found non-treasure card with ID: %d\n", 
                        i, st_test.hand[plyr][i]);
                good_cards = FALSE;
            }            
        } 
    }

    if(!good_cards) {
        pass = FALSE;
    }
    else {
        printf("--no non-treasures were drawn - OK\n");
    }

    output_test_result(pass);

    return pass;
}
