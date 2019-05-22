#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "stack.h"

/**
 * Crea la estructura de datos Stack
 * @return - Stack
 */
Stack STACK_create() {
    Stack s = {0, NULL};

    return s;
}

/**
 * Inserta un elemento al Stack
 * @param stack - Stack
 * @param card - Carta
 * @return - True/False
 */
int STACK_push(Stack *stack, Card card) {
    Node *node = (Node *) malloc(sizeof(Node));

    if (node == NULL) // memory error
        return 0;

    node->card = card;
    node->next = stack->last; //Vincula el nou node al anterior
    stack->last = node; // Vincula el stack al nou node
    (stack->size)++;
    return 1;
}
/**
 * Elimina el ultimo elemento del Stack
 * @param stack - Stack
 * @return - Devuelve el elemento
 */
Card STACK_pop(Stack *stack) {
    if (stack->last == NULL) // vacía
        return stack->last->card;

    Node *tmp = stack->last;
    stack->last = stack->last->next;
    //free(tmp);
    (stack->size)--;
    return tmp->card;
}

/**
 * Devuelve el ultimo elemento del Stack
 * @param stack - Stack
 * @return - Devuelve el elemento
 */
Card STACK_top(Stack *stack) {
    if (stack->last == NULL)
        return stack->last->card;

    return stack->last->card;
}

/**
 * Comprueba si el Stack esta vacio
 * @param stack - Stack
 * @return - True/False
 */
int STACK_is_empty(Stack stack) {
    if(stack.last == NULL){
        return 1;
    }
    return 0;
}

/**
 * Elimina todos los elementos del Stack
 * @param stack - Stack
 */
void STACK_delete(Stack *stack) {
    while (stack->last != NULL){
        STACK_pop(stack);
    }
}

/**
 * No se requiere su utilización (Utilizado para testear)
 * Muestra el contenido del stack
 * @param stack - Stack
 */
void STACK_show_stack(Stack stack) {
    Node *aux = stack.last;
    int cont = 1;
    while (aux != NULL) {
        printf("%d = Value: %d - Type: %d - Color: %s\n", cont, aux->card.value, aux->card.type, aux->card.color);
        aux = aux->next;
        cont++;
    }
    free(aux);
}

/**
 * Permite la creación e insercion de un elemento
 * @param stack - Stack
 * @param value - Valor de la Carta
 * @param type - Tipo de la Carta
 * @param color - Color de la Carta
 */
void STACK_create_card(Stack *stack, int value, int type, char color[MAXC]){
    Card card;
    card.value = value;
    card.type = type;
    strcpy(card.color, color);
    STACK_push(stack, card); // A cada push s'incrementa el stack.size +1
}

/**
 * Llena el Stack con elementos (Baraja de Cartas)
 * @return - Devuelve el Stack
 */
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

    for(int i = 0; i < 2; i++) {
        // Block Turn
        STACK_create_card(&stack, -1, BLOCKTURN, "red");
        STACK_create_card(&stack, -1, BLOCKTURN, "yellow");
        STACK_create_card(&stack, -1, BLOCKTURN, "green");
        STACK_create_card(&stack, -1, BLOCKTURN, "blue");

        // Change Direction
        STACK_create_card(&stack, -1, CHANGEDIRECTION, "red");
        STACK_create_card(&stack, -1, CHANGEDIRECTION, "yellow");
        STACK_create_card(&stack, -1, CHANGEDIRECTION, "green");
        STACK_create_card(&stack, -1, CHANGEDIRECTION, "blue");

        // Get +2
        STACK_create_card(&stack, -1, GET2, "red");
        STACK_create_card(&stack, -1, GET2, "yellow");
        STACK_create_card(&stack, -1, GET2, "green");
        STACK_create_card(&stack, -1, GET2, "blue");
    }

    // Get +4
    STACK_create_card(&stack, -1, GET4, "none");
    STACK_create_card(&stack, -1, GET4, "none");
    STACK_create_card(&stack, -1, GET4, "none");
    STACK_create_card(&stack, -1, GET4, "none");


    // Change color
    STACK_create_card(&stack, -1, CHANGECOLOR, "none");
    STACK_create_card(&stack, -1, CHANGECOLOR, "none");
    STACK_create_card(&stack, -1, CHANGECOLOR, "none");
    STACK_create_card(&stack, -1, CHANGECOLOR, "none");

    return stack;
}

/**
 * Mezcla los elementos del Stack
 * @param s - Stack
 * @return - Devuelve el Stack
 */
Stack STACK_randomize(Stack *s){
    //Creo un array de nodos y lo lleno a partir del Stack (LIFO)
    Node *node = (Node *) malloc(sizeof(Node) * s->size);

    Node *aux = s->last;
    int cont = 0;
    while(aux != NULL){
        node[cont] = *aux;
        aux = aux->next;
        cont++;
    }
    free(aux);

    //Creo un nuevo stack, hago un random y lo voy llenando
    Stack randomStack = STACK_create();
    for(int i = 0; i < s->size; i++){
        while(1){
            int random = rand() % (s->size);
            //Se comprueba si ya ha tocado el mismo num random
            if(node[random].card.type != -1){
                STACK_push(&randomStack, node[random].card);
                node[random].card.type = -1;
                break;
            }else{
                random = rand() % (s->size);
            }
        }
    }
    free(node);
    return randomStack;
}