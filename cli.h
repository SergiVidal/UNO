#ifndef _CLI_H_
#define _CLI_H_

#define CLI_PLAY                    1
#define CLI_SHOW_STATS              2
#define CLI_EXIT                    3

#define CLI_SHOW_HAND               1
#define CLI_GET_CARD                2
#define CLI_PLAY_CARD               1

int CLI_get_option();

int CLI_get_action();

int CLI_get_more_action();

int CLI_choose_card();
#endif