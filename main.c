#include <stdio.h>
#include <stdlib.h>

#include "cli.h"
#include "stack.h"
#include "player.h"
#include "game.h"

//uno fichero_bots.uno fichero_usuario.uno


int main(int argc, char *argv[]) {
//    if (argc >= 3) {

    Game game = {0, 0, NULL, NULL};
    Stack deck;
    Stack discardDeck;

    int option;
    do {
        option = CLI_get_option();
        switch (option) {
            case CLI_PLAY:

                discardDeck = STACK_create(); // Genera baraja DESCARTE

                deck = STACK_fill_deck(); // Genera baraja
                deck = STACK_randomize(&deck); // Mezcla Baraja
//                STACK_show_stack(deck);

                Player *bots = GAME_create_bots(argv[1], &game); // Crea los bots
                Player player = GAME_create_player(argv[2], &game); // Crea el jugador

                printf("TotalPlayers: %d\n\n", game.total_players);

                GAME_init_hands(&deck, bots, &player, &game); // Reparte cartas a los jugadores

//                printf("After init hands:\n");
//                STACK_show_stack(deck);
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

//    } else {
//        printf("Error, tienes que introducir 2 argumento!\n");
//        return 1;
//    }
}