/*
 * Colin Powell
 * CS362: Assignment 3
 * test_helpers.h: unit test helper functions/macros.
 * these are universally applicable helper functions used for unit testing 
 * dominion.c code.
 */

#include <stdio.h>
#include "dominion.h"

#define TRUE 1
#define FALSE 0

void output_test_result(int pass);

int test_fulldeck_counts(int exp_decks[], int exp_hands[], 
                             int exp_discard[], struct gameState *st);
int test_playedcard_count(int played_count, struct gameState *st);
int test_supply_counts(int expected [], struct gameState *st);
void output_card_list(int plyr, struct gameState *st);
void output_supply(int k[], int num_k, struct gameState *st);

