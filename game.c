#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "game.h"
#include "player.h"
#include "cli.h"
#include "file.h"

/* ***** AUXILIAR FUNCTIONS ****** */
char to_upper();

void swap(Player *p1, Player *p2);

int roundnear(int n);

/* ***** PUBLIC ***** */

/**
 * Crea una lista de jugadores (incluido los bots y el player)
 * @param bots - Lista de bots
 * @param player - Player (Jugador != bot)
 * @param game - Estructura de datos que contiene el estado del juego
 * @return Lista que contiene todos los jugadores (unidos)
 */
Player *GAME_create_player_list(Player *bots, Player player, Game *game) {
    Player *players = NULL;
    players = (Player *) realloc(players, sizeof(Player) * game->total_players);

    for (int i = 0; i < game->total_players - 1; i++) {
        players[i] = bots[i];

    }
    players[game->total_players - 1] = player;

    return players;
}

/**
 * Ordena los jugadores alfabeticamente por su nombre
 * @param players - Lista de jugadores
 * @param length - Jugadores totales
 */
void GAME_sort_by_name(Player *players, int length) {
    int min_pos;

    for (int i = 0; i < length - 1; i++) {
        min_pos = i;
        for (int j = i + 1; j < length; j++) {
            if (strcmp(players[min_pos].name, players[j].name) > 0) {
                min_pos = j;
            }
        }
        swap(&players[min_pos], &players[i]);
    }
}

/**
 * Reparte cartas a los jugadores
 * @param stack - Estructura de datos que contiene la baraja
 * @param player_list - Lista de jugadores
 * @param game - Estructura de datos que contiene el estado del juego
 */
void GAME_init_hands(Stack *stack, Player *player_list, Game *game) {
    Card card;
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

/**
 * Inicializa el juego
 * @param game - Estructura de datos que contiene el estado del juego
 * @param players - Lista de jugadores
 */
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

    // Quita una carta valida del deck (la que se va a jugar) y la pone en la pila de descarte
    do {
        STACK_push(&game->discard_deck, STACK_pop(&game->deck));
    } while (game->discard_deck.last->card.type != 0);
}
/**
 * Muestra los jugadores existentes y sus cartas restantes
 * (en el caso del player muestra tambien la dirección del juego)
 * @param game - Estructura de datos que contiene el estado del juego
 */
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

/**
 * Obtiene el player
 * @param game - Estructura de datos que contiene el estado del juego
 * @return player
 */
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
/**
 * Comprueba si se permite tirar una carta
 * @param game - Estructura de datos que contiene el estado del juego
 * @param card - Estructura de datos que forma una Carta
 * @return
 */
int GAME_check_card(Game *game, Card card) {
    Card actual_card = game->discard_deck.last->card;
    // = Color o = Valor
    if (strcmp(card.color, actual_card.color) == 0 || card.value == actual_card.value || card.type == 4 ||
        card.type == 5) {
        return 1;
    } else {
        return 0;
    }
}

/**
 * Comprueba si el juego ha terminado, si ha terminado actualiza las estructuras de datos de los jugadores
 * @param game - Estructura de datos que contiene el estado del juego
 * @param players - Lista de jugadores
 * @return
 */
int GAME_is_end(Game *game, Player *players) {
    NodeBi *n = game->player_list.first->next;
    int total = 0;

    //While true
    while (n->next != NULL) {
        //Si alguno HA GANADO
        if (n->player.num_cards == 0) {
            // Si gana el player = OK
            if (strcmp(n->player.type, "Player") == 0) {
                printf("\n\nHas ganado la partida!\n");

                for (int i = 0; i < game->total_players; i++) {
                    if (strcmp(players[i].type, "Player") != 0) {
                        players[i].loses++;
                        printf("%s - Loses: %d\n", players[i].name, players[i].loses);
                    } else {
                        //Player
                        //Wins
                        players[i].wins++;
                        printf("Wins: %d\n", players[i].wins);

                        //Add Cards
                        total = players[i].wins + players[i].loses;
                        players[i].previous_games[total - 1] = n->player.num_cards;
                    }
                }
                printf("\n\n");
            } else {
                //Si gana un bot
                for (int i = 0; i < game->total_players; i++) {
                    if (strcmp(players[i].name, n->player.name) == 0) {
                        //Bot winner
                        //Wins
                        players[i].wins++;
                        printf("%s - Wins: %d\n", players[i].name, players[i].wins);
                    } else {
                        players[i].loses++;
                        printf("%s - Loses: %d\n", players[i].name, players[i].loses);
                        if (strcmp(players[i].type, "Player") == 0) {
                            printf("%s ha ganado la partida. Te quedaban %d en mano.\n", n->player.name,
                                   players[i].num_cards);
                            //Add Cards
                            total = players[i].wins + players[i].loses;
                            players[i].previous_games[total - 1] = players[i].num_cards;
                        }

                    }
                }
            }
            FILE_refresh_file(game, players);
            GAME_restart_game(game);
            return 1; // Empty
        }
        n = n->next;
    }
    return 0; // Not Empty
}

/**
 * Muestra el estado del juego
 * @param game - Estructura de datos que contiene el estado del juego
 */
void GAME_display_game_status(Game *game) {
    Card card = STACK_top(&game->discard_deck);
    printf("\n");
    GAME_show_player_list(game);
    char value[MAXC];

    if (card.type == 1) {
        strcpy(value, "Block Turn");
        printf("\t### %s %s ###\n", value, card.color);

    } else if (card.type == 2) {
        strcpy(value, "Change Direction");
        printf("\t### %s %s ###\n", value, card.color);

    } else if (card.type == 3) {
        strcpy(value, "Get +2");
        printf("\t### %s %s ###\n", value, card.color);

    } else if (card.type == 4) {
        strcpy(value, "Get +4");
        printf("\t### %s ###\n", value);

    } else if (card.type == 5) {
        strcpy(value, "Change Color");
        printf("\t### %s ###\n", value);
    } else {
        if (card.value == 10) {
            printf("\t### X %s ###\n", card.color);

        } else {
            printf("\t### %d %s ###\n", card.value, card.color);
        }
    }
    printf("\n");
    printf("Quedan %d cartas en la deck!\n", game->deck.size);
    printf("Quedan %d cartas en la discard deck!\n", game->discard_deck.size);
    printf("\n");

}

/**
 * Permite mmostrar y escoger la segunda opción al Player
 * @param game - Estructura de datos que contiene el estado del juego
 */
void GAME_choose_action(Game *game) {
    Player player = LISTBI_get(&game->player_list);
    char option;

    printf("\n%s, escoge una acción:\n", player.name);
    do {
        printf("A. Throw card\n");
        printf("B. Get card\n");
        printf("Enter an option:\n");
        option = to_upper();
        switch (option) {
            case CLI_PLAY_CARD:
                GAME_check_card_to_throw(game);
                break;
            case CLI_GET_CARD:
                GAME_player_pick_card(game);
                break;
            default:
                printf("Opcion incorrecta\n\n");
                break;
        }
    } while (option != 'A' && option != 'B');
}

/**
 * Muestra las cartas de la mano del Player
 * @param game - Estructura de datos que contiene el estado del juego
 */
void GAME_show_cards(Game *game) {
    Player player = LISTBI_get(&game->player_list);
    Card card;
    LIST_go_first(player.handList);
    int cont = 0;
    char value[MAXC];
    printf("\n");
    printf("%s - Tienes %d cartas: \n", player.name, player.num_cards);

    while (!LIST_end(*player.handList)) {
        card = LIST_get(player.handList);
        cont++;
        if (card.type == 1) {
            strcpy(value, "Block Turn");
            if (!GAME_check_card(game, card)) {
                printf("\t%d. [%s %s]\n", cont, value, card.color);
            } else {
                printf("\t%d. [%s %s] \t * \n", cont, value, card.color);
            }
        } else if (card.type == 2) {
            strcpy(value, "Change Direction");
            if (!GAME_check_card(game, card)) {
                printf("\t%d. [%s %s]\n", cont, value, card.color);
            } else {
                printf("\t%d. [%s %s] \t * \n", cont, value, card.color);
            }
        } else if (card.type == 3) {
            strcpy(value, "Get +2");
            if (!GAME_check_card(game, card)) {
                printf("\t%d. [%s %s]\n", cont, value, card.color);
            } else {
                printf("\t%d. [%s %s] \t * \n", cont, value, card.color);
            }
        } else if (card.type == 4) {
            strcpy(value, "Get +4");
            printf("\t%d. [%s] \t * \n", cont, value);

        } else if (card.type == 5) {
            strcpy(value, "Change Color");
            printf("\t%d. [%s] \t * \n", cont, value);
        } else {
            if (!GAME_check_card(game, card)) {
                printf("\t%d. [%d - %s]\n", cont, card.value, card.color);
            } else {
                printf("\t%d. [%d - %s] * \n", cont, card.value, card.color);
            }
        }
        LIST_next(player.handList);
    }
    LIST_go_first(player.handList);

    GAME_choose_action(game);
}

/**
 * Muestra las primeras acciones permitidas al usuario
 * @param game - Estructura de datos que contiene el estado del juego
 */

void GAME_display_actions(Game *game) {
    Player player = LISTBI_get(&game->player_list);
    char option;

    printf("%s, escoge una acción:\n", player.name);
    do {
        printf("A. Show card\n");
        printf("B. Get card\n");
        printf("Enter an option:\n");

        option = to_upper();
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
    } while (option != 'A' && option != 'B');
}

/**
 * Permite lanzar una carta a los jugadores
 * @param game - Estructura de datos que contiene el estado del juego
 * @param player - Jugador
 * @param card - Estructura de datos que forma una Carta
 */
void GAME_throw_card(Game *game, Player player, Card card) {
    STACK_push(&game->discard_deck, card);
    LIST_remove(player.handList);
    (game->player_list.pdi->player.num_cards)--;
    GAME_card_behaviour(game);
}

/**
 * Permite volcar las Cartas de la Baraja de Descarte, a la Baraja
 * (Cuando la Baraja se queda sin cartas)
 * @param game - Estructura de datos que contiene el estado del juego
 */
void GAME_refill_deck(Game *game) {
    if (game->discard_deck.size == 1 && game->deck.size == 0) {
        printf("Error! Juego Finalizado! No hay cartas ni en la Deck ni en la Discard Deck!\n");
        return;
    }

    Card card = STACK_pop(&game->discard_deck); // La ultima jugada no se quita.
    while (!STACK_is_empty(game->discard_deck)) {
        STACK_push(&game->deck, STACK_pop(&game->discard_deck));
    }
    STACK_push(&game->discard_deck, card);
}

/**
 * Permite robar carta de la baraja
 * @param game - Estructura de datos que contiene el estado del juego
 */
void GAME_player_pick_card(Game *game) {
    char value[MAXC];
    int option;
    Player player = LISTBI_get(&game->player_list);
    Card card;
    if (STACK_is_empty(game->deck)) {
        printf("Se han acabado las cartas, recargando!\n");
        GAME_refill_deck(game);

    }
    card = STACK_pop(&game->deck); // Saca una carta del Deck
    LIST_go_first(player.handList);
    GAME_get_card(game, player, card);

    if (strcmp(player.type, "Player") == 0) {
        if (card.type == 1) {
            strcpy(value, "Block Turn");
        } else if (card.type == 2) {
            strcpy(value, "Change Direction");
        } else if (card.type == 3) {
            strcpy(value, "Get +2");
        } else if (card.type == 4) {
            strcpy(value, "Get +4");
        } else if (card.type == 5) {
            strcpy(value, "Change Color");
        } else {
            sprintf(value, "%d", card.value);
        }

        if (!GAME_check_card(game, card)) {
            printf("Se ha robado un %s - %s. No se puede jugar.\n\n", value, card.color);
            CLI_wait();
            return;
        } else {
            if (card.type == 4 || card.type == 5) {
                printf("Se ha robado un %s. ¿Deseas jugarlo? [1 = Si/2 = No]\n", value);
            } else {
                printf("Se ha robado un %s - %s. ¿Deseas jugarlo? [1 = Si/2 = No]\n", value, card.color);
            }
            do {
                option = CLI_want_throw();
                if (option < 1 || option > 2) {
                    printf("Opcion incorrecta!\n");
                } else {
                    if (option == 1) {
                        LIST_go_first(player.handList);
                        GAME_throw_card(game, player, card);
                    } else {
                        printf("Se ha terminado tu turno.\n");
                    }
                }
            } while (option < 1 || option > 2);
        }
        CLI_wait();
    }
    printf("\n");
}

/**
 * Cuenta cuantas cartas de cada color poseen los bots y cambiar el color segun el color mas poseido
 * @param game - Estructura de datos que contiene el estado del juego
 */
void GAME_count_cards(Game *game) {
    int red = 0, blue = 0, green = 0, yellow = 0;
    char color[MAXC];
    Player player = LISTBI_get(&game->player_list);
    Card card;

    while (!LIST_end(*player.handList)) {
        //GET CARD
        card = LIST_get(player.handList);

        if (strcmp(card.color, "red") == 0) {
            red++;
        } else if (strcmp(card.color, "blue") == 0) {
            blue++;
        } else if (strcmp(card.color, "green") == 0) {
            green++;
        } else if (strcmp(card.color, "yellow") == 0) {
            yellow++;
        }
        LIST_next(player.handList);
    }

    if (red >= blue && red >= green && red >= yellow) {
        strcpy(color, "red");
    } else if (blue >= red && blue >= green && blue >= yellow) {
        strcpy(color, "blue");
    } else if (green >= red && green >= blue && green >= yellow) {
        strcpy(color, "green");
    } else if (yellow >= red && yellow >= blue && yellow >= green) {
        strcpy(color, "yellow");
    }
    STACK_create_card(&game->discard_deck, 10, NUMBER, color);// 10= identificador
    printf("%s ha cambiado al color %s\n", player.name, color);
}

/**
 * Inserta una carta en la mano del jugador
 * @param game - Estructura de datos que contiene el estado del juego
 * @param player - Jugador
 * @param card - Estructura de datos que forma una Carta
 */
void GAME_get_card(Game *game, Player player, Card card) {
    LIST_insert(player.handList, card); // Inserta una carta en la PDIList
    (game->player_list.pdi->player.num_cards)++;
}

/**
 * Realiza el comportamiento de las Cartas una vez estas son tiradas por los jugadores
 * @param game - Estructura de datos que contiene el estado del juego
 */
void GAME_card_behaviour(Game *game) {
    Card card = game->discard_deck.last->card;
    Player player = LISTBI_get(&game->player_list);
    int value;
    int option = 0;
    if (card.type == 1) { // Block turn
        if (strcmp(player.type, "Player") != 0) {
            printf("%s juega un Salto de Turno %s!\n", player.name, card.color);
        }
        // Se pasa un turno (+ despues de cada jugada hay un salto de turno implicito)
        if (game->direction == 0) {
            LISTBI_next(&game->player_list);
        } else {
            LISTBI_previous(&game->player_list);
        }
    } else if (card.type == 2) { // Change direction
        if (strcmp(player.type, "Player") != 0) {
            printf("%s juega un Cambio de Sentido %s!\n", player.name, card.color);
        }
        if (game->direction == 0) {
            game->direction = 1;
        } else {
            game->direction = 0;
        }

    } else if (card.type == 3) { // Get +2
        if (strcmp(player.type, "Player") != 0) {
            printf("%s juega un +2!\n", player.name);
        }

        if (game->direction == 0) {
            LISTBI_next(&game->player_list);
        } else {
            LISTBI_previous(&game->player_list);
        }
        player = LISTBI_get(&game->player_list);
        for (int i = 0; i < 2; i++) {
            if (STACK_is_empty(game->deck)) {
                printf("Se han acabado las cartas, recargando!\n");
                GAME_refill_deck(game);
            }
            LIST_insert(player.handList, STACK_pop(&game->deck)); // Inserta una carta en la PDIList
            (game->player_list.pdi->player.num_cards)++;

        }


    } else if (card.type == 4) { // Get +4
        if (strcmp(player.type, "Player") != 0) {
            printf("%s juega un +4!\n", player.name);
            LIST_go_first(player.handList);
            // Cambio color bot
            GAME_count_cards(game);
        } else {
            do {
                option = CLI_which_color();
                if (option < 1 || option > 4) {
                    printf("Opcion incorrecta\n");
                } else {
                    value = 10; // Valor imposible para poder identificar
                    if (option == 1) { // Red
                        STACK_create_card(&game->discard_deck, value, NUMBER, "red");
                    } else if (option == 2) { // Blue
                        STACK_create_card(&game->discard_deck, value, NUMBER, "blue");
                    } else if (option == 3) { // Green
                        STACK_create_card(&game->discard_deck, value, NUMBER, "green");
                    } else if (option == 4) { // Yellow
                        STACK_create_card(&game->discard_deck, value, NUMBER, "yellow");
                    }
                }
            } while (option < 1 || option > 4);
        }
        // Se pasa un turno (+ despues de cada jugada hay un salto de turno implicito)
        if (game->direction == 0) {
            LISTBI_next(&game->player_list);
        } else {
            LISTBI_previous(&game->player_list);
        }
        player = LISTBI_get(&game->player_list);
        for (int i = 0; i < 4; i++) {
            if (STACK_is_empty(game->deck)) {
                printf("Se han acabado las cartas, recargando!\n");
                GAME_refill_deck(game);
            }
            GAME_get_card(game, player, STACK_pop(&game->deck));
        }

    } else if (card.type == 5) { // Change Color
        if (strcmp(player.type, "Player") != 0) {
            printf("%s juega un Cambio de color!\n", player.name);
            LIST_go_first(player.handList);
            // Cambio color bot
            GAME_count_cards(game);
        } else {
            do {
                option = CLI_which_color();
                if (option < 1 || option > 4) {
                    printf("Opcion incorrecta\n");
                } else {
                    value = 10; // Valor para poder identificar
                    if (option == 1) { // Red
                        STACK_create_card(&game->discard_deck, value, NUMBER, "red");
                    } else if (option == 2) { // Blue
                        STACK_create_card(&game->discard_deck, value, NUMBER, "blue");
                    } else if (option == 3) { // Green
                        STACK_create_card(&game->discard_deck, value, NUMBER, "green");
                    } else if (option == 4) { // Yellow
                        STACK_create_card(&game->discard_deck, value, NUMBER, "yellow");
                    }
                }
            } while (option < 1 || option > 4);
        }
    } else {
        if (strcmp(player.type, "Player") != 0) {
            printf("%s juega un %d %s!\n", player.name, card.value, card.color);
        }
    }
}

/**
 * Comprueba que puedes tirar alguna carta una vez seleccionada la opcion ThrowCard (Player)
 * @param game - Estructura de datos que contiene el estado del juego
 */
void GAME_check_card_to_throw(Game *game) {
    Player player = LISTBI_get(&game->player_list);
    Card card;
    int option = 0;
    int cont = 0;
    LIST_go_first(player.handList);
    while (!LIST_end(*player.handList)) {
        card = LIST_get(player.handList);
        if (GAME_check_card(game, card)) {
            cont++;
        }
        LIST_next(player.handList);
    }
    if (cont == 0) {
        printf("\nNo puedes tirar ninguna carta, robando...\n");
        CLI_wait();
        GAME_player_pick_card(game);
        return;
    }

    if (strcmp(player.type, "Player") == 0) {
        do {
            option = CLI_choose_card();
            if (option < 1 || option > player.num_cards) {
                printf("\nError al seleccionar la carta\n");
            } else {
                LIST_go_first(player.handList);
                for (int i = 0; i < option - 1; i++) {
                    LIST_next(player.handList);
                }
                card = LIST_get(player.handList);
                if (!GAME_check_card(game, card)) {
                    printf("\nNo se puede jugar esa carta\n");
                } else {
                    GAME_throw_card(game, player, card);
                }
            }
            printf("\n");
        } while (option < 1 || option > player.num_cards || !GAME_check_card(game, card));
    }
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
 4.1 Calmado - Si roba un comodin lo guardará
 4.2 Agresivo - Si roba un comodin lo tirara
 */

/**
 * Establece el comportamiento de los bots segun sus preferencias
 * @param game - Estructura de datos que contiene el estado del juego
 */
void GAME_play_bot(Game *game) {
    Player player = LISTBI_get(&game->player_list);
    Card actual_card = game->discard_deck.last->card;
    Card player_card;

    while (!LIST_end(*player.handList)) {
        player_card = LIST_get(player.handList);
        //  1. Zeros
        if (player_card.value == 0) {
            //  1.1 Zeros = color
            if (strcmp(player_card.color, actual_card.color) == 0) {
                GAME_throw_card(game, player, player_card);
                return;
                // 1.2 Zeros != color
            } else if (actual_card.value == 0) {
                GAME_throw_card(game, player, player_card);
                return;
            }
            // 2. No Zeros/Comodines
        } else if (player_card.type != 4 && player_card.type != 5) {
            // 2.1 = Color
            if (strcmp(player_card.color, actual_card.color) == 0) {
                // 2.2 = Color / Numero
                if (actual_card.type == 0) {
                    GAME_throw_card(game, player, player_card);
                    return;
                    // 2.3 = Color / Especial
                } else if (actual_card.type > 0 && actual_card.type < 4) {
                    GAME_throw_card(game, player, player_card);
                    return;
                }
            }
            // 3. Comodines
        } else if (player_card.type == 4 || player_card.type == 5) {
            GAME_throw_card(game, player, player_card);
            return;
        }
        LIST_next(player.handList);
    }
    //Robar carta
    if (LIST_end(*player.handList)) {
        LIST_go_first(player.handList);
        if (STACK_is_empty(game->deck)) {
            printf("Se han acabado las cartas, recargando!\n");
            GAME_refill_deck(game);
        }
        GAME_get_card(game, player, STACK_pop(&game->deck));
        printf("%s roba una carta.\n", player.name);
        if (GAME_check_card(game, player.handList->last->card) && strcmp(player.type, "Agresivo") == 0) {
            GAME_throw_card(game, player, player.handList->last->card);
            return;
        } else {
            printf("%s pasa su turno.\n", player.name);
            return;
        }
    }
}

/**
 * Motor del juego
 * @param game - Estructura de datos que contiene el estado del juego
 * @param players - Lista de jugadores
 */
void GAME_play(Game *game, Player *players) {
    Player player;
    LISTBI_go_first(&game->player_list);
    system("clear");

    while (!GAME_is_end(game, players)) {
        player = LISTBI_get(&game->player_list);
        if (strcmp(player.type, "Player") == 0) {
            GAME_display_game_status(game);
            GAME_display_actions(game);
            system("clear");
        } else {
            GAME_play_bot(game);
        }
        if (game->direction == 0) {
            LISTBI_next(&game->player_list);
        } else {
            LISTBI_previous(&game->player_list);
        }
        if (game->discard_deck.size == 1 && game->deck.size == 0) {
            printf("Error! Juego Finalizado! No hay cartas ni en la Deck ni en la Discard Deck!\n");
            return;
        }
    }
}

/**
 * Reinicia el juego
 * @param game - Estructura de datos que contiene el estado del juego
 */
void GAME_restart_game(Game *game) {
    game->direction = 0;
    STACK_delete(&game->deck);
    STACK_delete(&game->discard_deck);
    Player player;
    LISTBI_go_first(&game->player_list);
    for (int i = 0; i < game->total_players; i++) {
        player = LISTBI_get(&game->player_list);
        LIST_destroy(player.handList);
        game->player_list.pdi->player.num_cards = 0;
        LISTBI_next(&game->player_list);
    }
}

/**
 * Permite al usuario entrar en la sección de estadisticas
 * @param game - Estructura de datos que contiene el estado del juego
 */
void GAME_show_stats(Game *game){
    int option;
    printf("UNO - Stats\n");
    do {
        option = CLI_show_stats();
        switch (option) {
            case CLI_PLAYER:
                printf("UNO - Player Stats\n");
                GAME_show_player_stats(game);
                break;
            case CLI_BOTS:
                printf("UNO - Bots Stats\n");
                GAME_show_bots_stats(game);
                break;
            case CLI_BACK:
                break;
            default:
                printf("Opcion incorrecta\n\n");
                break;
        }
    } while (option != CLI_BACK);
}

/**
 * Muestra las estadisticas del Player
 * @param game - Estructura de datos que contiene el estado del juego
 */
void GAME_show_player_stats(Game *game) {
    Player player = GAME_get_itself(game);
    printf("Nombre: %s\n\n", player.name);

    int total = player.wins + player.loses;
    float wins = ((float) player.wins / total) * 100;
    float loses = ((float) player.loses / total) * 100;

    char visual_wins[MAXC];
    char visual_loses[MAXC];

    int roundwins = roundnear((int) wins) / 10;
    for (int i = 0; i < roundwins; i++) {
        visual_wins[i] = '.';
    }
    visual_wins[roundwins] = '\0';

    int roundloses = roundnear((int) loses) / 10;
    for (int i = 0; i < roundloses; i++) {
        visual_loses[i] = '.';
    }
    visual_loses[roundloses] = '\0';

    printf("Games Stats:\n");
    printf("\t Wins: %s %d (%.2f percent)\n", visual_wins, player.wins, wins);
    printf("\t Loses: %s %d (%.2f percent)\n", visual_loses, player.loses, loses);

}

/**
 * Muestra las estadisticas de los Bots
 * @param game - Estructura de datos que contiene el estado del juego
 */
void GAME_show_bots_stats(Game *game) {
    int aggree_wins = 0;
    int aggre_loses = 0;
    int calm_wins = 0;
    int calm_loses = 0;
    int aggre_total = 0;
    int calm_total = 0;
    int total_wins = 0;
    int total_loses = 0;
    int total = 0;

    printf("Name \t\t\t\t Wins \t\t\t\t Loses\n");
    printf("---------------------------------------------------------------------\n");
    NodeBi *n = game->player_list.first->next;
    while (n->next != NULL) {
        if (strcmp(n->player.type, "Player") != 0) {
            printf("%s \t\t\t %d \t\t\t\t %d\n", n->player.name, n->player.wins, n->player.loses);
            if (strcmp(n->player.type, "Agresivo") == 0) {
                aggree_wins = aggree_wins + n->player.wins;
                aggre_loses = aggre_loses + n->player.loses;
            } else {
                calm_wins = calm_wins + n->player.wins;
                calm_loses = calm_loses + n->player.loses;
            }
        }
        n = n->next;
    }


    aggre_total = aggree_wins + aggre_loses;
    calm_total = calm_wins + calm_loses;
    total_wins = aggree_wins + calm_wins;
    total_loses = aggre_loses + calm_loses;
    total = total_wins + total_loses;

    float aggree_winsper = (((float) aggree_wins / aggre_total) * 100);
    aggree_winsper = aggree_winsper > 0 ? aggree_winsper : 0;

    float aggreelosesper = ((float) aggre_loses / aggre_total) * 100;
    aggreelosesper = aggreelosesper > 0 ? aggreelosesper : 0;

    float calm_winsper = ((float) calm_wins / calm_total) * 100;
    calm_winsper = calm_winsper > 0 ? calm_winsper : 0;

    float calm_losesper = ((float) calm_loses / calm_total) * 100;
    calm_losesper = calm_losesper > 0 ? calm_losesper : 0;

    float total_winsper = ((float) total_wins / total) * 100;
    total_winsper = total_winsper > 0 ? total_winsper : 0;

    float total_losesper = ((float) total_loses / total) * 100;
    total_losesper = total_losesper > 0 ? total_losesper : 0;

    printf("---------------------------------------------------------------------\n");
    printf("Agresivo \t\t\t %d (%.2f percent) \t\t %d (%.2f percent)\n", aggree_wins, aggree_winsper,
           aggre_loses,
           aggreelosesper);
    printf("Calmados \t\t\t %d (%.2f percent) \t\t %d (%.2f percent)\n", calm_wins, calm_winsper, calm_loses,
           calm_losesper);
    printf("---------------------------------------------------------------------\n");
    printf("Total \t\t\t\t %d (%.2f percent) \t\t %d (%.2f percent)\n", total_wins, total_winsper, total_loses,
           total_losesper);
    printf("---------------------------------------------------------------------\n");
}


/* ***** PRIVADAS ***** */
/**
 * Convierte una letra Minuscula en Mayuscula
 * @return Letra en Mayuscula
 */
char to_upper() {
    char input[5];
    fgets(input, 5, stdin);

    if (input[0] >= 'a' && input[0] <= 'z') {
        input[0] = input[0] - ('a' - 'A');
        return input[0];
    }
    return input[0];
}

/**
 * Intercambia 2 jugadores
 * @param p1 - Jugador 1
 * @param p2 - Jugador 2
 */
void swap(Player *p1, Player *p2) {
    Player tmp = *p1;
    *p1 = *p2;
    *p2 = tmp;
}

/**
 * Redondea un numero al mutliplo de 10 mas cercano
 * @param n - Numero
 * @return - Numero multiplo de 10
 */
int roundnear(int n) {
    // Smaller multiple
    int a = (n / 10) * 10;

    // Larger multiple
    int b = a + 10;

    // Return of closest of two
    return (n - a > b - n) ? b : a;
}
