#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "stack.h"

Stack STACK_create() {
    Stack s = {0, NULL};

    return s;
}

int STACK_push(Stack *stack, Card *card) {
    Node *node = (Node *) malloc(sizeof(Node));

    if (node == NULL) // memory error
        return 0;

    node->card = card;
    node->next = stack->last; //Vincula el nou node al anterior
    stack->last = node; // Vincula el stack al nou node
    (stack->size)++;
    return 1;
}

Card* STACK_pop(Stack *stack) {
    if (stack->last == NULL) // vacía
        return NULL;

    Node *tmp = stack->last;
    stack->last = stack->last->next;
    //free(tmp);

    return tmp->card;
}

Card* STACK_top(Stack *stack) {
    if (stack->last == NULL)
        return 0;

    return stack->last->card;
}

int STACK_is_empty(Stack stack) {
    return stack.last == NULL;
}

void STACK_delete(Stack *stack) {
    while (STACK_pop(stack));
}

void STACK_show_stack(Stack stack) {
    Node *aux = stack.last;
    int cont = 1;
    while (aux != NULL) {
        printf("%d = Value: %d - Type: %d - Color: %s\n", cont, aux->card->value, aux->card->type, aux->card->color);
        aux = aux->next;
        cont++;
    }
    free(aux);
}

void STACK_create_card(Stack *stack, int value, int type, char color[MAXC]){
    Card *card = (Card *) malloc (sizeof(Card));
    card->value = value;
    card->type = type;
    strcpy(card->color, color);
    STACK_push(stack, card); // A cada push s'incrementa el stack.size +1
}

//Falta crearla random
Stack STACK_fill_deck() {
    Stack stack = STACK_create();
    // Numbers
    for (int i = 0; i <= 9; i++) {
        // 0s
        if (i == 0) {
            STACK_create_card(&stack, i, NUMBER, "red");
            STACK_create_card(&stack, i, NUMBER, "yellow");
            STACK_create_card(&stack, i, NUMBER, "green");
            STACK_create_card(&stack, i, NUMBER, "blue");
        } else {
            // Other Numbers
            for (int j = 0; j < 2; j++) {
                STACK_create_card(&stack, i, NUMBER, "red");
                STACK_create_card(&stack, i, NUMBER, "yellow");
                STACK_create_card(&stack, i, NUMBER, "green");
                STACK_create_card(&stack, i, NUMBER, "blue");
            }

        }
    }

//    for(for i = 0; i < 2; i++) {
        // Block Turn

//        card->value = -1; // Not number
//        card->type = 1; // Block Turn Code
//        strcpy(card->color, "red");
//        STACK_push(&stack, card);
//
//        strcpy(card->color, "yellow");
//        STACK_push(&stack, card);
//
//        strcpy(card->color, "green");
//        STACK_push(&stack, card);
//
//        strcpy(card->color, "blue");
//        STACK_push(&stack, card);

        // Change Direction
//        card->type = 2; // Change Direction Code
//        strcpy(card->color, "red");
//        STACK_push(&stack, card);
//
//        strcpy(card->color, "yellow");
//        STACK_push(&stack, card);
//
//        strcpy(card->color, "green");
//        STACK_push(&stack, card);
//
//        strcpy(card->color, "blue");
//        STACK_push(&stack, card);

        // Get +2
//        card->type = 3; // Get +2 Code
//        strcpy(card->color, "red");
//        STACK_push(&stack, card);
//
//        strcpy(card->color, "yellow");
//        STACK_push(&stack, card);
//
//        strcpy(card->color, "green");
//        STACK_push(&stack, card);
//
//        strcpy(card->color, "blue");
//        STACK_push(&stack, card);
//    }

    // Get +4
//    card->type = 4; // Get +2 Code
//    strcpy(card->color, "red");
//    STACK_push(&stack, card);
//
//    strcpy(card->color, "yellow");
//    STACK_push(&stack, card);
//
//    strcpy(card->color, "green");
//    STACK_push(&stack, card);
//
//    strcpy(card->color, "blue");
//    STACK_push(&stack, card);


    // Change color
//    card->type = 5; // Change color Code
//    strcpy(card->color, "red");
//    STACK_push(&stack, card);
//
//    strcpy(card->color, "yellow");
//    STACK_push(&stack, card);
//
//    strcpy(card->color, "green");
//    STACK_push(&stack, card);
//
//    strcpy(card->color, "blue");
//    STACK_push(&stack, card);

    return stack;
}