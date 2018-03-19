/* 
 * Colin Powell
 * CS362: Assignment 3
 * cardtest3: Mine unit tests
 */

#include "test_helpers.h"
#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"

#include <string.h>
#include <stdio.h>
#include <assert.h>

int mine_test(int plyr, int drawn, int choice, struct gameState *state);

int main() {

    int i;
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
    orig_state.hand[plyr][0] = mine;    // set mine in pos. 0

    // put copper in second slot (after mine)
    orig_state.hand[plyr][1] = copper;

    // put estates in non-mine, non-copper slots
    for(i = 2; i < orig_state.handCount[plyr]; i++) {
        orig_state.hand[plyr][i] = estate;
    }
    
    assert(orig_state.deckCount[plyr] == 5);

    // mine contract:
    // trash a treasure, get treasure costing UP TO 3 more
        // copper: copper or silver
        // silver: silver or gold
    // if no treasure in hand, card discarded, no other changes
    // no change to other player game states
    // if wrong gain card choice: no change (fail)
    // if wrong trash card choice: no change (fail)

    printf("MINE:\nALL TESTS: player hand size = %d\n\n", orig_state.handCount[plyr]);

    /* TEST: playing mine with invalid trash card */

    printf("\n*TEST: play mine w/ invalid trash card - not a coin*\n");
    memcpy(&test_state, &orig_state, sizeof(struct gameState));
    
    mine_test(0, 2, silver, &test_state);


    printf("\n*TEST: play mine w/ invalid trash card - not in hand*\n");
    memcpy(&test_state, &orig_state, sizeof(struct gameState));

    // place a valid card in the idx one after the highest in hand
    test_state.hand[plyr][ test_state.handCount[plyr] ] = copper;
    mine_test(0, test_state.handCount[plyr], silver, &test_state );


    printf("\n*TEST: play mine w/ invalid trash card (%d)*\n", MAX_HAND - 1);
    memcpy(&test_state, &orig_state, sizeof(struct gameState));

    // place a dummy value in the final index of the player's hand[] array
    // to skip the first error check
    test_state.hand[plyr][MAX_HAND -1] = copper;
    // attempting to trash MAX_HAND - 1 (not a valid card id)
    mine_test(0, MAX_HAND - 1, copper, &test_state);


    printf("\n*TEST: play mine w/ invalid trash card (%d)*\n", -1);
    memcpy(&test_state, &orig_state, sizeof(struct gameState));

    // attempting to trash -1 (not a valid card id)
    mine_test(0, -1, copper, &test_state);


    printf("\n*TEST: play mine w/ non-coin gain card (%d)*\n", adventurer);
    memcpy(&test_state, &orig_state, sizeof(struct gameState));
    
    // attempt to gain adventurer into hand
    mine_test(0, 1, adventurer, &test_state);
   
    printf("\n*TEST: play mine w/ too-expensive gain card (%d)*\n", gold);
    memcpy(&test_state, &orig_state, sizeof(struct gameState));

    mine_test(0, 1, gold, &test_state);


    printf("\n*TEST: play mine w/ correct cards: exchange copper for silver*\n");
    memcpy(&test_state, &orig_state, sizeof(struct gameState));

    mine_test(0, 1, silver, &test_state);
    
    printf("\n*TEST: play mine w/ correct cards: exchange gold for copper*\n");
    memcpy(&test_state, &orig_state, sizeof(struct gameState));

    // put gold in second slot (after mine)
    test_state.hand[plyr][1] = gold;
    mine_test(0, 1, copper, &test_state);

    return 0;
}

/*
 * mine_test: unit testing engine for mine card.
 * -receives: plyr, the idx of the current player
 *            card_to_trash, the idx of the card to trash in the player's hand
 *            card_to_gain, the enum / ID value of the card to gain
 *            state, a pointer to the current game state
 * -returns:  a series of messages printed to stdout representing the 
 *            status of the test (pass/fail). on exit, returns 0 for a
 *            failed test, and 1 on successful test. 
 */

int mine_test(int plyr, int card_to_trash, int card_to_gain, struct gameState *state) {

    int i = 0;  // counter
    int pass = TRUE;   // tracks test status
    
    int hand_pos = 0;    // pos of card to play
    int bonus = 0;      // not part of effect, but needed for cardEffect

    struct gameState st_test;   // used to store copy of game state for testing
    memcpy(&st_test, state, sizeof(struct gameState)); 

    int exp_ret = 0;    // expected cardEffect ret val
    int ret;            // actual cardEffect ret val

    // store expected hand, deck, discard, played, and supply counts:
    int exp_hands[state->numPlayers];
    int exp_decks[state->numPlayers];
    int exp_discard[state->numPlayers];

    int exp_play_count = 1; // only mine should be played

    // supp_size: size of supply count
    int supp_size = sizeof(state->supplyCount) / sizeof(state->supplyCount[0]);
    int exp_supply[supp_size];

    // copy initial hand and deck count states into temp arrays
    for(i = 0; i < state->numPlayers; i++) {
        exp_hands[i] = state->handCount[i];
        exp_decks[i] = state->deckCount[i];
        exp_discard[i] = state->discardCount[i];
    }
    
    // player will have one less card in their hand after successful play
    // discarded mine + gained card - trashed card
    for(i = 0; i < supp_size; i++) {
        exp_supply[i] = state->supplyCount[i];
    }
    exp_hands[plyr] = state->handCount[plyr]; 
   
    /* ERROR CHECKING */

    // set expected return value: any of these conditions
    // should make the card function a no-op (no state change)
    if(state->hand[plyr][card_to_trash] < copper || 
       state->hand[plyr][card_to_trash] > gold   || 
       card_to_gain < copper  || card_to_gain > gold ) {
       
        exp_ret = -1;   
    }
    // if card to trash is not in hand (out of range), should ret. -1 
    else if( card_to_trash >= (state->handCount[plyr]) || card_to_trash < 0 ) {
        exp_ret = -1;
    }
    // else: card effect should also return -1 if choice is too expensive 
    // if gain / trash cards are both coins, this will only happen if 
    // gain = gold and trash = copper
    else if(state->hand[plyr][card_to_trash] == copper && card_to_gain == gold ){
        exp_ret = -1;
    }
    else {
        exp_hands[plyr]--;
        exp_supply[card_to_gain]--;
    }
    
    /* TESTING */
    printf("current turn: %d\n", plyr);
    ret = cardEffect(mine, card_to_trash, card_to_gain, 0, &st_test, hand_pos, &bonus);

    if( exp_ret != ret ) {
        pass = FALSE;
    }

    printf("card effect return value: %d, expected: %d\n", ret, exp_ret);

    // if error, assert that no changes to game state were made - return if true 
    if( ret < 0 ) {
        printf("checking that card effect error leaves state unchanged:\n");
        if(memcmp(state, &st_test, sizeof(struct gameState) ) == 0) {
            printf("--no changes: OK. terminating test...\n");
            output_test_result(pass);
            return TRUE;
        }
        else {
            printf("--changes to state: see below for test results:\n");
        }
    }

    /* if function was successful, check state against expected changes */

    // check that all players have expected hand / deck counts
    if(test_fulldeck_counts(exp_decks, exp_hands, exp_discard, &st_test) == FALSE) {
        pass = FALSE;
    }

    // check that played card count has not changed for all cards 
    if(test_playedcard_count(exp_play_count, &st_test) == FALSE) {
        pass = FALSE;
    }

    // check that supply count has changed only for gained card 
    if(test_supply_counts(exp_supply, &st_test) == FALSE) {
        pass = FALSE;
    }

    output_test_result(pass);
    return pass;
}
