#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "game.h"
#include "player.h"

//Falta Implementar lista de cartas restantes de toodas las partidas

Player *GAME_create_bots(char *filename) {
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
            printf("Name: %s Type: %s Num: %d\n", players[i].name, players[i].type, players[i].num_cards);
        }

        fclose(f);
    }
    return players;
}

Player GAME_create_player(char *filename) {
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

        printf("Name: %s Type: %s NumCards: %d Wins: %d Loses: %d\n", player.name, player.type, player.num_cards, player.wins, player.loses);

        fclose(f);
    }
    return player;
}
