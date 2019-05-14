#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "game.h"
#include "player.h"

// Error al guardar nombre y typo, se traga un \n
Player *GAME_create_bots(char *filename, Game *game) {
    FILE *f = NULL;
    Player *players = NULL;
    char skip;
    int total = 0;

    f = fopen(filename, "r");
    if (f == NULL) {
        printf("Error intentando abrir el fichero %s\n", filename);
    } else {
        fscanf(f, "%d", &total);
        fscanf(f, "%c", &skip);
        players = (Player *) realloc(players, sizeof(Player) * total);
        for (int i = 0; i < total; i++) {
            fscanf(f, "%c", &skip);
            fgets(players[i].name, MAXC, f);
            fgets(players[i].type, MAXC, f);
            fscanf(f, "%d", &players[i].num_cards);
            fscanf(f, "%c", &skip);
            (game->total_players)++;
        }

        fclose(f);
    }
    return players;
}

// Error al guardar nombre y typo, se traga un \n
Player GAME_create_player(char *filename, Game *game) {
    FILE *f = NULL;
    Player player;

    f = fopen(filename, "r");
    if (f == NULL) {
        printf("Error intentando abrir el fichero %s\n", filename);
    } else {
        fgets(player.name, MAXC, f);
        fscanf(f, "%d", &player.wins);
        fscanf(f, "%d", &player.loses);
        strcpy(player.type, "Player");
        player.num_cards = 7;
        (game->total_players)++;
        fclose(f);
    }
    return player;
}

Player *GAME_create_player_list(Player *bots, Player player, Game *game) {
    Player *players = NULL;
    players = (Player *) realloc(players, sizeof(Player) * game->total_players);

    for (int i = 0; i < game->total_players - 1; i++) {
        players[i] = bots[i];

    }
    players[game->total_players - 1] = player;

    return players;
}

void GAME_swap(Player *p1, Player *p2) {
    Player tmp = *p1;
    *p1 = *p2;
    *p2 = tmp;
}

void GAME_sort_by_name(Player *players, int length) {
    int min_pos;

    for (int i = 0; i < length - 1; i++) {
        min_pos = i;
        for (int j = i + 1; j < length; j++) {
            if (strcmp(players[min_pos].name, players[j].name) > 0) {
                min_pos = j;
            }
        }
        GAME_swap(&players[min_pos], &players[i]);
    }
}

void GAME_init_hands(Stack *stack, Player *player_list, Game *game) {
    Card *card;
    PDIList *handList = NULL;

    handList = (PDIList *) realloc(handList, sizeof(PDIList) * game->total_players);

    for (int i = 0; i < game->total_players; i++) { //i < totalbots
        handList[i] = LIST_create();
        for (int j = 0; j < player_list[i].num_cards; j++) {
            card = STACK_pop(stack);
            LIST_insert(&handList[i], card);
        }

    }
}

//int GAME_is_end(Game *game){
//    LISTBI_go_first(game->player_list);
//    while(game->player_list->pdi->next != NULL){
//        if (game->player_list->pdi->player->num_cards == 0) {
//            return 1; // Empty
//        }
//        game->player_list->pdi = game->player_list->pdi->next;
//
//    }
//    return 0; // Not Empty
//}

void GAME_init_game(Game *game, Player *players) {
    // Init
    Stack deck;
    Stack discardDeck;
    ListBi player_list;

    // Genera y Mezcla BARAJA
    deck = STACK_fill_deck();
    deck = STACK_randomize(&deck);
    game->deck = deck;

    // Genera baraja DESCARTE
    discardDeck = STACK_create();
    game->discard_deck = discardDeck;

    // Players
    GAME_sort_by_name(players, game->total_players);
    player_list = LISTBI_create();
    for (int i = 0; i < game->total_players; ++i) {
        LISTBI_insert(&player_list, &players[i]);
    }
    game->player_list = player_list;

    // Reparte cartas a los jugadores
    GAME_init_hands(&game->deck, players, game);

    // Quita una carta del deck (la que se va a jugar) y la pone en la pila de descarte
    STACK_push(&game->discard_deck, STACK_pop(&game->deck));
}

//void GAME_play(Game *game){
//    Player *player;
//
//    LISTBI_go_first(&game->player_list);
//    while(!GAME_is_end(game)){
//        player = game->player_list.pdi->player;
//        if(player != NULL) {
//            printf("actual: %s\n", player->name);
//        } else{
//            printf("Error, Player NULL\n");
//            return;
//        }
//    }
//}

//void GAME_show_cards(ListBi player_list, int length){
//    LISTBI_go_first(&player_list);
//    for(int i = 0; i < length; i++){
//        player_list.pdi->player->pdiList
//        Player---
//        LISTBI_next(&player_list);
//
//
//    }
//
//}

void GAME_show_players(ListBi *list){
    LISTBI_go_first(list);
    Player *player;
    while(list->pdi->next != NULL){
        player = LISTBI_get(*list);
        printf("\t%s - \t%d cards \n\n", player->name, player->num_cards); // player.wins, player.loses
        list->pdi = list->pdi->next;
    }
}