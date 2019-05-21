#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cli.h"

/* ***** AUXILIAR FUNCTIONS ****** */

int is_number(char *option);

/* ***** PUBLIC ***** */

void CLI_wait(){
    char aux;
    do{
        printf("Press ENTER to continue...\n");
        scanf("%c", &aux);
    }while(aux != '\n');
}

int CLI_get_option() {
    char str_option[5];
    int option;
    printf("UNO\n");
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

int CLI_choose_card() {
    char str_option[5];
    int option;

    printf("Que carta quieres jugar?\n");
    fgets(str_option, 5, stdin);
    str_option[strlen(str_option) - 1] = '\0';

    //Si is_number = 0 , option = -1
    //Si no, devuelve el integer (asci to int) de la opcion seleccionada
    if (!is_number(str_option))
        option = -1;
    else option = atoi(str_option);

    return option;
}

int CLI_which_color(){
    char str_option[5];
    int option;

    printf("Which color do you want? \n");
    printf("1. Red\n");
    printf("2. Blue\n");
    printf("3. Green\n");
    printf("4. Yellow\n");
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

int CLI_want_throw(){
    char str_option[5];
    int option;

    fgets(str_option, 5, stdin);
    str_option[strlen(str_option) - 1] = '\0';

    //Si is_number = 0 , option = -1
    //Si no, devuelve el integer (asci to int) de la opcion seleccionada
    if (!is_number(str_option))
        option = -1;
    else option = atoi(str_option);

    return option;
}


char CLI_get_action() {
    char str_option[5];
    printf("A. Show card\n");
    printf("B. Get card\n");
    printf("Enter an option:\n");
    fgets(str_option, 5, stdin);
    return str_option[0];
}

char CLI_get_more_action(){
    char str_option[5];
    printf("A. Throw card\n");
    printf("B. Get card\n");
    printf("Enter an option:\n");
    fgets(str_option, 5, stdin);
    return str_option[0];
}
//int CLI_get_more_action(){
//    char str_option[5];
//    int option;
//    printf("A. Throw card\n");
//    printf("B. Get card\n");
//    printf("Enter an option:\n");
//    fgets(str_option, 5, stdin);
//    str_option[strlen(str_option) - 1] = '\0';
//
//    //Si is_number = 0 , option = -1
//    //Si no, devuelve el integer (asci to int) de la opcion seleccionada
//    if (!is_number(str_option))
//        option = -1;
//    else option = atoi(str_option);
//
//    return option;
//}

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