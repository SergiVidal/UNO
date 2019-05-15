#ifndef UNO_GAME_H
#define UNO_GAME_H

#include "player.h"
#include "listbi.h"

typedef struct {
    int total_players;
    int direction; // 0 = horario, 1 = antihorario
    ListBi player_list;
    Stack deck;
    Stack discard_deck;
} Game;

//Game GAME_create_game();

Player* GAME_create_bots(char * filename, Game *game); //Player*

Player GAME_create_player(char * filename, Game *game);

Player* GAME_create_player_list(Player *bots, Player player, Game *game);

void GAME_init_hands(Stack *stack, Player *player_list, Game *game);

void GAME_swap(Player *p1, Player *p2);

void GAME_sort_by_name(Player *players, int length);

int GAME_is_end(Game *game);

void GAME_init_game(Game *game, Player *players);

void GAME_play(Game *game);

void GAME_show_cards(Player player);

//void GAME_show_players(ListBi *list);

void GAME_display_game_status(Game *game);

void GAME_display_actions(Game *game);

void GAME_get_card(Game *game);
#endif //UNO_GAME_H
