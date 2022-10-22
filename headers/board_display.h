#ifndef BOARD_DISPLAY_H
#define BOARD_DISPLAY_H

struct board {
	char (*man)[8];
	char *letter_hints;
	char *dashes_copy;
	char *error_str;
	char *prompt;
};
	
/* properties of a body part */
struct body_part {
	unsigned char y;
	unsigned char x;
	char part;
};
	
void print_man(char man_copy[][8]); 

void print_board(struct board *the_board);

void print_letter_hints(char *letter_hints);

void print_dashes(char *dashes_copy); 

void clear_board(struct body_part *body_parts,char man_copy[][8]); 

void print_error(char *error_str);

#endif
