
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "headers/board_display.h"

/* properties of a body part */
	
void print_man(char (*man_copy)[8]) {

	int p;
	printf("\n\n");
	for(p=0;p<7;p++) {
		printf("                 %s\n",man_copy[p]);
	}
	printf("\n\n");

}

void print_letter_hints(char *letter_hints) {

		printf("     hidden word: %s\n",letter_hints);

}

void print_dashes(char *dashes_copy) {

	printf("                  %s\n\n",dashes_copy);

}

void print_prompt(char *prompt) {
	printf("%s",prompt);
}

void clear_board(struct body_part *body_parts,char (*man_copy)[8]) {
	unsigned char i, x, y;
	for(i=0;i<6;i++){
		x = body_parts[i].x;
		y = body_parts[i].y;
		man_copy[y][x] = ' ';
	}
}

void print_error(char *error_str) {
	printf("%s\n\n",error_str);
}

void print_board(struct board* the_board)
{
	if(isatty(0)) {
		/* clear screen and move cursor to top left */
		printf("\e[2J\e[H");
	}
	print_man(the_board->man);
	print_letter_hints(the_board->letter_hints);
	print_dashes(the_board->dashes_copy);
	print_error(the_board->error_str);
	print_prompt(the_board->prompt);
}
