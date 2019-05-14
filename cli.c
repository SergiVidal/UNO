#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cli.h"

/* ***** AUXILIAR FUNCTIONS ****** */

int is_number(char *option);

/* ***** PUBLIC ***** */

int CLI_get_option() {
    char str_option[5];
    int option;
    printf("1. Play\n");
    printf("2. Show statistics\n");
    printf("3. Exit\n");
    printf("Enter an option:\n");
    fgets(str_option, 5, stdin);
    str_option[strlen(str_option) - 1] = '\0';

    //Si is_number = 0 , option = -1
    //Si no, devuelve el integer (asci to int) de la opcion seleccionada
    if (!is_number(str_option))
        option = -1;
    else option = atoi(str_option);

    return option;
}

int CLI_get_action() {
    char str_option[5];
    int option;
    printf("1. Show hand\n");
    printf("2. Get card\n");
    printf("Enter an option:\n");
    fgets(str_option, 5, stdin);
    str_option[strlen(str_option) - 1] = '\0';

    //Si is_number = 0 , option = -1
    //Si no, devuelve el integer (asci to int) de la opcion seleccionada
    if (!is_number(str_option))
        option = -1;
    else option = atoi(str_option);

    return option;
}

/* ***** PRIVADAS ***** */
int is_number(char *option) {
    int i = 0;

    while (option[i] != '\0') {
        if (option[i] < '0' || option[i] > '9')
            return 0;
        i++;
    }

    return 1;
}