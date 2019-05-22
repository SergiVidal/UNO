#ifndef UNO_GAME_H
#define UNO_GAME_H

#include "player.h"
#include "listbi.h"

#define PLAYER_FILE "fichero_usuario.uno"

typedef struct {
    int total_players;
    int direction; // 0 = horario, 1 = antihorario
    ListBi player_list;
    Stack deck;
    Stack discard_deck;
} Game;

Player* GAME_create_bots(char * filename, Game *game);

Player GAME_create_player(char * filename, Game *game);

Player* GAME_create_player_list(Player *bots, Player player, Game *game);

void GAME_refresh_file(Game *game, Player *players);

void GAME_init_hands(Stack *stack, Player *player_list, Game *game);

void GAME_sort_by_name(Player *players, int length);

void GAME_init_game(Game *game, Player *players);

int GAME_is_end(Game *game, Player *players);

void GAME_display_game_status(Game *game);

void GAME_show_cards(Game *game);

void GAME_throw_card(Game *game, Player player, Card card);

void GAME_player_pick_card(Game *game);

void GAME_display_actions(Game *game);

void GAME_play(Game *game, Player *players);

void GAME_check_card_to_throw(Game *game);

void GAME_play_bot(Game *game);

Player GAME_get_itself(Game *game);

void GAME_count_cards(Game *game);

void GAME_card_behaviour(Game *game);

void GAME_show_player_list(Game *game);

int GAME_check_card(Game *game, Card card);

void GAME_choose_action(Game *game);

void GAME_get_card(Game *game, Player player, Card card);

void GAME_restart_game(Game *game);

void GAME_show_player_stats(Game *game);

void GAME_show_bots_stats(Player *bots, int total);

#endif //UNO_GAME_H
