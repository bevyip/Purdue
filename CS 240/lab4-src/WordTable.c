#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "WordTable.h"

// Initializes a word table
void wtable_init(WordTable * wtable)
{
	// Allocate and initialize space for the table
	wtable->nWords = 0;
	wtable->maxWords = 10;
	wtable->wordArray = (WordInfo *) malloc(wtable->maxWords * sizeof(WordInfo));
	for (int i = 0; i < wtable->maxWords; i++) {
		llist_init(&wtable->wordArray[i].positions);
	}
}

// Add word to the tableand position. Position is added to the corresponding linked list.
void wtable_add(WordTable * wtable, char * word, int position)
{
	// Find first word if it exists
	for (int i = 0; i < wtable->nWords; i++) {
		if ( strcmp(wtable->wordArray[i].word, word)== 0 ) {
			// Found word. Add position in the list of positions
			llist_insert_last(&wtable->wordArray[i].positions, position);
			return;
		}
	}

	// Word not found.
	if (wtable->nWords >= wtable->maxWords) {
		wtable->maxWords = 2*(wtable->maxWords);
		wtable->wordArray = (WordInfo *) realloc (wtable->wordArray, (wtable->maxWords) * sizeof(WordInfo));
	}
	// Make sure that the array has space.
	// Expand the wordArray here.

	// Add new word and position
	wtable->wordArray[wtable->nWords].word = strdup(word);
	llist_insert_last(&wtable->wordArray[wtable->nWords].positions, position);
	wtable->nWords++;
}

// Print contents of the table.
void wtable_print(WordTable * wtable, FILE * fd)
{
	fprintf(fd, "------- WORD TABLE -------\n");

	// Print words
	for (int i = 0; i < wtable->nWords; i++) {
		fprintf(fd, "%d: %s: ", i, wtable->wordArray[i].word);
		llist_print( &wtable->wordArray[i].positions);
	}
}

// Get positions where the word occurs
LinkedList * wtable_getPositions(WordTable * wtable, char * word)
{
	// Write your code here
	for (int i = 0; i < wtable->nWords; i++) {
		if (strcmp(wtable->wordArray[i].word, word) == 0) {
			return &wtable->wordArray[i].positions;
		}
	}
	return NULL;
}

//
// Separates the string into words
//

#define MAXWORD 200
char word[MAXWORD];
int wordLength;
int wordCount;
int charCount;
int wordPos;

#define isaletter(ch) ((ch>='A'&&ch<='Z')||(ch>='a'&&ch<='z'))



// It returns the next word from stdin.
// If there are no more more words it returns NULL.
// A word is a sequence of alphabetical characters.
static char * nextword(FILE * fd) {
	// Write your code here
	char c;
	int i = 0;
	while ((c = getc(fd)) != EOF) {
		if (isaletter(c)) {
			word[i] = c;
			i++;
			charCount++;
		} else if (!(isaletter(c))) {
			word[i] = '\0';
			charCount++;
			if (strlen(word) > 0) {
				wordLength = strlen(word);
				i = 0;
				return word;
			}
			i = 0;
		}
	}
        return NULL;
}

// Conver string to lower case
void toLower(char *s) {
	// Write your code here
	// to convert one char to lower case. You have to do it for all chars in string.
	int i;
	int length = strlen(s);
	char ch;
	for (i = 0; i < length; i++) {
		ch = s[i];
		if (ch>='A'&&ch<='Z') {
			ch = (ch-'A')+'a';
			s[i]=ch;
		}
	}
}


// Read a file and obtain words and positions of the words and save them in table.
int wtable_createFromFile(WordTable * wtable, char * fileName, int verbose)
{
	// Write your code here
	FILE * fd = fopen(fileName, "r");
	if (fd == NULL) {
		return 0;
	}
	char * f = nextword(fd);
	int final;
	while (f != NULL) {
		toLower(f);
		final = charCount - strlen(f) - 1;
		wtable_add(wtable,f,final);
		if(verbose == 1) printf("%d: word=%s, pos=%d\n", wordCount, f, final);
		wordCount++;
		f = nextword(fd);
	}
	fclose(fd);
	return 0;
}

// Sort table in alphabetical order.
void wtable_sort(WordTable * wtable) {
	// Write your code here
	WordInfo w;
	for (int i = 0; i < wordCount-1; i++) {
		for (int j = i + 1; j < 526; j++) {
			if (strcmp(wtable->wordArray[i].word, wtable->wordArray[j].word) > 0) {
				w = wtable->wordArray[i];
				wtable->wordArray[i] = wtable->wordArray[j];
				wtable->wordArray[j] = w;
			}
		}
	}
}

// Print all segments of text in fileName that contain word.
// at most 200 character. Use fseek to position file pointer.
// Type "man fseek" for more info. 
int wtable_textSegments(WordTable * wtable, char * word, char * fileName)
{
	// Write your code here
	FILE * fd = fopen(fileName, "r");
	if (fd == NULL) {
		return 0;
	}
	LinkedList * counter = wtable_getPositions(wtable, word); 
	printf("===== Segments for word \"%s\" in book \"%s\" =====\n", word, fileName);
	ListNode *c = counter->head;
	while (c != NULL) {
		printf("---------- pos=%d-----\n", c->value);
		fseek(fd,c->value,SEEK_SET);
		printf("......");
		for (int j = 0; j < 200; j++) printf("%c", fgetc(fd));
		printf("......\n");
		c = c->next;

	}
	return 0;
}
