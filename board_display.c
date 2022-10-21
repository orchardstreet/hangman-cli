
#include <stdlib.h>
#include <stdio.h>
#include "headers/board_display.h"

/* properties of a body part */
	
void print_man(char man_copy[][8]) {

	int p;
	for(p=0;p<7;p++) {
		printf("      %s\n",man_copy[p]);
	}
	printf("\n");

}

void print_letter_hints(char *letter_hints,unsigned char letter_hints_present) {

	if(letter_hints_present) {
		printf("      %s\n",letter_hints);
	} else {
		printf("\n");
	}

}

void print_dashes(char *dashes_copy) {

	printf("      %s\n\n",dashes_copy);

}

void clear_board(struct body_part *body_parts,char man_copy[][8]) {
	unsigned char i, x, y;
	for(i=0;i<6;i++){
		x = body_parts[i].x;
		y = body_parts[i].y;
		man_copy[y][x] = ' ';
	}
}
