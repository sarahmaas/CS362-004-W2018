/*
 * Colin Powell
 * CS362: Assignment 4
 * Adventurer Random Test
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include "random_test_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TRUE 1
#define FALSE 0

int NUM_FAILURES = 0;   // track test failures globally

int count_total_treasures(struct gameState *st);
int is_treasure( int card );
int check_adventurer(test_params *tparams, struct gameState *st);

int main() {

    // seed random number generator
    int seed = 1;
    SelectStream(1);
    PutSeed((long)seed);

    test_params tparams;

    tparams.test_card = adventurer;
    tparams.total_cards = (treasure_map - curse + 1);
    tparams.king_card_start = gold + 1;
    tparams.max_deck_sz = MAX_DECK;
    tparams.max_hand_sz = MAX_HAND;
    tparams.max_supply = 40;

    struct gameState st;

    int num_treasures;
    int n;

    printf("Testing adventurer...\n"
           "RANDOM TESTS\n"
           "------------\n");

    for(n = 0; n < 100; n++) {
        memset(&st, '\0', sizeof(struct gameState));

        pick_king_cards(&tparams);
        setup_random_game(&tparams, &st);

        /*
         * IF: there are less than two treasures in the player's possession
         * (including hand) - the program will abort and terminate prematurely
         * this is because it doesn't check for drawCard failure - it will continue
         * drawing from hand even after deck + discard are empty.
         * the following code is a workaround to ensure that all tests complete - 
         * ensures that (buggy) code will at least finish. 
         */
       
        num_treasures = count_total_treasures(&st);      

        // make sure that at least two treasures, total: place remainder in hand
        while(num_treasures < 2) {
            st.hand[st.whoseTurn][ st.handCount[st.whoseTurn] ] = copper;
            st.handCount[st.whoseTurn]++;
            num_treasures++;
        }

        check_adventurer(&tparams, &st); 
    }
    
    printf("\nTESTING COMPLETE. TOTAL ITERATIONS: %d, TOTAL FAILURES: %d\n", n, NUM_FAILURES);

    return 0;
}

int check_adventurer(test_params *tparams, struct gameState *st) {

    struct gameState pre;
    memcpy(&pre, st, sizeof(struct gameState));

    int i;
    int plyr = st->whoseTurn;

    int pos = 0;

    int bonus = 0;  // needed for cardeffect
    int draw_count = 0;

    int temphand[MAX_HAND];
    int z = 0;

    #ifdef VERBOSE
        printf("PRE ADVENTURER TEST:\n");
        print_player_cards(plyr, &pre);
    #endif

    // get hand position of card under test
    while(pos < st->handCount[plyr]) {
        if(st->hand[plyr][pos] == tparams->test_card) {
            break;
        }
        pos++;
    }

    cardEffect(tparams->test_card, 0, 0, 0, st, pos, &bonus);

    // get expected draw count, deck positions of cards drawn
    i = pre.deckCount[plyr] - 1;
    while( i >= 0 && draw_count < 2) {

        if(is_treasure(pre.deck[plyr][i])) {

            pre.hand[plyr][pre.handCount[plyr]] = pre.deck[plyr][i];
            // place card into hand
            pre.handCount[plyr]++;
            draw_count++;
        }
        else {
            temphand[z] = pre.deck[plyr][i];
            z++; 
        }
        pre.deckCount[plyr]--;
        i--;

    }

    // if we couldn't find both needed treasure cards in deck, check discard
    if( draw_count < 2 ) {
        
        // move discard into deck
        memcpy(pre.deck[plyr], st->deck[plyr], sizeof(int) * pre.discardCount[plyr]);
        memcpy(pre.discard[plyr], st->discard[plyr], sizeof(int) * pre.discardCount[plyr]);

        // set deck count to previous discard count size
        pre.deckCount[plyr] = pre.discardCount[plyr];
        pre.discardCount[plyr] = 0;

        // repeat previous draw card exercise with new deck
        i = pre.deckCount[plyr] - 1;
        while( i >= 0 && draw_count < 2) {
            if(is_treasure(pre.deck[plyr][i])) {

                pre.hand[plyr][pre.handCount[plyr]] = pre.deck[plyr][i];
                // place card into hand
                pre.handCount[plyr]++;
                draw_count++;
            }
            else {
                temphand[z] = pre.deck[plyr][i];
                z++; 
            }
            pre.deckCount[plyr]--;
            i--;
        }

    }

    // remove played adventurer from hand
    pre.hand[plyr][pos] = pre.hand[plyr][pre.handCount[plyr] - 1];
    pre.hand[plyr][pre.handCount[plyr] - 1] = -1;
    pre.handCount[plyr]--;

    // place drawn + discarded cards in hand
    while(z > 0) {
        pre.discard[plyr][pre.discardCount[plyr]++] = temphand[z - 1];
        z--;
    }

    pre.playedCards[pre.playedCardCount] = adventurer;
    pre.playedCardCount += 1;

    if( memcmp(&pre, st, sizeof(struct gameState)) != 0) {
        // if not, print differences

        NUM_FAILURES++;

        #ifdef VERBOSE
            printf("EXPECTED ADVENTURER TEST:\n");
            print_player_cards(plyr, &pre);

            printf("OBSERVED ADVENTURER TEST:\n");
            print_player_cards(plyr, st);
        #endif

        print_diffs(tparams, &pre, st);
        printf("\n\n");
    }
    return 0;
}

/*
 * is_treasure(): returns TRUE if argument represents a treasure card, false if not
 */

int is_treasure( int card ) {
    return (card >= copper && card <= gold);
}


/*
 * count_total_treasures(): counts the total number of treasures in the deck, discard, and hand
 */

int count_total_treasures(struct gameState *st) {

    int count = 0;
    int i = 0;
    int p = st->whoseTurn;

    // account for treasures in hand
    for(i = 0; i < st->handCount[p]; i++) {
        if(is_treasure(st->hand[p][i])) {
            count++;
        }
    }

    // account for treasures in deck
    for(i = 0; i < st->deckCount[p]; i++) {
        if(is_treasure(st->deck[p][i])) {
            count++;
        }
    }

    // account for treasures in discard
    for(i = 0; i < st->discardCount[p]; i++) {
        if(is_treasure(st->discard[p][i])) {
            count++;
        }
    }
    return count;
}
