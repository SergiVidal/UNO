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
// TODO: Falta añadir la handList a cada uno de los players
void GAME_init_hands(Stack *stack, Player *player_list, Game *game) {
    Card *card;
    PDIList *handList = NULL;

    handList = (PDIList *) realloc(handList, sizeof(PDIList) * game->total_players);
    for (int i = 0; i < game->total_players; i++) { //i < totalbots
        handList[i] = LIST_create();
        for (int j = 0; j < player_list[i].num_cards; j++) {
            card = STACK_pop(stack); // Saca una carta del Deck
            LIST_insert(&handList[i], card); // Inserta una carta en la PDIList
        }
        //Falta añadir la handList a cada uno de los players
        player_list[i].handList = &handList[i];
    }
}

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

    // Reparte cartas a los jugadores
    GAME_init_hands(&game->deck, players, game);

    // Players
    GAME_sort_by_name(players, game->total_players);
    player_list = LISTBI_create();
    for (int i = 0; i < game->total_players; ++i) {
        LISTBI_insert(&player_list, &players[i]);
    }
    game->player_list = player_list;

    // Quita una carta del deck (la que se va a jugar) y la pone en la pila de descarte
    STACK_push(&game->discard_deck, STACK_pop(&game->deck));
}

//Muestra los players junto al num total de cartas
void GAME_show_players(ListBi *player_list){
    LISTBI_go_first(player_list);
    Player *player;
    while(player_list->pdi->next != NULL){
        player = LISTBI_get(player_list);
        printf("\t%s \t%d cards \n\n", player->name, player->num_cards); // player.wins, player.loses
        player_list->pdi = player_list->pdi->next;
    }
}

//Show Player Cards Available
void GAME_show_cards(Player *player){
    LIST_go_first(player->handList);
    int cont = 0;
    char value[MAXC];

    printf("%s", player->name);
    while(player->handList->last->next != NULL) {
        cont++;
        if(player->handList->last->next->card->type == 1) {
            strcpy(value, "Block Turn");
            printf("\t%d. %s %s\n", cont, value, player->handList->last->next->card->color);
        }else if (player->handList->last->next->card->type == 2){
            strcpy(value, "Change Direction");
            printf("\t%d. %s %s\n", cont, value, player->handList->last->next->card->color);
        }else if (player->handList->last->next->card->type == 3){
            strcpy(value, "Get +2");
            printf("\t%d. %s %s\n", cont, value, player->handList->last->next->card->color);
        }else if (player->handList->last->next->card->type == 4){
            strcpy(value, "Get +4");
            printf("\t%d. %s\n", cont, value);
        }else if (player->handList->last->next->card->type == 2) {
            strcpy(value, "Change Color");
            printf("\t%d. %s\n", cont, value);
        }else {
            printf("\t%d. %d %s\n", cont, player->handList->last->next->card->value, player->handList->last->next->card->color);
        }
        player->handList->last = player->handList->last->next;
    }
}
// Muestra los players y la cantidad de cartas
// + La carta actual
void GAME_display_game_status(Game *game){
    GAME_show_players(&game->player_list);
    printf("### %d %s ###\n\n", game->discard_deck.last->card->value, game->discard_deck.last->card->color); // Printa la carta actual
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