#ifndef LOAD_WORDS_H
#define LOAD_WORDS_H
#include <stdio.h>

FILE * open_dict_file(void);
int calculate_words_in_dict_file(FILE * dict_file,char *fread_chunk,char **valid_word_positions_in_file);

#endif
