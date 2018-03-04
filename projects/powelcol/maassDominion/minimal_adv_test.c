/* 
 * Colin Powell
 * CS362: Minimal Adventurer Test
 * (displays played card bug)
 */

#include <stdio.h>
#include "dominion.h"
#include "dominion_helpers.h"

int main() {

    struct gameState state;
    int plyr = 0;
    int handpos = 0;
    int bonus = 0;
    int i = 0;

    state.numPlayers = 2;
    state.whoseTurn = plyr;
    state.playedCardCount = 0;
    state.discardCount[plyr] = 0;
    state.deckCount[plyr] = 5;

    for(i = 0; i < 5; i++) {
       state.deck[plyr][i] = copper; 
    }

    state.handCount[plyr] = 1;
    state.hand[plyr][handpos] = adventurer; 
    
    cardEffect(adventurer, 0, 0, 0, &state, handpos, &bonus);

    printf("********ADVENTURER MINIMAL TEST********\n");
    printf("Initial state: 2 Copper cards in deck, one card (Adventurer) in hand\n");
    printf("After Player %d plays Adventurer\n", plyr);

    printf("Player %d - Expected hand size: %d, Observed hand size: %d\n", plyr, 2, state.handCount[plyr]);     

    printf("Player %d - Expected deck size: %d, Observed deck size: %d\n", plyr, 3, state.deckCount[plyr]);     

    printf("Player %d - Expected discard size: %d, Observed discard size: %d\n", plyr, 0, state.discardCount[plyr]);

    printf("Played card count - Expected: %d, Observed: %d\n", 1, state.playedCardCount);
    printf("Cards in hand: ");
    for(i = 0; i < state.handCount[plyr]; i++) {
        printf("(%d)", state.hand[plyr][i]);
    }
    printf("\n");

    return 0;

}
