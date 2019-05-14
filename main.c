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


//                STACK_show_stack(*game.deck);

                //Revisar porque peta si se hace el init_game()
                printf("LISTBI_show_players (Outside Function)\n");
                LISTBI_show_players(game.player_list);

//                // Genera y Mezcla BARAJA
//                deck = STACK_fill_deck();
//                deck = STACK_randomize(&deck);
//                game.deck = &deck;
//
//                // Genera baraja DESCARTE
//                discardDeck = STACK_create();
//                game.discard_deck = &discardDeck;
//
//                // Crea Bots y Jugador
//                bots = GAME_create_bots(argv[1], &game);
//                player = GAME_create_player(argv[2], &game);
//                players = GAME_create_player_list(bots, player, &game);
//                free(bots);
//
//                // Reparte cartas a los jugadores
//                GAME_init_hands(game.deck, players, &game);
//
//                // Players
//                GAME_sort_by_name(players, game.total_players);
//                player_list = LISTBI_create();
//                for (int i = 0; i < game.total_players; ++i) {
//                    LISTBI_insert(&player_list, &players[i]);
//                }
//                game.player_list = &player_list;
//                LISTBI_show_players(game.player_list);
//
//                // Quita una carta del deck (la que se va a jugar) y la pone en la pila de descarte
//                STACK_push(game.discard_deck, STACK_pop(game.deck));
//
//                // Printa la carta actual
//                printf("### %d %s ###\n", game.discard_deck->last->card->value, game.discard_deck->last->card->color);



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