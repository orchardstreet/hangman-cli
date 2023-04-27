#include "headers/config.h"
#include "headers/load_words.h"
#include <stdio.h>
#include <stdlib.h>

FILE * open_dict_file(void)
{
	FILE *dict_file;

	dict_file = fopen(DICT_FILE_LOCATION,"r");
	if(dict_file == NULL) {
		perror("");
		fprintf(stderr,"Cannot open dictionary file at %s. Please place a text file of english words"
						"separated by newlines in /dict and put the location of that file"
						"in /headers/config.h where it says 'DICT_FILE_LOCATION'\n",DICT_FILE_LOCATION);
		exit(1);
	}

	return dict_file;

}

int get_new_chunk_from_file(int *fread_chunk_index,char *fread_chunk,FILE * dict_file)
{
	int chunk_length;
		/* read file at FREAD_CHUNK_SIZE bytes at a time into fread_chunk */
	*fread_chunk_index = 0;
	chunk_length = fread(fread_chunk,1,FREAD_CHUNK_SIZE - 1,dict_file);
	if(ferror(dict_file)) {
		fprintf(stderr,"Couldnt read from dict file at %s\n",DICT_FILE_LOCATION);
		perror("");
		fclose(dict_file);
		exit(1);
	}
	return chunk_length;
}

unsigned char check_valid_sequence(int *fread_chunk_index,int *chunk_length,char *fread_chunk,FILE * dict_file)
{
	int valid_alphabetical_character_count = 0;

	/* count a-z or A-Z characters at beginning of potential valid sequence */
	while((fread_chunk[*fread_chunk_index] >= 'a' && fread_chunk[*fread_chunk_index] <= 'z') || (fread_chunk[*fread_chunk_index] >= 'A' && fread_chunk[*fread_chunk_index] <= 'Z')) {
		*fread_chunk_index = *fread_chunk_index + 1;
		if(*fread_chunk_index == *chunk_length) {
			if(feof(dict_file)) {
				return EOF_SEEN;
			}
			*chunk_length = get_new_chunk_from_file(fread_chunk_index,fread_chunk,dict_file);
			if(*chunk_length == 0) {
				return EOF_SEEN;
			}
		}
		valid_alphabetical_character_count++;
	}

	if(valid_alphabetical_character_count < MAX_CHARACTERS || valid_alphabetical_character_count < MIN_CHARACTERS) {
		return NO;
	}

	*fread_chunk_index = *fread_chunk_index + 1;
	if(*fread_chunk_index == *chunk_length) {
		if(feof(dict_file)) {
			return EOF_SEEN;
		}
		*chunk_length = get_new_chunk_from_file(fread_chunk_index,fread_chunk,dict_file);
		if(*chunk_length == 0) {
			return EOF_SEEN;
		}
	}
	if(fread_chunk[*fread_chunk_index] == '\n')
		return YES;

	if(fread_chunk[*fread_chunk_index] == '\r') {
		*fread_chunk_index = *fread_chunk_index + 1;
		if(*fread_chunk_index == *chunk_length) {
			if(feof(dict_file)) {
				return EOF_SEEN;
			}
			*chunk_length = get_new_chunk_from_file(fread_chunk_index,fread_chunk,dict_file);
			if(*chunk_length == 0) {
				return EOF_SEEN;
			}
		}
		if(fread_chunk[*fread_chunk_index] == '\n')
			return YES;
	}

}

int calculate_words_in_dict_file(FILE *dict_file,char *fread_chunk,char **valid_word_positions_in_file)
{
	int words_in_file = 0;
	unsigned char valid_sequence;
	int fread_chunk_index = 0;
	int fread_chunk_index_save;
	int chunk_length = 0;

	chunk_length = get_new_chunk_from_file(&fread_chunk_index,fread_chunk,dict_file);
	if(chunk_length == 0) {
		fprintf(stderr,"Error: Nothing to read in %s\n",DICT_FILE_LOCATION);
		exit(1);
	}

	/* iterate through newlines and find valid sequences, grab new chunk of file if needed */
	while(1) {
		fread_chunk_index_save = fread_chunk_index;
		valid_sequence = check_valid_sequence(&fread_chunk_index,&chunk_length,fread_chunk,dict_file);
		if(valid_sequence == YES) {
			valid_word_positions_in_file[words_in_file] = fread_chunk + fread_chunk_index_save;
			words_in_file++;
			if(words_in_file == MAX_WORDS_IN_FILE)
				return words_in_file;
			fread_chunk_index = fread_chunk_index + 1;
			if(fread_chunk_index == chunk_length) {
				if(feof(dict_file)) {
					break;
				}
				chunk_length = get_new_chunk_from_file(&fread_chunk_index,fread_chunk,dict_file);
				if(chunk_length == 0) {
					break;
				}
			}
		} else if (valid_sequence == NO) { /* needless comparison, here for clarity */
			fread_chunk_index = fread_chunk_index + 1;
			if(fread_chunk_index == chunk_length) {
				if(feof(dict_file)) {
					break;
				}
				chunk_length = get_new_chunk_from_file(&fread_chunk_index,fread_chunk,dict_file);
				if(chunk_length == 0) {
					break;
				}
			}
			continue;
		} else if (valid_sequence == EOF_SEEN) {
			break;
		}

	}

	if(words_in_file == 0) {
		fprintf(stderr,"No words found in file. Please place a text file of english words"
						"separated by newlines in /dict and put the location of that file"
						" in /headers/config.h where it says 'DICT_FILE_LOCATION'\n");
		exit(1);

	}
	return words_in_file;

}

	/* open file and check for errors
	 * use while loop to fread chunks from file
	 * with each chunk in memory, check for valid sequences, which consist of the following:
	 * (is beginning of file or after newline) (starts with a string of word characters
	 * which are more than 2 and less than MAX_CHARACTERS) (first non-word character is a carriage return
	 * or newline) (if fist non-word character is a carriage return, second non-word character is a newline)
	 * ^above in a function, if resolves to true, then increment words_in_file until MAX_WORDS_IN_FILE.
	 * we'll have a buffer of char *temp_words[TEMP_WORDS_COUNT] (around 200) words available in memory for games
	 * for this, we need a randomized array of valid word positions in the file, so the location of the first
	 * character of a potential valid sequence in a temporary variable, if the sequence evaluates as true, then
	 * save the position in char *valid_word_positions_in_file[MAX_WORDS_IN_FILE] consecutively
	 * next, iterate through aforementioned array between 0 and words_in_file: 2 * words_in_file times,
	 * with each iteration, pick two non-matching random numbers as indexes to swap, using a temporary
	 * char *temp_char_pointer to facilitate the swap
	 * now we have a randomized order of valid words
	 * have an index variable to keep track of these called valid_word_positions_in_file_index
	 * next we load TEMP_WORDS_COUNT words from the file, through a loop with i that goes from 0 to TEMP_WORDS_COUNT times, if valid_word_positions_in_file_index won't get overrun.  If it will, we represent
	 * that index to 0
	 * With each iteration, do, adjust fd to valid_word_positions_in_file[valid_word_positions_in_file_index], then run fgets(temp_words[i],MAX_CHARACTERS,fd), and increment valid_word_positions_in_file_index
	 *yay! we should now be able to run through the game loop TEMP_WORDS_COUNT times without accessing the hard
	 *drive, and having a random word available from memory.  After we iterate that many times through the game loop, we need to load TEMP_WORDS_COUNT more words into temp_words with aforementioned function
	 *the above makes it so we can accept even corrupted files as dicts, as long as the words are newline
	 *separated at some point.  Tell the user they can use a custom dict by setting DICT_FILE_LOCATION and
	 *MAX_WORDS_IN_FILE to their liking.  The user can also adjust TEMP_WORDS_COUNT.  Memory will be
	 *(MAX_WORDS_IN_FILE * sizeof(char *)) + (TEMP_WORDS_COUNT * MAX_CHARACTERS).  A typical game should then be
	 * take about 42k bytes of memory or (5000 * 8) + (200 * 10).  The above minimizes hard drive reads and
	 * memory.  To minimize memory more is possible by allowing for the possibility of repeat words before
	 * MAX_WORDS_IN_FILE has been exausted.  This could be achieved by not saving the index of each valid
	 * word in a file.  For each time a word is loaded into temp_words, you could pick a random n number between
	 * 0 and words_in_file, and go through the valid_word file n times to find the word.  This dramatically
	 * increases the load time of each seet of temp_words, and might only be useful for enormous dicts, or
	 * if we were running with less than 42k bytes additional bytes of memory (unlikely) */
