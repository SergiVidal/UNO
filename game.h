#ifndef UNO_GAME_H
#define UNO_GAME_H

#include "player.h"

typedef struct {
    int total_players;
    int direction; // 0 = horario, 1 = antihorario
    Player *actual_player;
    Card *actual_card;
} Game;


Player* GAME_create_bots(char * filename, Game *game); //Player*

Player GAME_create_player(char * filename, Game *game);

void GAME_init_hands(Stack *stack, Player *bots, Player *player, Game *game);

void GAME_discard_card();

void GAME_show_hand();

void SORT_swap(Player *p1, Player *p2);

void GAME_sort_by_name(Player *players, int length);

#endif //UNO_GAME_H
