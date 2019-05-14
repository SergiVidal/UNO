#include <stdio.h>
#include <stdlib.h>

#include "cli.h"
#include "stack.h"
#include "player.h"
#include "game.h"

//uno fichero_bots.uno fichero_usuario.uno


int main(int argc, char *argv[]) {
    if (argc >= 3) {
        Player *players;
        Player *bots;
        Player player;
        Game game = {0, 0, NULL, NULL, NULL};

        Player *aux;

        int option;
        do {
            option = CLI_get_option();
            switch (option) {
                case CLI_PLAY:
                    // Crea Bots y Jugador
                    bots = GAME_create_bots(argv[1], &game);
                    player = GAME_create_player(argv[2], &game);
                    players = GAME_create_player_list(bots, player, &game);

                    GAME_init_game(&game, players);

                    GAME_display_game_status(&game);

                    //Show actual Player
                    LISTBI_go_first(&game.player_list);
                    aux = LISTBI_get(&game.player_list);

                    GAME_show_cards(aux);

                    players = NULL;
                    bots = NULL;
                    free(bots);
                    free(players);
                    break;
                case CLI_SHOW_STATS:
                    break;
                case CLI_EXIT:
                    printf("Fin del programa.\n");
                    break;
                default:
                    printf("Opcion incorrecta\n\n");
                    break;
            }
        } while (option != CLI_EXIT);

        return 0;

    } else {
        printf("Error, tienes que introducir 2 argumento!\n");
        return 1;
    }
}