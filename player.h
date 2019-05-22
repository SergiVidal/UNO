#ifndef UNO_PLAYER_H
#define UNO_PLAYER_H

#include "stack.h"
#include "list.h"


typedef struct {
    char name[MAXC];
    char type[MAXC]; //2 - Agresivo, 1 - Calmado, 0 - Player
    int num_cards;
    int wins;
    int loses;
    PDIList *handList;
    int previous_games[MAXC];
} Player;

#endif //UNO_PLAYER_H
