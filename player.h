#ifndef UNO_PLAYER_H
#define UNO_PLAYER_H

#include "stack.h"

#define MAX    50

typedef struct {
    char name[MAX];
    int wins;
    int loses;
    int type; //2 - Agresivo, 1 - Calmado, 0 - Player
    Hand *hand;
} Player;

typedef struct {
    char name[MAX];
    int cartas_anteriores[MAX];
} PlayerStat;

#endif //UNO_PLAYER_H
