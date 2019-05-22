#include <stdlib.h>

#include "list.h"

/**
 * Crea la estructura de datos LIST
 * @return List
 */
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

/**
 * Permite insertar un elemento a la Lista
 * @param list - Lista
 * @param card - Carta
 * @return - True/False
 */
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

/**
 * Mueve el cursor al principio de la lista
 * @param list - Lista
 */
void LIST_go_first(PDIList *list) {
    list->last = list->first;
}

/**
 * Elimina un elemento de la lista
 * @param list - Lista
 * @return - True/False
 */
int LIST_remove(PDIList *list) {
    if (LIST_end(*list)) { // estem al final
        return 0;
    }
    Node *tmp = list->last->next; // Serveix per eliminar de memoria
    list->last->next = list->last->next->next; // Equivalent: list->last->next = tmp->next:
    free(tmp);
    (list->size)--;

    return 1;
}

/**
 * Obtiene el elemento donde apunta el cursor
 * (Pre-condición: No esta vacia ni esta el cursor al final
 * @param list - Lista
 * @return - Devuelve el elemento
 */
Card LIST_get(PDIList *list) {
    return list->last->next->card;
}

/**
 * Comprueba si la Lista esta vacia
 * @param list - Lista
 * @return - True/False
 */
int LIST_is_empty(PDIList list) {
    return list.first->next == NULL; // Si es NULL retorna 1 (Si is empty)
}

/**
 * Desplaza el cursor una posición a la derecha
 * @param list - Lista
 * @return - True/False
 */
int LIST_next(PDIList *list) {
    if (list->last->next == NULL) {
        return 0;
    }
    list->last = list->last->next;
    return 1;
}

/**
 * Comprueba si estas al final de la lista
 * @param list - Lista
 * @return - True/False
 */
int LIST_end (PDIList list){
    return list.last->next == NULL; // Si es NULL retorna 1 (Si is empty)
}

/**
 * Destruye la Lista
 * @param list - Lista
 */
void LIST_destroy (PDIList *list) {
    LIST_go_first(list);
    while (!LIST_is_empty(*list)) { // llista no buida
        LIST_remove(list);
    }
    free(list->first); // elimino el node fantasma
    list->first = list->last = NULL;
}