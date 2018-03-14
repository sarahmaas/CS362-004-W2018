/*
 * Colin Powell
 * CS362: Random Test Helpers implementation file
 * Generic helper functions useful for random unit testing
 */

#include "random_test_helpers.h"
#include <math.h>
#include "rngs.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * print_diffs: inspect changes in game state, print any difference from expectation.
 * -- parameters: tparams - pointer to an initialized test_params struct (see header file)
 *                expect - pointer to expected game state struct
 *                observe - pointer to observed game state struct (after function call)
 */


void print_diffs(test_params *tparams, struct gameState *expect, struct gameState *observe) {

    int i;
    int np;
        
    printf("-----TEST FAILED: PRINTING DIFFERENCES DETECTED-----\n");

    // compare number of players; take lowest number if different
    if(expect->numPlayers != observe->numPlayers) {
        printf("--number of players modified! comparing to lowest i count\n");
        if( expect->numPlayers < observe->numPlayers ) {
            np = expect->numPlayers;
        } 
        else {
            np = observe->numPlayers;
        }
    }
    else {
        np = expect->numPlayers;
    }
    
    printf("--NOTE: testing game with %d players, current turn: player %d\n", np, expect->whoseTurn);
    
    if(expect->whoseTurn != observe->whoseTurn) {
        printf("--current turn was changed! observed turn: %d, expected turn: %d\n", 
                observe->whoseTurn, expect->whoseTurn);
    }

    // check hand, discard, deck counts for each player
    for(i = 0; i < np; i++) {

        if( expect->deckCount[i] != observe->deckCount[i] ) {
            printf("--player %d deckCount: observed: %d, expected: %d\n", 
                    i, observe->deckCount[i], expect->deckCount[i]);    
        }
        
        if( expect->handCount[i] != observe->handCount[i] ) {
            printf("--player %d handCount: observed: %d, expected: %d\n", 
                    i, observe->handCount[i], expect->handCount[i]);    
        }

        if( expect->discardCount[i] != observe->discardCount[i] ) {
            printf("--player %d discardCount: observed: %d, expected: %d\n", 
                    i, observe->discardCount[i], expect->discardCount[i]);    
        }
    }

    // compare number of actions
    if(observe->numActions != expect->numActions) {
        printf("--number of actions: observed %d, expected: %d\n", 
                observe->numActions, expect->numActions);
    }

    // compare played cards
    if( expect->playedCardCount != observe->playedCardCount ) {
        printf("--played card count: observed %d, expected: %d\n", 
                observe->playedCardCount, expect->playedCardCount);       
    }

    if( expect->playedCards[expect->playedCardCount] !=
        observe->playedCards[observe->playedCardCount] ) {
        printf("--played cards: observed %d, expected: %d\n", 
                observe->playedCards[observe->playedCardCount - 1],
                expect->playedCards[expect->playedCardCount - 1]);       
    }

    // compare supply
    for(i = 0; i <= tparams->total_cards; i++) {
        if( observe->supplyCount[i] != expect->supplyCount[i] ) {
            printf("--supply of (%2d): observed: %d, expected: %d\n", i, 
                    observe->supplyCount[i], expect->supplyCount[i]);
        }
    }
}


/*
 * print_player_cards(): prints the content of a player's hand, deck, and discard
 *                       piles. 
 * -- parameters: plyr, the player for which we are printing cards
 *                st, a pointer to the appropriate gameState struct.
 */

void print_player_cards(int plyr, struct gameState *st) {

    int i = 0;

    printf("HAND: ");
    for(i = 0; i < st->handCount[plyr]; i++) {
        printf("(%2d)", st->hand[plyr][i]);
    }
    printf("\n");

    printf("DECK: ");
    for(i = 0; i < st->deckCount[plyr]; i++) {
        printf("(%2d)", st->deck[plyr][i]);
    }
    printf("\n");

    printf("DISCARD: ");
    for(i = 0; i < st->discardCount[plyr]; i++) {
        printf("(%2d)", st->discard[plyr][i]);
    }
    printf("\n");

}


/*
 * pick_king_cards: picks a deck of kingdom cards including Smithy
 */


void pick_king_cards(test_params *tparams) {

    int i = 0;
    int j = 0;
    int card = 0;
    int chosen = FALSE;
    int tgt_chosen = FALSE;

    while(i < NUM_KING_CARDS){
        chosen = FALSE;
//        card = rand() % (KING_CARD_END - KING_CARD_START) + KING_CARD_START;

        // pick random kingdom card from range of kingdom cards
        card = (int) floor(Random() * ((tparams->total_cards) - tparams->king_card_start)) +
                        (tparams->king_card_start);

        // look for duplicate choices - if dup, do not add to deck
        for(j = 0; j < i; j++) {
            if( tparams->k[j] == card) {
                chosen = TRUE;
            }
        }
        if(!chosen) {
            // if card is the card under test, flag that it has been chosen
            if(card == tparams->test_card) {
                tgt_chosen = TRUE;
            }
            tparams->k[i] = card;
            i++;
        } 
    }
    // if card under test not chosen, place randomly in kingdom card array
    if(!tgt_chosen) {
        tparams->k[(int) floor(Random() * NUM_KING_CARDS)] = tparams->test_card; 
    }
}

/*
 * setup_random_game(): populates deck, discard, hand based on test parameters
 */

int setup_random_game(test_params *tparams, struct gameState *st) {

    int dk, ds, hd; // counters for deck, discard, hand
    int card;
    int test_card_pos = -1; // monitors if card under test is in hand

    dk = ds = hd = 0;

    // random number of players, select player at random 
    st->numPlayers = floor(Random() * 3) + 2;   // 2 - 4 players
    st->whoseTurn = floor(Random() * st->numPlayers);
    int plyr = st->whoseTurn; 

    // enforce min. 1 cards in hand (for card under test) - max cards
    // in hand is MAX_HAND = MAX_DECK = 500 cards 
    st->handCount[plyr] = floor(Random() * tparams->max_hand_sz) + 1;

    // for deck, select from remaining cards possible - limit total
    // hand, deck, discard to MAX_DECK (500) cards 
    st->deckCount[plyr] = floor(Random() * (tparams->max_deck_sz - st->handCount[plyr]  + 1));

    // if there are cards left - if MAX_DECK has not been reached - place in discard
    if(st->handCount[plyr] + st->deckCount[plyr] < tparams->max_deck_sz) {
        st->discardCount[plyr] = floor(Random() * (tparams->max_deck_sz 
                                                   - st->handCount[plyr] 
                                                   - st->deckCount[plyr] + 1));
    }

    // place cards in deck
    while(dk < st->deckCount[plyr]) {
        card = floor(Random() * tparams->total_cards);
        if( card_in_game(card, tparams) ) {
            st->deck[plyr][dk] = card;
            dk++;    
        } 
    }

    // place cards in discard
    while(ds < st->discardCount[plyr]) {
        card = floor(Random() * tparams->total_cards);
        if( card_in_game(card, tparams) ) {
            st->discard[plyr][ds] = card;
            ds++;    
        } 
    }

    // place cards in hand
    while(hd < st->handCount[plyr]) {
        card = floor(Random() * tparams->total_cards); 
        if( card_in_game(card, tparams) ) {
            // if card is the test card, record its position
            if(card == tparams->test_card) {
                test_card_pos = hd;
            }
            st->hand[plyr][hd] = card;
            hd++;    
        } 
    }

    // place card under test in hand, if not placed already
    if( test_card_pos < 0) {
        st->hand[plyr][ (int)floor(Random() * st->handCount[plyr]) ] = tparams->test_card;
    }

    // place random number of cards in each supply
    for( card = 0; card <= tparams->total_cards; card++) {
        st->supplyCount[card] = floor(Random() * tparams->max_supply);
    } 
   
    return 0;
} 

/* 
 * card_in_game(): returns true if argument represents a card in the current
 * game, false if not.
 */

int card_in_game(int card, test_params *tparams) {
    
    int valid_kingcard = FALSE;
    int i = 0;

    // look through kingdom card choices, check if card found
    for(i = 0; i < NUM_KING_CARDS; i++) {
        if(card == tparams->k[i]) {
            valid_kingcard = TRUE;
            break;
        }
    }

    // return true if found, OR card is a non-kingdom card
    return (card <= gold || valid_kingcard);
}
