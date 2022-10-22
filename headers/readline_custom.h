#ifndef READLINE_CUSTOM_H
#define READLINE_CUSTOM_H
#include "board_display.h"

enum exit_codes {SUCCESS,EXIT_PROGRAM,EOF_SEEN,CONTINUE,FAILURE,TRUE,FALSE, UNFINISHED_MESSAGE, FINISHED_MESSAGE, CONNECTION_CLOSED, NO_MESSAGE,YES,NO};
signed char quit_prompt(struct board *the_board);
signed char flush_stdin(void);
signed char readline_custom(char *input,size_t input_size_temp,struct board *the_board,size_t word_length);

#endif
