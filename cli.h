#ifndef _CLI_H_
#define _CLI_H_

#define CLI_PLAY                    1
#define CLI_SHOW_STATS              2
#define CLI_EXIT                    3

#define CLI_SHOW_HAND               'A'
#define CLI_GET_CARD                'B'
#define CLI_PLAY_CARD               'A'

void CLI_wait();

int CLI_get_option();

char CLI_get_action();

int CLI_which_color();

int CLI_want_throw();

char CLI_get_more_action();

int CLI_choose_card();
#endif