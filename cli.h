#ifndef _CLI_H_
#define _CLI_H_

#define CLI_PLAY                    1
#define CLI_SHOW_STATS              2
#define CLI_EXIT                    3

#define CLI_SHOW_HAND               'A'
#define CLI_GET_CARD                'B'
#define CLI_PLAY_CARD               'A'

#define CLI_PLAYER                  1
#define CLI_BOTS                    2
#define CLI_BACK                    3

void CLI_wait();

int CLI_get_option();

int CLI_which_color();

int CLI_want_throw();

int CLI_choose_card();

int CLI_show_stats();
#endif