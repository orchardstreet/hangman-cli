#ifndef CONFIG_H
#define CONFIG_H
#define MAX_CHARACTERS 10
#define MIN_CHARACTERS 3 /* should not be less than three or equal or greater than MAX_CHARACTERS */
#define DICT_FILE_LOCATION "dict/5000-more-common.txt"
#define FREAD_CHUNK_SIZE 65536 /* in bytes */
#define MAX_WORDS_IN_FILE 600000 /* cannot be more than sizeof(int); */

enum exit_codes {SUCCESS,EXIT_PROGRAM,EOF_SEEN,CONTINUE,FAILURE,TRUE,FALSE,YES,NO,FINISHED};

#endif
