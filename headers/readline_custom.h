#ifndef READLINE_CUSTOM_H
#define READLINE_CUSTOM_H
#include "board_display.h"

enum exit_codes {SUCCESS,EXIT_PROGRAM,EOF_SEEN,CONTINUE,FAILURE,TRUE,FALSE,YES,NO};
signed char flush_stdin(void);
signed char print_board_and_prompt_quit(struct board *the_board);
signed char print_board_and_readline(char *input,size_t input_size_temp,struct board *the_board,size_t word_length);

#endif
