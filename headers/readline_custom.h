#ifndef READLINE_CUSTOM_H
#define READLINE_CUSTOM_H
#include "board_display.h"

signed char flush_stdin(void);
signed char print_board_and_prompt_quit(struct board *the_board);
signed char print_board_and_readline(char *input,size_t input_size_temp,struct board *the_board,size_t word_length);

#endif
