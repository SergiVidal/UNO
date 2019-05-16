#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "game.h"
#include "player.h"
#include "cli.h"

// Fix function to create game and lets play another game
//Game GAME_create_game(){
//    Game game = {0, 0, NULL, NULL, NULL};
//    return game;
//}

Player *GAME_create_bots(char *filename, Game *game) {
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
        player.name[strlen(player.name) - 1] = '\0';
        fscanf(f, "%d", &player.wins);
        fscanf(f, "%d", &player.loses);
        strcpy(player.type, "Player");
        player.num_cards = 7;
        (game->total_players)++;
        fclose(f);
    }
    return player;
}

Player *GAME_create_player_list(Player *bots, Player player, Game *game) {
    Player *players = NULL;
    players = (Player *) realloc(players, sizeof(Player) * game->total_players);

    for (int i = 0; i < game->total_players - 1; i++) {
        players[i] = bots[i];

    }
    players[game->total_players - 1] = player;

    return players;
}

void GAME_swap(Player *p1, Player *p2) {
    Player tmp = *p1;
    *p1 = *p2;
    *p2 = tmp;
}

void GAME_sort_by_name(Player *players, int length) {
    int min_pos;

    for (int i = 0; i < length - 1; i++) {
        min_pos = i;
        for (int j = i + 1; j < length; j++) {
            if (strcmp(players[min_pos].name, players[j].name) > 0) {
                min_pos = j;
            }
        }
        GAME_swap(&players[min_pos], &players[i]);
    }
}

void GAME_init_hands(Stack *stack, Player *player_list, Game *game) {
    Card *card;
    PDIList *handList = NULL;

    handList = (PDIList *) realloc(handList, sizeof(PDIList) * game->total_players);
    for (int i = 0; i < game->total_players; i++) { //i < totalbots
        handList[i] = LIST_create();
        for (int j = 0; j < player_list[i].num_cards; j++) {
            card = STACK_pop(stack); // Saca una carta del Deck
            LIST_insert(&handList[i], card); // Inserta una carta en la PDIList
        }
        //Falta añadir la handList a cada uno de los players
        player_list[i].handList = &handList[i];
    }
}

void GAME_init_game(Game *game, Player *players) {
    // Init
    Stack deck;
    Stack discardDeck;
    ListBi player_list;

    // Genera y Mezcla BARAJA
    deck = STACK_fill_deck();
    deck = STACK_randomize(&deck);
    game->deck = deck;

    // Genera baraja DESCARTE
    discardDeck = STACK_create();
    game->discard_deck = discardDeck;

    // Reparte cartas a los jugadores
    GAME_init_hands(&game->deck, players, game);

    // Players
    GAME_sort_by_name(players, game->total_players);
    player_list = LISTBI_create();
    for (int i = 0; i < game->total_players; ++i) {
        LISTBI_insert(&player_list, &players[i]);
    }
    game->player_list = player_list;

    // Quita una carta del deck (la que se va a jugar) y la pone en la pila de descarte
    STACK_push(&game->discard_deck, STACK_pop(&game->deck));
}


void GAME_show_player_list(Game *game) {
    Player player;
    NodeBi *n = game->player_list.first->next;
    char direction[MAXC];
    while (n->next != NULL) {
        player = n->player;
        if (strcmp(player.type, "Player") == 0) {
            if (game->direction == 0) {
                strcpy(direction, "v");
            } else {
                strcpy(direction, "^");
            }
            printf("\t%s - %d cards \t%s \n\n", player.name, player.num_cards, direction); // player.wins, player.loses
        } else {
            printf("\t%s - %d cards \n\n", player.name, player.num_cards); // player.wins, player.loses
        }
        n = n->next;
    }
}

Player GAME_get_itself(Game *game) {
    NodeBi *n = game->player_list.first->next;
    while (n->next != NULL) {
        if (strcmp(n->player.type, "Player") == 0) {
            return n->player;
        }
        n = n->next;
    }
    return n->player;
}

int GAME_is_end(Game *game) {
    NodeBi *n = game->player_list.first->next;
    while (n->next != NULL) {
        if (n->player.num_cards == 0) {
            if (strcmp(n->player.type, "Player") == 0) {
                printf("Has ganado la partida!\n");
            } else {
                printf("%s ha ganado la partida. Te quedaban %d en mano.\n", n->player.name,
                       GAME_get_itself(game).num_cards);
            }
            return 1; // Empty
        }
        n = n->next;
    }
    return 0; // Not Empty
}

// Muestra los players y la cantidad de cartas
// + La carta actual
void GAME_display_game_status(Game *game) {
    Card *card = STACK_top(&game->discard_deck);
    printf("\n");
    GAME_show_player_list(game);
    char value[MAXC];

    if (card->type == 1) {
        strcpy(value, "Block Turn");
        printf("\t### %s %s ###\n", value, card->color);

    } else if (card->type == 2) {
        strcpy(value, "Change Direction");
        printf("\t### %s %s ###\n", value, card->color);

    } else if (card->type == 3) {
        strcpy(value, "Get +2");
        printf("\t### %s %s ###\n", value, card->color);

    } else if (card->type == 4) {
        strcpy(value, "Get +4");
        printf("\t### %s ###\n", value);

    } else if (card->type == 5) {
        strcpy(value, "Change Color");
        printf("\t### %s ###\n", value);
    } else {
        printf("\t### %d %s ###\n", card->value, card->color);
    }
    printf("\n");
}

//Show Player Cards Available
void GAME_show_cards(Game *game) {
    Player player = LISTBI_get(&game->player_list);
    Card *card;
    LIST_go_first(player.handList);
    int cont = 0;
    char value[MAXC];
    printf("\n");
    printf("%s - Tienes %d cartas: \n", player.name, player.num_cards);

    while (player.handList->last->next != NULL) {
        card = LIST_get(player.handList);
        cont++;
        if (card->type == 1) {
            strcpy(value, "Block Turn");
            printf("\t%d. [%s %s]\n", cont, value, card->color);

        } else if (card->type == 2) {
            strcpy(value, "Change Direction");
            printf("\t%d. [%s %s]\n", cont, value, card->color);

        } else if (card->type == 3) {
            strcpy(value, "Get +2");
            printf("\t%d. [%s %s]\n", cont, value, card->color);

        } else if (card->type == 4) {
            strcpy(value, "Get +4");
            printf("\t%d. [%s]\n", cont, value);

        } else if (card->type == 5) {
            strcpy(value, "Change Color");
            printf("\t%d. [%s]\n", cont, value);
        } else {
            printf("\t%d. [%d - %s]\n", cont, card->value, card->color);
        }
        LIST_next(player.handList);
    }
    LIST_go_first(player.handList);

    printf("\n%s, escoge una acción:\n", player.name);
    int option;
    do {
        option = CLI_get_more_action();
        switch (option) {
            case CLI_PLAY_CARD:
                GAME_throw_card(game);
                break;
            case CLI_GET_CARD:
                GAME_player_pick_card(game);
                break;
            default:
                printf("Opcion incorrecta\n\n");
                break;
        }
    } while (option < 1 || option > 2);


}

void GAME_display_actions(Game *game) {
    Player player = LISTBI_get(&game->player_list);

    printf("%s, escoge una acción:\n", player.name);
    int option;
    do {
        option = CLI_get_action();
        switch (option) {
            case CLI_SHOW_HAND:
                GAME_show_cards(game);
                break;
            case CLI_GET_CARD:
                GAME_player_pick_card(game);
                break;
            default:
                printf("Opcion incorrecta\n\n");
                break;
        }
    } while (option < 1 || option > 2);
}


void GAME_player_pick_card(Game *game) {
    char value[MAXC];
    Player player = LISTBI_get(&game->player_list);
    Card *card = STACK_pop(&game->deck); // Saca una carta del Deck
    LIST_insert(player.handList, card); // Inserta una carta en la PDIList

    (game->player_list.pdi->player.num_cards)++; // Aumenta el num de cartas del player actual

    if (strcmp(player.type, "Player") == 0) {
        printf("%s, has robado una carta\n\n", player.name);
        if (card->type == 1) {
            strcpy(value, "Block Turn");
            printf("Se ha robado un %s - %s. ¿Deseas jugarlo? [S/N]\n", value, card->color);
        } else if (card->type == 2) {
            strcpy(value, "Change Direction");
            printf("Se ha robado un %s - %s. ¿Deseas jugarlo? [S/N]\n", value, card->color);
        } else if (card->type == 3) {
            strcpy(value, "Get +2");
            printf("Se ha robado un %s - %s. ¿Deseas jugarlo? [S/N]\n", value, card->color);
        } else if (card->type == 4) {
            strcpy(value, "Get +4");
            printf("Se ha robado un %s. ¿Deseas jugarlo? [S/N]\n", value);
        } else if (card->type == 5) {
            strcpy(value, "Change Color");
            printf("Se ha robado un %s. ¿Deseas jugarlo? [S/N]\n", value);
        } else {
            printf("Se ha robado un %d - %s. ¿Deseas jugarlo? [S/N]\n", card->value, card->color);
        }
    } else if (strcmp(LISTBI_get(&game->player_list).type, "Calmado") == 0) {

        //Agresivo
    } else {

    }
    printf("\n");
//    GAME_show_cards(player);
}

// TODO: Crear funcion que devuelva el color que mas posee un jugador


// TODO: Crear funcion para el comportamiento de cada una de las cartas (se ejecuta una vez la carta tirada esta en la pila de descarte), faltan los cambios de colores
void GAME_card_behaviour(Game *game) {
    Card *card = game->discard_deck.last->card;

    if (card->type == 1) { // Block turn
        LISTBI_next(&game->player_list); // Se pasa un turno (+ despues de cada jugada hay un salto de turno implicito)

    } else if (card->type == 2) { // Change direction
        if (game->direction == 0) {
            game->direction = 1;
        } else {
            game->direction = 0;
        }

    } else if (card->type == 3) { // Get +2
        game->player_list.pdi->next->player.num_cards += 2;

    } else if (card->type == 4) { // Get +4
        game->player_list.pdi->next->player.num_cards += 4;
        // Funcion que devuelve el color que mas posee un jugador.
    } else if (card->type == 5) { // Change Color

    } else { // Number

    }

}

void GAME_throw_card(Game *game) {
    Player player = LISTBI_get(&game->player_list);
    int option = 0;

    if(strcmp(player.type, "Player") == 0) {
        while (option < 1 || option > player.num_cards) {
            option = CLI_choose_card();
            if (option < 1 || option > player.num_cards) {
                printf("\nError al seleccionar la carta\n");
            } else {
                for (int i = 0; i < option - 1; i++) {
                    LIST_next(player.handList);
                }

            }
            printf("\n");
        }
    }

    Card *card = LIST_get(player.handList);
    STACK_push(&game->discard_deck, card);
    LIST_remove(player.handList);
    (game->player_list.pdi->player.num_cards)--;
}

/*
 1. Zeros
 1.1 Zeros = color
 1.2 Zeros != color

 2. No Zeros/Comodines
 2.1 = Color
 2.2 = Color / Numero
 2.3 = Color / Especial

 3. Comodines

 4. Roba
 4.1 Calmado
 4.2 Agresivo
 */

void GAME_play_bot(Game *game) {
    Player player = LISTBI_get(&game->player_list);
    Card *actual_card = game->discard_deck.last->card;
    Card *player_card;

    while (player.handList->last->next != NULL) {
        //GET CARD
        player_card = LIST_get(player.handList);

        //  1. Zeros
        if (player_card->value == 0) {
            //  1.1 Zeros = color
            if (strcmp(player_card->color, actual_card->color) == 0) {
                // Tira Carta

                // 1.2 Zeros != color
            } else if (actual_card->value == 0) {
                //Tira carta

                // Si no es un 0 pero sigue siendo un numero..
            }
            // 2. No Zeros/Comodines
        } else if (player_card->type != 4 && player_card->type != 5) {
            // 2.1 = Color
            if (strcmp(player_card->color, actual_card->color) == 0) {
                // 2.2 = Color / Numero
                if (actual_card->type == 0) {
                    //Tira carta

                    // 2.3 = Color / Especial
                } else {

                }
            }
            // 3. Comodines
        } else if (player_card->type == 4 || player_card->type == 5) {
            // Tira carta

            // 4. Roba
        } else {
            // Robar carta:
            // 4.1 Calmado
//            if (strcmp(player.type, "Calmado") == 0) {
            // 4.2 Agresivo
//            } else {
//            }
        }

        //NEXT CARD
        LIST_next(player.handList);

    }
}

// TODO: Terminar
void GAME_play(Game *game) {
    Player player;
    LISTBI_go_first(&game->player_list);

    while (!GAME_is_end(game)) {
        player = LISTBI_get(&game->player_list);
        if (strcmp(player.type, "Player") == 0) {
            GAME_display_game_status(game);
            GAME_display_actions(game);
        } else {
            printf("%s - Ha sido su turno\n", player.name);
            //GAME_play_bot(game);
        }
//        Segun la direccion de la partida - Horaria 0 / Antihoraria 1
        if (game->direction == 0) {
            LISTBI_next(&game->player_list);
        } else {
            LISTBI_previous(&game->player_list);
        }
    }
}