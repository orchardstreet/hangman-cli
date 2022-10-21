#ifndef READLINE_CUSTOM_H
#define READLINE_CUSTOM_H

enum exit_codes {SUCCESS,EXIT_PROGRAM,EOF_SEEN,CONTINUE,FAILURE,TRUE,FALSE, UNFINISHED_MESSAGE, FINISHED_MESSAGE, CONNECTION_CLOSED, NO_MESSAGE};
signed char quit_prompt(char *prompt);
signed char flush_stdin(void);
signed char readline_custom(char *prompt,char *input,size_t input_size_temp);

#endif
