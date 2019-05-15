//
// Created by Sergi on 14/05/2019.
//

#ifndef UNO_LISTBI_H
#define UNO_LISTBI_H


#include "stack.h"
#include "player.h"


typedef struct _nodebi {
    Player player;
    struct _nodebi *next;
    struct _nodebi *prev;
} NodeBi;

typedef struct {
    NodeBi *first; // fantasma
    NodeBi *last; // fantasma
    NodeBi *pdi; // cursor
} ListBi;


ListBi LISTBI_create();

int LISTBI_insert(ListBi *, Player *);

void LISTBI_go_first(ListBi *list);

int LISTBI_next(ListBi *list);

int LISTBI_previous(ListBi *list);

//int PLIST_remove(ListBid *);
//
//
//int PLIST_previous(ListBid *);
//
Player LISTBI_get(ListBi *list);
//
//void PLIST_destroy(ListBid *);

void LISTBI_show_list(ListBi *player_list);

#endif //UNO_LISTBI_H
