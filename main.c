#include <stdio.h>
#include <stdlib.h>

#include "cli.h"
#include "stack.h"
#include "player.h"
#include "game.h"
//uno fichero_bots.uno fichero_usuario.uno
// /users/home/alumnes/LS/s.vidal/CLION/UNO

int main(int argc, char *argv[]) {
    if (argc >= 3) {
        Player *players;
        Player *bots;
        Player player;
        Game game = {0, 0, NULL, NULL, NULL};
        int option;

        // Crea Bots y Jugador
        bots = GAME_create_bots(argv[1], &game);
        player = GAME_create_player(argv[2], &game);
        players = GAME_create_player_list(bots, player, &game);


        do {
            CLI_wait();

            system("clear");
            option = CLI_get_option();
            printf("\n");
            switch (option) {
                case CLI_PLAY:



                    GAME_init_game(&game, players);

                    GAME_play(&game);


                    players = NULL;
                    bots = NULL;
                    free(bots);
                    free(players);
                    break;
                case CLI_SHOW_STATS:
                    break;
                case CLI_EXIT:
                    printf("Fin del programa.\n");
                    players = NULL;
                    bots = NULL;
                    free(bots);
                    free(players);
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