#include <stdio.h>
#include <stdlib.h>

#include "cli.h"
#include "stack.h"
#include "player.h"
#include "game.h"

//uno fichero_bots.uno fichero_usuario.uno


int main(int argc, char *argv[]) {
//    if (argc >= 3) {

    Stack deck;
    Stack discardDeck;
    ListBi player_list;
    Player *players;
    Player *bots;
    Player player;

    Player *aux;

    Game game = {0, 0, NULL, NULL, NULL};
    Card *aux_card = NULL;
    int option;
    do {
        option = CLI_get_option();
        switch (option) {
            case CLI_PLAY:
                // Crea Bots y Jugador
                bots = GAME_create_bots(argv[1], &game);
                player = GAME_create_player(argv[2], &game);
                players = GAME_create_player_list(bots, player, &game);
                free(bots);

                GAME_init_game(&game, players);
                GAME_show_players(&game.player_list);
                // Printa la carta actual
                printf("### %d %s ###\n", game.discard_deck.last->card->value, game.discard_deck.last->card->color);

//                STACK_show_stack(game.deck);
//                STACK_show_stack(game.discard_deck);

//                GAME_show_cards(game.player_list, game.total_players);
//

//
//                GAME_play(&game);

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