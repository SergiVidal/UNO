#ifndef UNO_GAME_H
#define UNO_GAME_H

#include "player.h"

Player* GAME_create_bots(char * filename); //Player*

Player GAME_create_player(char * filename);

#endif //UNO_GAME_H
