#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "headers/readline_custom.h"
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

signed char quit_prompt(char *prompt) {
	/* Asks user if he wants to quit or not
         * returns CONTINUE on "yes", EXIT_PROGRAM on "no" 
	 * Attempts to read from stdin to memory, until a valid input is given.
	 * Asks user again on no valid input, no input, or on overflow
	 * This program also flushes stdin on overflow.
	 * This function deals with pecularities of stdin redirect input, in case
	 * someone is automating the program with stdin redirect.
	 * RETURN VALUES: uses the exit_codes enum, which is self-explanatory */
	
	char *newline_position;
	char *browse_input;
	signed char special_character_present = 0;
	signed char retval;
	
	for(;;) {
		
		printf("%s",prompt);
		
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
				printf("Cannot enter more than %d characters, try again\n",QUIT_PROMPT_RESULT_SIZE - 3);
				continue;
			} else if (retval == EOF_SEEN) {
				return EXIT_PROGRAM;
			}
		}
		
		/* We didn't overflow stdin */

		/* If nothing entered, jump to top of function */
		if (newline_position == quit_prompt_input) {
			fprintf(stderr,"Nothing entered, try again\n");
			continue;
		}

		/* Get rid of newline */
		*newline_position = 0;
		
		/* Check if fgets read a null character, in which case warn user of not entering special characters
		 * This null character check should always be here, so we don't process an empty string */
		if(*quit_prompt_input == 0) {
			fprintf(stderr,"no special characters in input please\n");
			continue;
		}
		
				/* Check for other special characters.  This can be modified or removed.
		 * At this point in the function, a newline position exists, and it's not the first character */
		for(browse_input = quit_prompt_input + 1;browse_input < newline_position ;browse_input++) {
			if(*browse_input < ' ' || *browse_input > '~') {
				special_character_present = 1;
				break;
			}
		}
		
		if(special_character_present) {
			fprintf(stderr,"no special characters in input please\n");
			special_character_present = 0;
			continue;
		} else {
			break; /* leave entire function as SUCCESS */
		}

		if (!strcmp(quit_prompt_input,"no") || !strcmp(quit_prompt_input,"NO") || !strcmp(quit_prompt_input,"n") || !strcmp(quit_prompt_input,"N")) {
			return EXIT_PROGRAM;
		else if(!strcmp(quit_prompt_input,"yes") || !strcmp(quit_prompt_input,"YES") || !strcmp(quit_prompt_input,"Y") || !strcmp(quit_prompt_input,"y")) {
			return CONTINUE;
		} else {
			printf("Invalid input\n");
			continue;
		}
		
	}
	
	return SUCCESS; /* success */
} 

signed char readline_custom(char *prompt, char *input, size_t input_size_temp) {
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
		
		printf("%s",prompt);
		
		memset(input,0,input_size_temp);
		if (fgets(input,input_size_temp - 1,stdin) == NULL) {
			if(ferror(stdin)) {
				perror("");
				fprintf(stderr,"\nfgets failure\n");
				return EXIT_PROGRAM; 
			} else if (feof(stdin)) {
				printf("\ndetected EOF\n");
				clearerr(stdin);
				if(quit_prompt() == EXIT_PROGRAM)
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
				printf("Cannot enter more than %lu characters, try again\n",input_size_temp - 3);
				continue;
			} else if (retval == EOF_SEEN) {
				if(quit_prompt() == EXIT_PROGRAM)
					return EXIT_PROGRAM;
				continue;
			}
		}

		/* If nothing entered, jump to top of function */
		if (newline_position == input) {
			fprintf(stderr,"Nothing entered, try again\n");
			continue;
		}

		/* Get rid of newline */
		*newline_position = 0;
		
		if(*input == 'q' && input[1] == 0) {
			if(quit_prompt() == EXIT_PROGRAM)
				return EXIT_PROGRAM;
			continue;
		}
		
		/* Check if fgets read a null character, in which case warn user of not entering special characters
		 * This null character check should always be here, so we don't process an empty string */
		if(*input == 0) {
			fprintf(stderr,"first character entered is null character, no special characters in input please\n");
			continue;
		}

		/* Check for other special characters.  This can be modified or removed.
		 * At this point in the function, a newline position exists, and it's not the first character */
		for(browse_input = input + 1;browse_input < newline_position ;browse_input++) {
			if(*browse_input < ' ' || *browse_input > '~') {
				special_character_present = 1;
				break;
			}
		}
		
		if(special_character_present) {
			fprintf(stderr,"no special characters in input please\n");
			special_character_present = 0;
			continue;
		} else {
			break; /* leave entire function as SUCCESS */
		}
	}
	
	return SUCCESS; /* success */
} 
