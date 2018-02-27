#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <stdlib.h>

// testing getCost() function

void asserttrue(int statement);


int main(int argc, char* argv[]) {
    char functionName[] = "getCost()";
	printf("____________________________________________________\n");
	printf("Testing %s\n\n", functionName);
	fflush(stdout);
    
    // for all cards in enum CARD, costs should be:
    printf("\tcurse cost:\n");
 	asserttrue(getCost(curse) == 0);
    printf("\tcurse cost:\n");
    asserttrue(getCost(curse) == 0);
    printf("\testate cost:\n");
    asserttrue(getCost(estate) == 2);
    printf("\tduchy cost:\n");
    asserttrue(getCost(duchy) == 5);
    printf("\tprovince cost:\n");
    asserttrue(getCost(province) == 8);
    printf("\tcopper cost:\n");
    asserttrue(getCost(copper) == 0);
    printf("\tsilver cost:\n");
    asserttrue(getCost(silver) == 3);
    printf("\tgold cost:\n");
    asserttrue(getCost(gold) == 6);
    printf("\tadventurer cost:\n");
    asserttrue(getCost(adventurer) == 6);
    printf("\tcouncil_room cost:\n");
    asserttrue(getCost(council_room) == 5);
    printf("\tfeast cost:\n");
    asserttrue(getCost(feast) == 4);
    printf("\tgardens cost:\n");
    asserttrue(getCost(gardens) == 4);
    printf("\tmine cost:\n");
    asserttrue(getCost(mine) == 5);
    printf("\tremodel cost:\n");
    asserttrue(getCost(remodel) == 4);
    printf("\tsmithy cost:\n");
    asserttrue(getCost(smithy) == 4);
    printf("\tvillage cost:\n");
    asserttrue(getCost(village) == 3);
    printf("\tbaron cost:\n");
    asserttrue(getCost(baron) == 4);
    printf("\tgreat_hall cost:\n");
    asserttrue(getCost(great_hall) == 3);
    printf("\tminion cost:\n");
    asserttrue(getCost(minion) == 5);
    printf("\tsteward cost:\n");
    asserttrue(getCost(steward) == 3);
    printf("\ttribute cost:\n");
    asserttrue(getCost(tribute) == 5);
    printf("\tambassador cost:\n");
    asserttrue(getCost(ambassador) == 3);
    printf("\tcutpurse cost:\n");
    asserttrue(getCost(cutpurse) == 4);
    printf("\tembargo cost:\n");
    asserttrue(getCost(embargo) == 2);
    printf("\toutpost cost:\n");
    asserttrue(getCost(outpost) == 5);
    printf("\tsalvager cost:\n");
    asserttrue(getCost(salvager) == 4);
    printf("\tsea_hag cost:\n");
    asserttrue(getCost(sea_hag) == 4);
    printf("\ttreasure_map cost:\n");
    asserttrue(getCost(treasure_map) == 4);
    printf("\tother cost:\n");
    asserttrue(getCost(treasure_map + 1) == -1);
	printf("____________________________________________________\n");
	
    return 0;
}


// adding own asserttrue as recommended
void asserttrue(int statement) {
    if (statement) {
        printf("\t\tTest successfully completed.\n");
        fflush(stdout);
    } else {
        printf("\t\tTest failed.\n");
        fflush(stdout);
    }
}