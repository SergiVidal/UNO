#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "listbi.h"
#include "player.h"

/**
 * Crea la estructura de datos Lista Bidireccional
 * @return - Lista Bidireccional
 */
ListBi LISTBI_create(){
    ListBi list;

    list.first = (NodeBi *) malloc(sizeof(NodeBi));
    list.last = (NodeBi *) malloc(sizeof(NodeBi));

    if (list.first == NULL || list.last == NULL){
        printf("Error al crear la lista!\n");

        if(list.first != NULL){
            free(list.first);
        }
        if(list.last != NULL){
            free(list.last);
        }

    } else{
        list.pdi = list.first;
        list.first->next = list.last; //Apuntem un al altre
        list.first->prev = NULL; //Els 2 fantasmes apunten a NULL (1r.Prev y 2n.next)

        list.last->next = NULL; //Els 2 fantasmes apunten a NULL (1r.Prev y 2n.next)
        list.last->prev = list.first;//Apuntem un al altre
    }

    return list;
}
/**
 * Permite insertar un elemento a la Lista Bidireccional
 * @param list - Lista
 * @param player - Player
 * @return - True/False
 */
int LISTBI_insert(ListBi *list, Player *player){
    NodeBi *n = (NodeBi *) malloc(sizeof(NodeBi));

    if (n == NULL) {
        printf("Error al insertar una canción!\n");
    } else {
        n->player = *player; //Asignar song a N
        n->next = list->pdi->next; // El Next de N es el antiguo Next del PDI
        n->prev = list->pdi; // El Prev de N es el antiguo PDI
        list->pdi->next->prev = n; // El que iba despues del antiguo PDI, su Prev pasa a ser N
        list->pdi->next = n; // El next del antiguo PDI es N
        list->pdi = n; // El nuevo PDI es N
    }

    return 1;
}

/**
 * Mueve el cursor al principio de la Lista Bidireccional
 * @param list - Lista
 */
void LISTBI_go_first(ListBi *list){
    list->pdi = list->first->next;
}

/**
 * Desplaza el cursor una posición a la derecha
 * @param list - Lista Bidireccional
 * @return - True/False
 */
int LISTBI_next(ListBi *list){
    if (list->pdi != list->first && list->pdi != list->last) {
        if(list->pdi->next == list->last){
            list->pdi = list->first->next;
        }else{
            list->pdi = list->pdi->next;
        }
    }
    return 1;
}

/**
 * Desplaza el cursor una posición a la izquierda
 * @param list - Lista Bidireccional
 * @return - True/False
 */
int LISTBI_previous(ListBi *list){
    if (list->pdi != list->first && list->pdi != list->last) {
        if(list->pdi->prev == list->first){
            list->pdi = list->last->prev;
        }else{
            list->pdi = list->pdi->prev;
        }
    }
    return 1;
}

/**
 * Obtiene el elemento donde apunta el cursor
 * @param list - Lista Bidireccional
 * @return - Devuelve el elemento
 */
Player LISTBI_get(ListBi *list){
    if (list->pdi == list->first || list->pdi == list->last) {
        printf("No existe ningun jugador!\n\n");
    }
    return list->pdi->player;
}

/**
 * Elimina un elemento de la lista
 * @param list - Lista Bidireccional
 * @return - True/False
 */
int LISTBI_remove(ListBi *list){
    if (list->pdi == list->first || list->pdi == list->last) { // No puedes eliminar los nodos fantasmas first o last
        //printf("Error no se ha podido eliminar la canción\n\n");
        return 0;
    }
    NodeBi *tmp = list->pdi; // Serveix per eliminar de memoria

    list->pdi->prev->next = list->pdi->next;
    list->pdi->next->prev = list->pdi->prev;

    if(list->pdi->next == list->last){ // Si el next del pdi es el last..
        list->pdi = list->first->next; // el pdi sera el next del

        if(list->pdi == list->last){ // en el caso de que la lista se vacie completamente
            list->pdi = list->first;// pdi es el first
        }

    }else{
        list->pdi = list->pdi->next;

    }

    free(tmp);
    return 1;
}
/**
 * No se requiere su utilizacion
 * Destruye la Lista Bidireccional
 * @param list - Lista Bidireccional
 */
void LISTBI_destroy(ListBi *list){
    while (list->first->next != list->last) { // llista no buida
        LISTBI_remove(list);
    }
    free(list->first); // elimino el node fantasma
    free(list->pdi); // elimino el node fantasma
    free(list->last); // elimino el node fantasma
    list->first = list->last = list->pdi = NULL;
}