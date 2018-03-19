/*
 * Colin Powell
 * CS362: Assignment 4
 * Smithy Random Test
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

int NUM_FAILURES = 0;   // track test failures globally

int check_smithy(test_params *tparams, struct gameState *st);

/*
 * main(): test driver for Smithy
 */

int main(){

    int seed = 1;

    SelectStream(1);
    PutSeed((long)seed);

    // struct containing basic test parameters
    test_params tparams;

    tparams.test_card = smithy;
    tparams.total_cards = (treasure_map -  curse + 1);
    tparams.king_card_start = gold + 1;
    tparams.max_deck_sz = MAX_DECK;
    tparams.max_hand_sz = MAX_HAND;
    tparams.max_supply = 40;

    struct gameState st;

    int n; 

    printf("Testing Smithy...\n"
           "RANDOM TESTS\n"
           "------------\n");

    for(n = 0; n < 10000; n++) {
   
        // fully reset struct 
        memset(&st, '\0', sizeof(struct gameState));

        // create a deck of kingdom cards including smithy
        pick_king_cards(&tparams);
        setup_random_game(&tparams, &st); 
        check_smithy(&tparams, &st);
    }

    printf("\nTESTING COMPLETE. TOTAL ITERATIONS: %d, TOTAL FAILURES: %d\n", n, NUM_FAILURES);

    return 0;
}

/*
 * check_smithy: test oracle for Smithy 
 */

int check_smithy(test_params *tparams, struct gameState *st) {

    struct gameState pre;
    memcpy(&pre, st, sizeof(struct gameState));

    int i, j, k;
    int plyr = st->whoseTurn;
    int pos = 0;
    int pass = TRUE;
    int bonus = 0;  // needed for cardeffect
    int draw_count = 0;
    int drawn[3];
    int draw_left;

    #ifdef VERBOSE
        printf("PRE SMITHY TEST:\n");
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

    // determine expected number of cards to draw
    if( pre.deckCount[plyr] >= 3) {
        draw_count = 3;
        j = 0;
        // get which cards will be drawn
        for(i = pre.deckCount[plyr] - 1; i > (pre.deckCount[plyr] - 1) - draw_count; i--) {
            drawn[j++] = pre.deck[plyr][i];
        }

        // set hand count
        pre.handCount[plyr] += draw_count - 1;
        pre.deckCount[plyr] -= draw_count;

        // place drawn cards into hand
        pre.hand[plyr][pre.handCount[plyr] - 2] = drawn[0];
        pre.hand[plyr][pre.handCount[plyr] - 1] = drawn[1];
        
        // last drawn card placed where played card was located
        pre.hand[plyr][pos] = drawn[2];
        pre.hand[plyr][pre.handCount[plyr]] = -1;

    }
    else {
    
        // if there are sufficient cards left to draw 3, do so
        if( pre.deckCount[plyr] + pre.discardCount[plyr] >= 3) {
            draw_count = 3;
        }
        // otherwise, draw as many cards as are available
        else {
            draw_count = pre.deckCount[plyr] + pre.discardCount[plyr];
        }
        
        draw_left = draw_count;

        // copy post-effect deck into pre-effect deck (num cards = DISCARD COUNT)
        // num_in_deck = pre.discardCount - draw_count;
        
        // draw as many cards as possible:
        j = 0;
        for(i = pre.deckCount[plyr] - 1; i >= 0; i--) {
            drawn[j++] = pre.deck[plyr][i];
            draw_left--;
        }          

        // place drawn cards into hand
        j = 0;
        for(i = pre.handCount[plyr]; i < pre.handCount[plyr] + (draw_count - draw_left); i++ ) {
             pre.hand[plyr][i] = drawn[j++];   
        }

        // increment hand by number drawn
        pre.handCount[plyr] += (draw_count - draw_left);

        /*
         * POST-RESHUFFLING:
         */

        // trust drawCard somewhat: cannot replicate shuffling
        memcpy(pre.deck[plyr], st->deck[plyr], sizeof(int) * pre.discardCount[plyr]);
        // copy -1s from discard 
        memcpy(pre.discard[plyr], st->discard[plyr], sizeof(int) * pre.discardCount[plyr]);

        // update size of deck, discard
        pre.deckCount[plyr] = pre.discardCount[plyr];
        pre.discardCount[plyr] = 0;
        k = j;  // save current num. of cards drawn

        // draw remaining cards from deck    

        // get which cards will be drawn
        for(i = pre.deckCount[plyr] - 1; i > (pre.deckCount[plyr] - 1) - draw_left; i--) {
            drawn[j++] = pre.deck[plyr][i];
        }
        pre.deckCount[plyr] -= draw_left;   // take remaining cards to draw from deck


        // place drawn cards into hand
        for(i = pre.handCount[plyr]; i < pre.handCount[plyr] + draw_left; i++) {
           pre.hand[plyr][i] = drawn[k++]; 
        } 

        pre.handCount[plyr] += draw_left;

        // swap last-drawn card with played card
        pre.hand[plyr][pos] = pre.hand[plyr][ pre.handCount[plyr] - 1];
        pre.hand[plyr][ pre.handCount[plyr] - 1] = -1;
        pre.handCount[plyr] -= 1;

    }


    pre.playedCards[pre.playedCardCount] = smithy;
    pre.playedCardCount += 1;

    // memcmp to determine if the two match
    if( memcmp(&pre, st, sizeof(struct gameState)) != 0) {

        NUM_FAILURES++;

        #ifdef VERBOSE
            // if not, print differences
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

