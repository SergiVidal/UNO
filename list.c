#include <stdlib.h>

#include "list.h"

PDIList LIST_create() {
    PDIList list;

    list.size = 0; //check
    list.first = (Node *) malloc(sizeof(Node));
    if (list.first == NULL) {
// tractar error
    } else {
        list.last = list.first; // node fantasma es l'ultim i el primer
        list.last->next = NULL; // apuntem a NULL el node fantasma
    }
    (list.size)++;
    return list;
}

int LIST_insert(PDIList *list, Card card) {
    Node *n = (Node *) malloc(sizeof(Node));

    if (n == NULL)
        return 0;

    n->card = card;
    n->next = list->last->next;
    list->last->next = n;
    list->last = n;

    (list->size)++;

    return 1;
}

void LIST_go_first(PDIList *list) {
    list->last = list->first;
}

int LIST_remove(PDIList *list) {
    if (list->last->next == NULL) { // estem al final
        return 0;
    }
    Node *tmp = list->last->next; // Serveix per eliminar de memoria
    list->last->next = list->last->next->next; // Equivalent: list->last->next = tmp->next:
    free(tmp);

    return 1;
}

//Obte el element del punt de interes
Card LIST_get(PDIList *list) {
//    if (list->last == NULL) { // estem al final
//        return 0; // aka -1
//    }
    return list->last->next->card;
}

// NULL = 0 (direccion memoria 0x00000)
//int LIST_is_empty(PDIList list) {
//    return list.first->next == NULL; // Si es NULL retorna 1 (Si is empty)
//}


//
// Desplaça el last 1 posició
int LIST_next(PDIList *list) {
    if (list->last->next == NULL) {
        return 0;
    }
    list->last = list->last->next;
    return 1;
}

//int LIST_end (PDIList list){
//    return list.last->next == NULL; // Si es NULL retorna 1 (Si is empty)
//}

//void LIST_destroy (PDIList *list) {
//    PDI_LIST_go_first(list);
//    while (!PDI_LIST_is_empty(List*)) { // llista no buida
//        PDI_LIST_remove(list);
//    }
//    free(list->fist); // elimino el node fantasma
//    list->first = list->last = NULL;
//}