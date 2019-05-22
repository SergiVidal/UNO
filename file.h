#ifndef UNO_FILE_H
#define UNO_FILE_H

#include "player.h"
#include "game.h"

Player* FILE_create_bots(char * filename, Game *game);

Player FILE_create_player(char * filename, Game *game);

void FILE_refresh_file(Game *game, Player *players);

#endif //UNO_FILE_H
