#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file.h"
#include "player.h"
#include "game.h"

/**
 * Lee el archivo de configuración de los bots
 * @param filename - Nombre del archivo a leer (fichero_bots.uno)
 * @param game - Estructura de datos que contiene el estado del juego
 * @return - Lista de bots
 */
Player *FILE_create_bots(char *filename, Game *game) {
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
            fgets(players[i].name, MAXC, f);
            players[i].name[strlen(players[i].name) - 1] = '\0';
            fgets(players[i].type, MAXC, f);
            players[i].type[strlen(players[i].type) - 1] = '\0';
            fscanf(f, "%d", &players[i].num_cards);
            fscanf(f, "%c", &skip);
            (game->total_players)++;
            players[i].wins = 0;
            players[i].loses = 0;
        }
        fclose(f);
        if (total > 9) {
            printf("Error, hay demasiados jugadores!\n");
            exit(0);
        }
    }
    return players;
}
/**
 * Lee el archivo de configuración del player
 * @param filename - Nombre del archivo a leer (fichero_usuario.uno)
 * @param game - Estructura de datos que contiene el estado del juego
 * @return - Player
 */
Player FILE_create_player(char *filename, Game *game) {
    FILE *f = NULL;
    Player player;
    int total = 0;
    f = fopen(filename, "r");
    if (f == NULL) {
        printf("Error intentando abrir el fichero %s\n", filename);
    } else {
        fgets(player.name, MAXC, f);
        player.name[strlen(player.name) - 1] = '\0';
        fscanf(f, "%d", &player.wins);
        fscanf(f, "%d", &player.loses);
        total = player.wins + player.loses;
        for (int i = 0; i < total; ++i) {
            fscanf(f, "%d", &player.previous_games[i]);
        }
        strcpy(player.type, "Player");
        player.num_cards = 0;
        (game->total_players)++;
        fclose(f);
    }
    return player;
}

/**
 * Actualiza el archivo de configuración del player una vez finaliza el juego.
 * @param game - Estructura de datos que contiene el estado del juego
 * @param players - Lista de jugadores
 */
void FILE_refresh_file(Game *game, Player *players) {
    Player player;
    int total = 0;
    FILE *f = NULL;
    for (int i = 0; i < game->total_players; i++) {
        if (strcmp(players[i].type, "Player") == 0) {
            player = players[i];
        }
    }
    total = player.wins + player.loses;

    f = fopen(PLAYER_FILE, "w");
    if (f == NULL) {
        printf("Error intentando abrir el fichero %s\n", PLAYER_FILE);
    } else {

        fprintf(f, "%s", player.name);
        fprintf(f, "%s", "\n");

        fprintf(f, "%d", player.wins);
        fprintf(f, "%s", "\n");

        fprintf(f, "%d", player.loses);
        fprintf(f, "%s", "\n");
        for (int i = 0; i < total; ++i) {
            fprintf(f, "%d", player.previous_games[i]);
            fprintf(f, "%s", "\n");

        }

    }

    fclose(f);
}