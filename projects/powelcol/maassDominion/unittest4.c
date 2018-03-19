/*
 * Colin Powell
 * CS362: Assignment 3
 * unittest4.c : fullDeckCount tests
 */



#include "test_helpers.h"
#include "dominion.h"
#include "dominion_helpers.h"
#include <stdio.h>
#include <string.h>


#define TRUE 1
#define FALSE 0
#define NUM_TESTS 5

int main() {

    int card, test;   // counters
    int i;
    int num_players = 2;

    int pass = TRUE;
    int count, exp_count;

    struct gameState orig_state, test_state;

    // setup state with necessary parameteres: zero deck, discard, played, hand
    // no need to initialize game

    int plyr = orig_state.whoseTurn = 0;
    for(i = 0; i < num_players; i++) {
        orig_state.handCount[i] = 0;
        orig_state.deckCount[i] = 0;
        orig_state.discardCount[i] = 0;
    }
    // clear played cards, set played card count == 0
    for(i = 0; i < MAX_DECK; i++) {
        orig_state.playedCards[i] = 0;
    }
    orig_state.playedCardCount = 0;

    int def_pile_size = 5;  // default pile size

    printf("FUNCTION: fullDeckCount\n\n");

    /* first tests: make sure that deck, hand, discard counted individually */

    printf("*TEST: counting cards in deck, hand, discard, and played card piles*\n");
    printf("--------------------------------------------------\n");

    for(test = 0; test < NUM_TESTS; test++) {

        // copy original state 
        memcpy(&test_state, &orig_state, sizeof(struct gameState));
        plyr = 0;
   
        if(test == 0) {
            
            // set expected card count, card type
            exp_count = 15;
            card = curse;
   
            printf("SUB_TEST: cards from deck, discard, hand piles " 
                   "counted accurately...\n"
                   "filling each with 5 curses, counting them\n");

            test_state.discardCount[plyr] = def_pile_size;
            test_state.deckCount[plyr] = def_pile_size;
            test_state.handCount[plyr] = def_pile_size;

            for(i = 0; i < def_pile_size; i++) {
                test_state.discard[plyr][i] = card;
                test_state.deck[plyr][i] = card; 
                test_state.hand[plyr][i] = card; 
            } 
        }  
        if(test == 1) {
            exp_count = 0;
            card = curse;

            // test with empty piles 
            printf("SUB_TEST: correctly returns 0 for a card not in any of "
                   "deck, hand, or discard\n"
                   "filling each pile with 5 estates, searching for curse (%d)\n", curse);

            test_state.discardCount[plyr] = def_pile_size;
            test_state.deckCount[plyr] = def_pile_size;
            test_state.handCount[plyr] = def_pile_size;

            for(i = 0; i < def_pile_size; i++) {
                test_state.discard[plyr][i] = estate;
                test_state.deck[plyr][i] = estate;
                test_state.hand[plyr][i] = estate; 
            } 
        }
        if(test == 2) {
            exp_count = 12;
            card = curse;

            // test with varied pile sizes
            printf("SUB_TEST: correctly returns count for varied deck, discard, hand sizes\n"
                   "filling deck, discard, hand:\n"
                   "--3 curses, 4 curses, 5 curses, respectively\n"
                   "searching for curse (%d)\n", curse);

            // set test deck counts, populate decks with cards
            test_state.deckCount[plyr] = 3;
            for(i = 0; i < 3; i++) {
                test_state.deck[plyr][i] = curse;
            }

            test_state.discardCount[plyr] = 4;
            for(i = 0; i < 4; i++) {
                test_state.discard[plyr][i] = curse;
            }

            test_state.handCount[plyr] = 5;
            for(i = 0; i < 5; i++) {
                test_state.deck[plyr][i] = curse;
            } 
        }

        if(test == 3) {

            exp_count = 8; 
            card = treasure_map;

            printf("SUB_TEST: correctly identifies played cards when counting "
                   "full deck of player in current turn\n"
                   "one treasure map (%d) in each of deck, "
                   "discard, hand; 5 in played cards\n", treasure_map);

            // place a single treasure_map into current player's deck, discard, hand
            test_state.handCount[plyr] = 1;
            test_state.deckCount[plyr] = 1;
            test_state.discardCount[plyr] = 1;
        
            test_state.hand[plyr][0] = card;
            test_state.deck[plyr][0] = card;
            test_state.discard[plyr][0] = card;

            test_state.playedCardCount = def_pile_size;
            for(i = 0; i < test_state.playedCardCount; i++) {
                test_state.playedCards[i] = card;
            }

            printf("played cards: ");
            for(i = 0; i < test_state.playedCardCount; i++) {
                printf("(%d)", test_state.playedCards[i]);
            }
            printf("\n"); 
        }
    
        if(test == 4) {

            exp_count = 3; 
            card = treasure_map;
            plyr = plyr + 1;    // setup full deck for next player

            printf("SUB_TEST: correctly ignores played cards when counting "
                   "full deck of another player (not current turn)\n"
                   "one treasure map (%d) in each of deck, "
                   "discard, hand; 5 in played cards\n", treasure_map);

            // place a single treasure_map into current player's deck, discard, hand
            test_state.handCount[plyr] = 1;
            test_state.deckCount[plyr] = 1;
            test_state.discardCount[plyr] = 1;
        
            test_state.hand[plyr][0] = card;
            test_state.deck[plyr][0] = card;
            test_state.discard[plyr][0] = card;

            test_state.playedCardCount = def_pile_size;
            for(i = 0; i < test_state.playedCardCount; i++) {
                test_state.playedCards[i] = card;
            }

            printf("played cards: ");
            for(i = 0; i < test_state.playedCardCount; i++) {
                printf("(%d)", test_state.playedCards[i]);
            }
            printf("\n");         
        }

        // output list of cards, call fullDeckCount, store results
        output_card_list(plyr, &test_state);
        count = fullDeckCount(plyr, card, &test_state);

        // output result of this test, report any mismatches
        printf("comparing card count to expected count of %d...\n", exp_count);
        if(count != exp_count) {
            printf("--ERROR: count: %d, expected: %d\n", count, exp_count);
            pass = FALSE;
        }
        else {
            printf("--OK: count matches expected\n");
        }
        printf("\n");
    }

    // output overall test results
    output_test_result(pass);

    return 0;
}

