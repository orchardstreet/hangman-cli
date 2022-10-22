#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "headers/readline_custom.h"
#include "headers/board_display.h"
#define QUIT_PROMPT_RESULT_SIZE 6

char quit_prompt_input[QUIT_PROMPT_RESULT_SIZE];

signed char flush_stdin(void) {
	int c;
	for(;;) {
		c = getc(stdin);
		if(c == '\n') {
			return CONTINUE;
		} else if (c == EOF) {
			if(isatty(0))
				printf("\n");
			printf("detected EOF\n");
			clearerr(stdin);
			return EOF_SEEN;
		}
	}
}

signed char quit_prompt(struct board *the_board) {
	/* Asks user if he wants to quit or not
         * returns CONTINUE on "yes", EXIT_PROGRAM on "no" 
	 * Attempts to read from stdin to memory, until a valid input is given. * Asks user again on no valid input, no input, or on overflow * This program also flushes stdin on overflow. * This function deals with pecularities of stdin redirect input, in case
	 * someone is automating the program with stdin redirect.
	 * RETURN VALUES: uses the exit_codes enum, which is self-explanatory */
	
	char *newline_position;
	char *browse_input;
	signed char special_character_present = 0;
	signed char retval;
	
	for(;;) {
		
		print_board(the_board);
		
		memset(quit_prompt_input,0,QUIT_PROMPT_RESULT_SIZE);
		if (fgets(quit_prompt_input,QUIT_PROMPT_RESULT_SIZE - 1,stdin) == NULL) {
			if(ferror(stdin)) {
				perror("");
				fprintf(stderr,"\nfgets failure\n");
				return EXIT_PROGRAM; 
			} else if (feof(stdin)) {
				printf("\ndetected EOF\n");
				clearerr(stdin);
				return EXIT_PROGRAM;
			} else {
				fprintf(stderr,"\nunhandled error\n");
				return EXIT_PROGRAM; /* exit */
			}
		}
		if(!isatty(0)) {
			printf("\n");
		}
		
		/* CHECK FOR OVERFLOW, this should be the first check in this function that doesn't exit entire program (besides EOF check) */
		/* if what user enters reaches (input_size_temp - 3) without newline, count as overflow, flush stdin and ask again */
		newline_position = memchr(quit_prompt_input,'\n',QUIT_PROMPT_RESULT_SIZE - 2);
		if(!newline_position) {
			/* flush stdin */
			retval = flush_stdin();
			if (retval == CONTINUE) {
				the_board->error_str ="Cannot enter more than 3 characters, try again";
				continue;
			} else if (retval == EOF_SEEN) {
				return EXIT_PROGRAM;
			}
		}
		
		/* We didn't overflow stdin */

		/* If nothing entered, jump to top of function */
		if (newline_position == quit_prompt_input) {
			the_board->error_str = "Nothing entered, try again";
			continue;
		}

		/* Get rid of newline */
		*newline_position = 0;
		
		/* Check if fgets read a null character, in which case warn user of not entering special characters
		 * This null character check should always be here, so we don't process an empty string */
		/* Check for other special characters beyond first byte.  This check beyond the first byte can be modified  
		 * At this point in the function, a newline position exists, and it's not the first character */
		for(browse_input = quit_prompt_input;browse_input < newline_position ;browse_input++) {
			if(*browse_input < 'A' || (*browse_input < 'a' && *browse_input > 'Z') || *browse_input > 'z' ) {
				special_character_present = 1;
				break;
			}
		}
		
		if(special_character_present) {
			the_board->error_str ="Invalid input, please enter letters from a-z";
			special_character_present = 0;
			continue;
		} 

		if (!strcmp(quit_prompt_input,"no") || !strcmp(quit_prompt_input,"NO") || !strcmp(quit_prompt_input,"n") || !strcmp(quit_prompt_input,"N")) {
			return NO;
		} else if(!strcmp(quit_prompt_input,"yes") || !strcmp(quit_prompt_input,"YES") || !strcmp(quit_prompt_input,"Y") || !strcmp(quit_prompt_input,"y")) {
			return YES;
		} else {
			the_board->error_str = "Invalid input, please enter (y)es or (n)o";
			continue;
		}
		
	}
	
} 

signed char readline_custom(char *input, size_t input_size_temp, struct board *the_board,size_t word_length) {
	/* !! Don't pass a pointer to an array of size less than 4 !! */
	/* Asks user for input.
	 * Attempts to read from stdin to memory, until a valid input is given.
	 * Asks user again on no input, on overflow, or if detects a special
	 * character.
	 * This program also flushes stdin on overflow.
	 * This function deals with pecularities of stdin redirect input, in case
	 * someone is automating the program with stdin redirect.
	 * RETURN VALUES: uses the exit_codes enum, which is self-explanatory */
	
	char *newline_position;
	char *browse_input;
	signed char special_character_present = 0;
	char strcat_array[70];
	signed char retval;
	
	if (input == NULL) {
		fprintf(stderr,"passed NULL pointer to get_server_info_from_stdin() function\n");
		return EXIT_PROGRAM; /* exit */
	}
	
	if (input_size_temp < 4) {
		fprintf(stderr,"buffer provided not enough to read into fgets in function readline_custom\n");
		return EXIT_PROGRAM; /* exit */
	}
		
	for(;;) {
		
		if(word_length >= 255) {
			fprintf(stderr,"word passed into readline function is over 255, too long\n");
			return EXIT_PROGRAM;
		}
		sprintf(strcat_array,"Guess a letter you think is in the %zu letter word above: ",word_length);
		the_board->prompt = strcat_array;
		print_board(the_board);
		
		memset(input,0,input_size_temp);
		if (fgets(input,input_size_temp - 1,stdin) == NULL) {
			if(ferror(stdin)) {
				perror("");
				fprintf(stderr,"\nfgets failure\n");
				return EXIT_PROGRAM; 
			} else if (feof(stdin)) {
				printf("\ndetected EOF\n");
				clearerr(stdin);
				the_board->prompt = "Are you sure you would like to quit? (y)es/(n)o: ";
				retval = quit_prompt(the_board);
				if(retval == EXIT_PROGRAM || retval == YES)
					return EXIT_PROGRAM;
				continue;
			} else {
				fprintf(stderr,"\nunhandled error\n");
				return EXIT_PROGRAM; /* exit */
			}
		}
		if(!isatty(0)) {
			printf("\n");
		}
		
		/* CHECK FOR OVERFLOW, this should be the first check in this function that doesn't exit entire program (besides EOF check) */
		newline_position = memchr(input,'\n',input_size_temp - 2);
		if(!newline_position) {
			/* flush stdin */
			retval = flush_stdin();
			if (retval == CONTINUE) {
				the_board->error_str = "Cannot enter more than 1 character, try again";
				continue;
			} else if (retval == EOF_SEEN) {
				the_board->prompt = "Are you sure you would like to quit? (y)es/(n)o: ";
				retval = quit_prompt(the_board);
				if(retval == EXIT_PROGRAM || retval == YES)
					return EXIT_PROGRAM;
				continue;
			}
		}

		/* If nothing entered, jump to top of function */
		if (newline_position == input) {
			the_board->error_str = "Nothing entered, try again";
			continue;
		}

		/* Get rid of newline */
		*newline_position = 0;
		
		if(*input == 'q' && input[1] == 0) {
			the_board->prompt = "Are you sure you would like to quit? (y)es/(n)o: ";
			retval = quit_prompt(the_board);
			if(retval == EXIT_PROGRAM || retval == YES)
				return EXIT_PROGRAM;
			continue;
		}
		
		/* Check if fgets read a null character into first byte, in which case warn user of not entering special characters
		 * This null character check should always be here, so we don't process an empty string */
		/* Also, check for other special characters beyond the first byte.  Checking beyond first byte for null character
		 * can be modified.
		 * At this point in the function, a newline position exists, and it's not the first character */
		for(browse_input = input;browse_input < newline_position ;browse_input++) {
			if(*browse_input < 'a' || *browse_input > 'z') {
				special_character_present = 1;
				break;
			}
		}
		
		if(special_character_present) {
			the_board->error_str = "Invalid input, please enter a lowercase letter from 'a' to 'z'";
			special_character_present = 0;
			continue;
		} else {
			break; /* leave entire function as SUCCESS */
		}
	}
	
	return SUCCESS; /* success */
} 
