#include <stdio.h>
#include <stdlib.h>

#include "cli.h"
#include "stack.h"

int main(int argc, char *argv[]) {
//    if (argc >= 3) {

    Stack deck = STACK_fill_deck();
    STACK_show_stack(deck);

    int option;

    do {
        option = CLI_get_option();
        switch (option) {
            case CLI_PLAY:
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