#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "game.h"
#include "player.h"

//Falta Implementar lista de cartas restantes de toodas las partidas

Player* GAME_create_bots(char *filename, Game *game) {
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
        printf("\nPlayers:\n\n");
        for (int i = 0; i < total; i++) {
            fscanf(f, "%c", &skip);
            fgets(players[i].name, MAXC, f);
            fgets(players[i].type, MAXC, f);
            fscanf(f, "%d", &players[i].num_cards);
            fscanf(f, "%c", &skip);
            printf("Name: %s Type: %s NumCards: %d\n", players[i].name, players[i].type, players[i].num_cards);
            printf("\n");

            (game->total_players)++;
        }

        fclose(f);
    }
    return players;
}

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
        strcpy(player.type,"Player");
        player.num_cards = 7;

        printf("Name: %s Type: %s \n NumCards: %d \n\n", player.name, player.type, player.num_cards); //  player.wins, player.loses
        (game->total_players)++;
        fclose(f);
    }
    return player;
}


void SORT_swap(Player *p1, Player *p2){
    Player tmp = *p1;
    *p1 = *p2;
    *p2 = tmp;
}

void GAME_sort_by_name(Player *players, int length){
    int min_pos;

    for(int i = 0; i < length - 1; i++){
        min_pos = i;
        for(int j = i + 1; j < length; j++){
            if (strcmp(players[min_pos].name,players[j].name) > 0) {
                min_pos = j;
            }
        }
        SORT_swap(&players[min_pos], &players[i]);
    }
}


void GAME_init_hands(Stack *stack, Player *bots, Player *player, Game *game){
    Card *card;
//    Card *aux;
    PDIList *handList = NULL;
    handList = (PDIList *) realloc(handList, sizeof(PDIList) * game->total_players); // 4 = TotalPlayers (Bots + Player)

    int i = 0;

    for (i = 0; i < game->total_players-1; i++) { //i < totalbots

        handList[i] = LIST_create();
        printf("%s", bots[i].name);
        for (int j = 0; j < bots[i].num_cards; j++) {
            card = STACK_pop(stack);
            printf("Nº: %d, Value: %d, Type: %d, Color: %s\n", j, card->value, card->type, card->color);
            LIST_insert(&handList[i], card);
        }
        printf("\n");

    }

    handList[game->total_players - 1] = LIST_create(); // 3 = last player
    printf("%s", player->name);

    for (i = 0; i < player->num_cards; i++){
        card = STACK_pop(stack);
        printf("Nº: %d, Value: %d, Type: %d, Color: %s\n", i, card->value, card->type, card->color);
        LIST_insert(&handList[game->total_players - 1], card); // 3 = last player

    }
//    aux = LIST_get(&handList[3]);
//    printf("Last card given to player: ");
//    printf("Value: %d, Type: %d, Color: %s\n", aux->value, aux->type, aux->color);

    printf("\n");

}

