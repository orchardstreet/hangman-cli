#ifndef BOARD_DISPLAY_H
#define BOARD_DISPLAY_H

/* properties of a body part */
struct body_part {
	unsigned char y;
	unsigned char x;
	char part;
};
	
void print_man(char man_copy[][8]); 

void print_letter_hints(char *letter_hints,unsigned char letter_hints_present);

void print_dashes(char *dashes_copy); 

void clear_board(struct body_part *body_parts,char man_copy[][8]); 

#endif
