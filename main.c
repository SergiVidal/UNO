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
    Game game = {0, 0, NULL, NULL, NULL};
    Card *aux_card = NULL;
    int option;
    do {
        option = CLI_get_option();
        switch (option) {
            case CLI_PLAY:
                deck = STACK_fill_deck(); // Genera baraja
                deck = STACK_randomize(&deck); // Mezcla Baraja
                game.deck = &deck;
//                STACK_show_stack(*game.deck);

                discardDeck = STACK_create(); // Genera baraja DESCARTE
                game.discard_deck = &discardDeck;

                Player *bots = GAME_create_bots(argv[1], &game); // Crea los bots
                Player player = GAME_create_player(argv[2], &game); // Crea el jugador
                game.player_list = GAME_create_player_list(bots, player, &game);
                free(bots);

                GAME_sort_by_name(game.player_list, game.total_players);
//                GAME_show_players(game.player_list, game.total_players);
                GAME_init_hands(game.deck, game.player_list, &game); // Reparte cartas a los jugadores

//                aux_card = STACK_pop(game.deck);
                STACK_push(game.discard_deck, STACK_pop(game.deck)); // Quita una carta del deck (la que se va a jugar) y la pone en la pila de descarte

//                printf("Deck:\n");
//                STACK_show_stack(*game.deck);
//
//                printf("Discard Deck:\n");
//                STACK_show_stack(*game.discard);

                printf("### %d %s ###\n", game.discard_deck->last->card->value, game.discard_deck->last->card->color);

                int empty = GAME_is_end(game.player_list, game.total_players);
                printf("Empty? %d\n", empty);
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