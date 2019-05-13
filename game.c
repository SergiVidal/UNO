#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "game.h"
#include "player.h"

//Falta Implementar lista de cartas restantes de toodas las partidas


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

// Unir el player a la lista de bots, se renombrara a players
Player *GAME_create_player_list(Player *bots, Player player, Game *game) {
    Player *players = NULL;
    players = (Player *) realloc(players, sizeof(Player) * game->total_players);

    for (int i = 0; i < game->total_players - 1; i++) {
        players[i] = bots[i];

    }
    players[game->total_players - 1] = player;

    return players;
}

void SORT_swap(Player *p1, Player *p2) {
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
        SORT_swap(&players[min_pos], &players[i]);
    }
}

//Se tiene que modificar para que trabaje con 1 sola lista de players, en vez de players y bots por separado - Despues de implementar GAME_create_player_list
void GAME_init_hands(Stack *stack, Player *player_list, Game *game) {
    Card *card;
    PDIList *handList = NULL;
    handList = (PDIList *) realloc(handList, sizeof(PDIList) * game->total_players); // 4 = TotalPlayers (Bots + Player)

    int i = 0;

    for (i = 0; i < game->total_players; i++) { //i < totalbots

        handList[i] = LIST_create();
        printf("%s", player_list[i].name);
        for (int j = 0; j < player_list[i].num_cards; j++) {
            card = STACK_pop(stack);
            printf("Nº: %d, Value: %d, Type: %d, Color: %s\n", j, card->value, card->type, card->color);
            LIST_insert(&handList[i], card);
        }
        printf("\n");

    }
}

void GAME_show_players(Player *player_list, int length) {
    printf("\nPlayers:\n\n");
    for (int i = 0; i < length; i++)
        printf("\t%s \t%d cards \n\n", player_list[i].name, player_list[i].num_cards); // player.wins, player.loses
}

int GAME_is_end(Player *players, int total_players){
    for (int i = 0; i < total_players; ++i) {
        if (players[i].num_cards == 0){
            return 1; // Empty
        }
    }
    return 0; // Not Empty
}
