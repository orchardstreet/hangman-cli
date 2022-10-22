#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "headers/readline_custom.h"
#include "headers/board_display.h"
#define MAX_CHARACTERS 10 

/* main function ---------------------------------------------------------------------------------------------------------------------------------------- */
int main(void) {
	
	/* init variables */
	char *word = "jambalayaa"; /* word user has to guess */
	char letter_guess[4] = {0};      /* array for getting character user entered */
	char c;                    /* character user entered, aka letter_guess[0] */
	char body_part_char;       /* holder for body part used in any given moment */
	char strcat_array[70];     /* for string concatenation */
	unsigned char i,x,y;       /* loop iterator, body part coordinates (x,y) */
	unsigned char did_find_character; /* if found character */ 
	unsigned char characters_found;   /* if found character, number of chars found */ 
	signed char retval;               /* general function return value */
	char letters_found[MAX_CHARACTERS + 1] = {0}; /* to keep track of letters found */
	char dashes[(MAX_CHARACTERS * 2) + 1];        /* array to store letter hint underlines/dashes to print on board */
	char letter_hints[(MAX_CHARACTERS * 2) + 1];  /* array to store correctly guessed letters to print on board */
	char *found_char;          /* pointer and iterator to point to found characters in answer word */
	size_t word_length;        /* the length of the word */
	char man[7][8] = {         /* hangman and man, represented in memory for printing */
		" _____ ",
		" |   | ",
		" |     ",
		" |     ",
		" |     ",
		" |     ",
	    " ------"
	};
	struct body_part body_parts[6] = { /* list of body parts and their properties for printing or removing from board */
		{2,5,'O'}, /* head */
		{3,4,'/'}, /* left arm */
		{3,5,'|'}, /* torso */
		{3,6,'\\'},/* right arm */
		{4,4,'/'}, /* left leg */
		{4,6,'\\'} /* right leg */
	};
	int body_parts_index; /* next body part to display if incorrect character chosen */

	/* Prevent stdout and stderr from appearing out of order on CLI */
	setvbuf(stdout,NULL,_IONBF,0); 
	setvbuf(stderr,NULL,_IONBF,0); 

	/* null terminate variables */
	strcat_array[49] = 0;
	
	struct board the_board = {man, letter_hints, dashes, "", ""};
	
	/* main loop for all games ------------------------------------------------------------------------------------------------------------------ */
	for(;;) {
		
		/* init main loop variables */
		body_parts_index = 0;
		characters_found = 0;

		/* find and error check word length */
		word_length = strlen(word);
		if(word_length > MAX_CHARACTERS) {
			fprintf(stderr,"Error: word rendered from dictionary exceeds max characters allowed\n");
			exit(EXIT_FAILURE);
		} else if (word_length < 1) {
			fprintf(stderr,"Error: word rendered from dictionary contains no characters\n");
			exit(EXIT_FAILURE);
		}
		
		/* init letter_hints and letters_found arrays */
		memset(letter_hints,' ',word_length * 2);
		letter_hints[word_length * 2] = 0;
		memset(letters_found,0,MAX_CHARACTERS + 1);
		
		/* generate dashes in dash array according to word length */
		memset(dashes,' ',word_length * 2); /* fill dashes array with spaces */
		dashes[word_length * 2] = 0; /* terminate dashes array with null character */
		for(i = 0;i<word_length;i++){ /* fill dashes array with appropriate number of dashes */
			dashes[i*2] = '-';
		}
		
		/* clear error_str in the_board struct */
		the_board.error_str = "";

		/* big loop for individual game, ie for a single word ----------------------------------------------------------------------------------------- */
		for(;;) {
			
			/* init variables */
			did_find_character = 0;
			
			/* print screen and get character guess from user --------------------------------------------------- */
			if(print_board_and_readline(letter_guess,4,&the_board,word_length) == EXIT_PROGRAM) {
				printf("Exiting...\n");
				exit(EXIT_SUCCESS);
			}
			c = letter_guess[0];
			
			/* Check if user already guessed the character, and if so jump to top ------------------------------- */
			if(strchr(letters_found,c)) {
				sprintf(strcat_array,"Already guessed the letter %c",c);
				the_board.error_str = strcat_array;
				continue;
			}
			
			/* Search the answer word for the character the user guessed------------------------------------------ */
			for(found_char = word;*found_char;found_char++) {
				found_char = strchr(found_char,c);
				if(!found_char) /* if word user guessed not in word, jump to top ----------------------------------*/
					break;
				/* the user guessed a correct character */
				characters_found++;
				letters_found[characters_found - 1] = c;
				letter_hints[(found_char - word) * 2] = c;
				did_find_character = 1;
			}
			
			if(did_find_character) {
				/* if the user won -------------------------------------------------------------------------------- */
				if(characters_found == word_length) {
					the_board.error_str = "You won, congratulations!";
					/* ask winner if they want to play again */
					the_board.prompt = "Would you like to play again (y)es/(n)o: ";
					retval = print_board_and_prompt_quit(&the_board);
					if(retval == EXIT_PROGRAM || retval == NO) {
						printf("Exiting...\n");
						exit(EXIT_SUCCESS);
					} else { /* winner wants to play again, clear board */
						clear_board(body_parts,man);
						break; /* breaks out of guessing loop, starting new game */
					}
				}
				/*if user guessed right but didn't win yet -----------------------------------------------------------*/
			    the_board.error_str = "Correct guess";
			} else if(!did_find_character) { /* if user guessed wrong ----------------------------------------------- */
				/* fill man array with next body part to represent loss */
				x = body_parts[body_parts_index].x;
				y = body_parts[body_parts_index].y;
				body_part_char = body_parts[body_parts_index].part;
				man[y][x] = body_part_char;
				body_parts_index++;
				/*if the user lost ----------------------------------------------------------------------------------  */
				if(body_parts_index == 6) {
					the_board.error_str = "You died, you lose";
					the_board.prompt = "Would you like to play again (y)es/(n)o: ";
					retval = print_board_and_prompt_quit(&the_board);
					if(retval == EXIT_PROGRAM || retval == NO) {
						printf("Exiting...\n");
						exit(EXIT_SUCCESS);
					} else { /* user wants to play again after losing, wipe the board */
						clear_board(body_parts,man);
						break; /* breaks out of guessing loop, starting new game */
					} /* end of play again check */
				} /* end of game lost check */
				/* if user guessed wrong, but didn't lose yet --------------------------------------------------------- */
				sprintf(strcat_array,"Wrong guess: '%c'",c);
				the_board.error_str = strcat_array;
			} /* end of did_not_find_character */
			
		} /* end of guessing loop */

	} /* end of main loop */


	return 0;
}
