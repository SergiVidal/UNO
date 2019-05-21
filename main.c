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
        Player aux;
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

                    GAME_restart_game(&game);
                    break;
                case CLI_SHOW_STATS:

                    printf("UNO - Stats\n");
                    do {
                        option = CLI_show_stats();
                        switch (option) {
                            case CLI_PLAYER:
                                printf("UNO - Player Stats\n");
                                GAME_show_player_stats(player);
                                break;
                            case CLI_BOTS:
                                printf("UNO - Bots Stats\n");
                                GAME_show_bots_stats(bots, game.total_players);
                                break;
                            case CLI_BACK:
                                break;
                            default:
                                printf("Opcion incorrecta\n\n");
                                break;
                        }
                    } while (option != CLI_BACK);
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