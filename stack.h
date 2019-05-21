#ifndef UNO_STACK_H
#define UNO_STACK_H

#define MAXC            50
#define NUMBER          0
#define BLOCKTURN       1
#define CHANGEDIRECTION 2
#define GET2            3
#define GET4            4
#define CHANGECOLOR     5

typedef struct {
    char color[MAXC];
    int value; // -1 Not Number
    int type; // 0 - Number, 1 - block turn, 2 - change direction, 3- get +2, 4 - get +4 , 5 - change color
    //int available; // Se puede tirar segun la ultima carta tirada?
} Card;

typedef struct _node{
    Card card;
    struct _node *next;
} Node;

typedef struct {
    int size;
    Node *last;
} Stack; //Deck, Discard Deck


Stack STACK_create();

int STACK_push(Stack *, Card);

Card STACK_pop(Stack *);

Card STACK_top(Stack *);

int STACK_is_empty(Stack);

void STACK_delete(Stack *);

void STACK_show_stack(Stack stack);

void STACK_create_card(Stack *stack, int value, int type, char color[MAXC]);

Stack STACK_fill_deck();

Stack STACK_randomize(Stack *s);

#endif //UNO_STACK_H
