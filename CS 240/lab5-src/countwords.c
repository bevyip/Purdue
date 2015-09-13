

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct WordInfo {
	char * word;
	int count;
};

typedef struct WordInfo WordInfo;

int maxWords = 10;
int nWords = 0;
WordInfo*  wordArray;

#define MAXWORD 100
int wordLength;
char word[MAXWORD];
FILE * fd;
int charCount;
int wordPos;

void toLower(char *s) {
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

#define isaletter(ch) ((ch>='A'&&ch<='Z')||(ch>='a'&&ch<='z'))

// It returns the next word from stdin.
// If there are no more more words it returns NULL. 
static char * nextword() {
	int i = 0;
	char c;
	while ((c = fgetc(fd)) != EOF) {
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
	if (i > 0) {
		word[i] = '\0';
		charCount++;
		wordLength = strlen(word);
		return word;
	}
        return NULL;
}

int main(int argc, char **argv) {
	if (argc < 2) {
		printf("Usage: countwords filename\n");
		exit(1);
	}

	char * filename = argv[1];

	fd = fopen(filename, "r");

	if (fd == NULL) {
		return 0;
	}
	
	char * d = (char *) malloc (1000 * sizeof(char));
	int i = 0;
	int batamereko = 0;
	wordArray = (WordInfo *) malloc (maxWords * sizeof(WordInfo));
	while ((d = nextword()) != NULL) {
		toLower(d);
		//printf("%s\n",d);
		int j = 0;
		for (j = 0; j < nWords; j++) {
			if (strcmp(wordArray[j].word, word) == 0) {
				wordArray[j].count++;
				batamereko = 1;
				break;
			}
		}
		if (batamereko == 0) {
		  //      printf("1\n");
			//printf("d %s\n ",d);
			wordArray[nWords].word = strdup(d);
			//printf("2\n");
			wordArray[nWords].count++;
		//	printf(" d %s Wordarray %s\n",d,wordArray[i].word);
			nWords++;
		}
		batamereko = 0;
		if (nWords >= maxWords) {
			maxWords = maxWords * 2;
			wordArray = (WordInfo *) realloc (wordArray, maxWords * sizeof(WordInfo));
		}
		i++;
	}
	int k;
	int f, g;
	WordInfo w;
	for (f = 0; f < nWords - 1; f++) {
		for (g = f + 1; g < nWords; g++) {
	        	if (strcmp(wordArray[f].word, wordArray[g].word) > 0) {
	                	w = wordArray[f];
	                        wordArray[f] = wordArray[g];
	                        wordArray[g] = w;
			}
		}
        }

	for (k = 0; k < nWords; k++) {
		printf("%s %d\n", wordArray[k].word, wordArray[k].count);
	}
	
	return 0;
}

