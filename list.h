#ifndef UNO_LIST_H
#define UNO_LIST_H

#include "stack.h"

//typedef struct _node {
//    Card *card;
//    struct _node *next;
//} Node;

typedef struct {
    Node *first;
    Node *last; //o *pdi
    int size; // ... metadata (#elements, etc)
} PDIList;

PDIList LIST_create();

PDIList LIST_insert(PDIList list, int e);

int LIST_remove(PDIList * list);

int LIST_get(PDIList);

int LIST_is_empty(PDIList);

void LIST_go_first(PDIList *);

void LIST_next(PDIList *);

int LIST_end (PDIList);

void LIST_destroy (PDIList *);

#endif //UNO_LIST_H
