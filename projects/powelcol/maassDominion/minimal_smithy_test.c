/*
 * Colin Powell
 * CS362: Assignment 5 
 * minimal_smithy_test.c: minimal unit test to expose Smithy bug
 * (displays four card draw fault)
 */

#include "dominion.h"
#include "dominion_helpers.h"

#include <string.h>
#include <stdio.h>
#include <assert.h>

int main() {

    struct gameState state;
    state.numPlayers = 2;

    int plyr = 0;
    state.whoseTurn = plyr;

    state.playedCardCount = 0;

    int bonus = 0;  // not used

    int hand_pos = 0;
    state.handCount[plyr] = 1;    
    state.hand[plyr][0] = smithy;

    int i = 0;
    state.deckCount[plyr] = 5; 
    for(i = 0; i < 5; i++) {
        state.deck[plyr][i] = copper;
    } 


    cardEffect(smithy, 0, 0, 0, &state, hand_pos, &bonus);

    printf("********SMITHY MINIMAL TEST********\n");
    printf("Initial state: 5 cards in deck, one card (Smithy) in hand.\n");
    printf("After Player %d plays Smithy:\n", plyr);
    printf("Player %d - Expected hand size: %d, Observed hand size: %d\n", plyr, 3, state.handCount[plyr]);
    printf("Player %d - Expected deck size: %d, Observed deck size: %d\n", plyr, 2, state.deckCount[plyr]);

    return 0;
}
