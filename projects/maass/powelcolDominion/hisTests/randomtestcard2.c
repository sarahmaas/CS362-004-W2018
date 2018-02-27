/*
 * Colin Powell
 * CS362: Assignment 4
 * Adventurer Random Test
 */

#include "random_test_helpers.h"
#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

int NUM_FAILURES = 0;

int check_village(test_params *tparams, struct gameState *st);

/*
 * main(): test driver for Smithy
 */

int main(){

    int seed = 1;

    SelectStream(1);
    PutSeed((long)seed);

    // struct containing basic test parameters
    test_params tparams;

    tparams.test_card = village;
    tparams.total_cards = (treasure_map -  curse + 1);
    tparams.king_card_start = gold + 1;
    tparams.max_deck_sz = 500;
    tparams.max_hand_sz = 500;
    tparams.max_supply = 40;

    struct gameState st;

    int n; 

    printf("Testing Village...\n"
           "RANDOM TESTS\n"
           "------------\n");

    for(n = 0; n < 100000; n++) {
   
        // fully reset struct 
        memset(&st, '\0', sizeof(struct gameState));

        // create a deck of kingdom cards including village
        pick_king_cards(&tparams);
        setup_random_game(&tparams, &st); 
        check_village(&tparams, &st);
    }

    printf("\nTESTING COMPLETE. TOTAL ITERATIONS: %d, TOTAL FAILURES: %d\n", n, NUM_FAILURES);

    return 0;
}

/*
 * check_village: test oracle for Village 
 */

int check_village(test_params *tparams, struct gameState *st) {

    struct gameState pre;
    memcpy(&pre, st, sizeof(struct gameState));

    int plyr = st->whoseTurn;
    int pos = 0;
    int pass = TRUE;
    int bonus = 0;  // needed for cardeffect

    #ifdef VERBOSE
        printf("PRE VILLAGE TEST:\n");
        print_player_cards(plyr, &pre);
    #endif

    // get hand position of card under test
    while(pos < st->handCount[plyr]) {
        if(st->hand[plyr][pos] == tparams->test_card) {
            break;
        }
        pos++;
    }
    
    // call cardEffect on st
    cardEffect(tparams->test_card, 0, 0, 0, st, pos, &bonus);

    /*
     * set expected post-call state:
     */

    // hand count should not change (draw a card, play a card)
    // discard count should not change
    // deck size should decrease by one if deck size > 0
    // otherwise, discard == deck, draw top from deck

    if( pre.deckCount[plyr] > 0) {
        // draw a card
        pre.hand[plyr][pos] = pre.deck[plyr][pre.deckCount[plyr] - 1];
    
        // this card will have been swapped from last pos. in original call;
        // set former pos. at -1 to match discardCard
        pre.hand[plyr][pre.handCount[plyr]] = -1;
        
        // modify deck count appropriately
        pre.deckCount[plyr]--;

    }
    else if(pre.discardCount[plyr] > 0) {
        memcpy(pre.deck[plyr], st->deck[plyr], sizeof(int) * pre.discardCount[plyr]);
        memcpy(pre.discard[plyr], st->discard[plyr], sizeof(int) * pre.discardCount[plyr]);

//        pre.hand[plyr][st->handCount[plyr] - 1] = st->hand[plyr][st->handCount[plyr] - 1];

        pre.deckCount[plyr] = pre.discardCount[plyr];
        pre.discardCount[plyr] = 0;

        // replace played card with drawn card
        pre.hand[plyr][pos] = pre.deck[plyr][pre.deckCount[plyr] -1];
        pre.hand[plyr][pre.handCount[plyr]] = -1;
        pre.deckCount[plyr]--;
    }
    else {
        pre.hand[plyr][pos] = pre.hand[plyr][ pre.handCount[plyr] - 1 ];
        pre.hand[plyr][ pre.handCount[plyr] - 1 ] = -1;
        pre.handCount[plyr]--;
    }

    // add two actions (action is subtracted after card effect is assessed)
    pre.numActions += 2;
/*
    int i;
    for(i = 0; i < MAX_DECK; i++) {
        if(pre.deck[plyr][i] != st->deck[plyr][i]) {
            printf("deck mismatch pos: %d, pre: (%d) post: (%d)\n", i, pre.deck[plyr][i], st->deck[plyr][i]);
        }
    }

    for(i = 0; i < MAX_DECK; i++) {
        if(pre.discard[plyr][i] != st->discard[plyr][i]) {
            printf("discard mismatch pos: %d, pre: (%d) post: (%d)\n", i, pre.discard[plyr][i], st->discard[plyr][i]);
        }
    }

    for(i = 0; i < MAX_HAND; i++) {
        if(pre.hand[plyr][i] != st->hand[plyr][i]) {
            printf("hand mismatch pos: %d, pre: (%d) post: (%d)\n", i, pre.hand[plyr][i], st->hand[plyr][i]);
        }
    }
*/
    // add village to played cards, set played card count
    pre.playedCards[pre.playedCardCount] = village;
    pre.playedCardCount += 1;

    // memcmp to determine if the two match
    if( memcmp(&pre, st, sizeof(struct gameState)) != 0) {

        NUM_FAILURES++;
    
        // if not, print differences
        #ifdef VERBOSE
            printf("EXPECTED SMITHY TEST:\n");
            print_player_cards(plyr, &pre);

            printf("OBSERVED SMITHY TEST:\n");
            print_player_cards(plyr, st);
        #endif 

        print_diffs(tparams, &pre, st);
        printf("\n\n");

    }

    return pass;

}

