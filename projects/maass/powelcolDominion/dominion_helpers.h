#ifndef _DOMINION_HELPERS_H
#define _DOMINION_HELPERS_H

#include "dominion.h"

int drawCard(int player, struct gameState *state);
int updateCoins(int player, struct gameState *state, int bonus);
int discardCard(int handPos, int currentPlayer, struct gameState *state, 
		int trashFlag);
int gainCard(int supplyPos, struct gameState *state, int toFlag, int player);
int getCost(int cardNumber);
int cardEffect(int card, int choice1, int choice2, int choice3, 
	       struct gameState *state, int handPos, int *bonus);

int adventurerEffect( struct gameState *state );
/* Implementation of the effect of the Adventurer card - called in playCard */

int councilRoomEffect( int handPos, struct gameState *state );
/* Implementation of the effect of the Council Room card - called in playCard */

int smithyEffect( int handPos, struct gameState *state );
/* Implementation of the effect of the Smithy card - called in playCard */

int mineEffect( int handPos, int trashChoice, int gainChoice, struct gameState *state );
/* Implementation of the effect of the Mine card - called in playCard */

int villageEffect( int handPos, struct gameState *state );
/* Implementation of the effect of the Village card - called in playCard */

#endif
