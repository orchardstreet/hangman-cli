#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "headers/readline_custom.h"
#define MAX_CHARACTERS 10 

void print_man(char man_copy[][8]) {

	int p;
	for(p=0;p<7;p++) {
		printf("      %s\n",man_copy[p]);
	}
	printf("\n");

}

void print_letter_hints(char *letter_hints,char letter_hints_present) {

	if(letter_hints_present) {
		printf("      %s\n",letter_hints);
	} else {
		printf("\n");
	}

}

void print_dashes(char *dashes_copy) {

	printf("      %s\n\n",dashes_copy);

}

int main(void) {

	char *word = "jambalayaa";
	int i;
	int j;
	char c;
	char x;
	char y;
	char did_find_character = 0;
	char characters_found = 0;
	char letters_found[MAX_CHARACTERS + 1] = {0};
	char letter_guess[4];
	char to_play_again;
	char play_again[6];
	char dashes[(MAX_CHARACTERS * 2) + 1];
	char letter_hints[(MAX_CHARACTERS * 2) + 1];
	char *found_char;
	char letter_hints_present;
	size_t word_length;
	char man[7][8] = {
		" _____ ",
		" |   | ",
		" |     ",
		" |     ",
		" |     ",
		" |     ",
	        " ------"
	};
	struct body_part {
		char y;
		char x;
		char part;
	};
	struct body_part body_parts[6] = {
		{2,5,'O'}, /* head */
		{3,4,'/'}, /* left arm */
		{3,5,'|'}, /* torso */
		{3,6,'\\'},/* right arm */
		{4,4,'/'}, /* left leg */
		{4,6,'\\'} /* right leg */
	};
	int body_parts_index;

	/* Prevent stdout and stderr from appearing out of order on CLI
	 * by setting both to unbuffered mode, normally stdout is
	 * full buffered and stderr is unbuffered, which causes the
	 * aforementioned issue. */
	setvbuf(stdout,NULL,_IONBF,0);
	setvbuf(stderr,NULL,_IONBF,0);

	letter_guess[3] = 0;
	play_again[5] = 0;

	/* main loop */
	for(;;) {
		
		/* set full_body_random_index */
		body_parts_index = 0;

		/* generate dashes accordin to word length */
		word_length = strlen(word);
		if(word_length > MAX_CHARACTERS) {
			fprintf(stderr,"Error: word rendered from dictionary exceeds max characters allowed\n");
			exit(EXIT_FAILURE);
		} else if (word_length < 1) {
			fprintf(stderr,"Error: word rendered from dictionary contains no characters\n");
			exit(EXIT_FAILURE);
		}
		memset(dashes,' ',word_length * 2);
		dashes[word_length * 2] = 0;
		for(i = 0;i<word_length;i++){ 
			dashes[i*2] = '-';
		}
		memset(letter_hints,' ',word_length * 2);
		letter_hints[word_length * 2] = 0;
		letter_hints_present = 0;
		to_play_again = 0;
		characters_found = 0;
		memset(letters_found,0,MAX_CHARACTERS + 1);

		/* guessing loop */
		for(;;) {
			found_char = word;
			print_man(man);
			print_letter_hints(letter_hints,letter_hints_present);
			print_dashes(dashes);
			did_find_character = 0;
			for(;;) {
				if(readline_custom("Guess a letter from 'a' to 'z': ",letter_guess,4) == EXIT_PROGRAM) {
					printf("Exiting...\n");
					exit(EXIT_SUCCESS);
				}
				c = letter_guess[0];
				if(c < 97 || c > 122) {
					printf("Invalid input, please enter a lowercase letter from 'a' to 'z'\n");
					continue;
				}
				break;
			}
			if(strchr(letters_found,c)) {
				printf("Already guessed that letter\n");
				continue;
			}
			printf("You guessed: %s\n\n\n\n\n",letter_guess);
			while(found_char) {
				found_char = strchr(found_char,c);
				if(!found_char)
					break;
				/* found character */
				characters_found++;
				letters_found[characters_found - 1] = c;
				did_find_character = 1;
				letter_hints[(found_char - word) * 2] = c;
				letter_hints_present = 1;
				found_char++;
			}
			if(characters_found == word_length) {
				print_man(man);
				print_letter_hints(letter_hints,letter_hints_present);
				print_dashes(dashes);
				printf("You won!\n");
				for(;;) {
					if(readline_custom("Would you like to play again (y)es/(n)o: ",play_again,6) == EXIT_PROGRAM) {
						printf("Exiting...\n");
						exit(EXIT_SUCCESS);
					}
					if(!strcmp(play_again,"yes") || !strcmp(play_again,"YES") || !strcmp(play_again,"Y") || !strcmp(play_again,"y")) {
						to_play_again = 1;
						for(j=0;j<6;j++){
							x = body_parts[j].x;
							y = body_parts[j].y;
							man[y][x] = ' ';
						}
						break;
					} else if (!strcmp(play_again,"no") || !strcmp(play_again,"NO") || !strcmp(play_again,"n") || !strcmp(play_again,"N")) {
						printf("Exiting...\n");
						exit(EXIT_SUCCESS);
					} else {
						printf("Not a valid entry, please enter 'yes' or 'no'\n");
						continue;
					}
				}
				if(to_play_again)
					break;
			}
			if(did_find_character) {
				printf("characters found: %d\n",(int)characters_found);
			}
			if(!did_find_character) {
				x = body_parts[body_parts_index].x;
				y = body_parts[body_parts_index].y;
				c = body_parts[body_parts_index].part;
				man[y][x] = c;
				body_parts_index++;
			}
			if(body_parts_index == 6) {
				print_man(man);
				printf("You died, you lose\n");
				for(;;) {
					if(readline_custom("Would you like to play again (y)es/(n)o: ",play_again,6) == EXIT_PROGRAM) {
						printf("Exiting...\n");
						exit(EXIT_SUCCESS);
					}
					if(!strcmp(play_again,"yes") || !strcmp(play_again,"YES") || !strcmp(play_again,"Y") || !strcmp(play_again,"y")) {
						to_play_again = 1;
						for(j=0;j<6;j++){
							x = body_parts[j].x;
							y = body_parts[j].y;
							man[y][x] = ' ';
						}
						break;
					} else if (!strcmp(play_again,"no") || !strcmp(play_again,"NO") || !strcmp(play_again,"n") || !strcmp(play_again,"N")) {
						printf("Exiting...\n");
						exit(EXIT_SUCCESS);
					} else {
						printf("Not a valid entry, please enter 'yes' or 'no'\n");
						continue;
					}
				}

			}
			if(to_play_again)
				break;

		}

	} /* end of main loop */


	printf("Exiting...\n");
	return 0;
}
