
#include <stdio.h>
#include <stdlib.h>

#define MAXSTACK 50

int isbalanced (char * s) {
	char array[20];
	int i = 0;
	int counter = 0;
	char * temp = s;
	while (*temp != '\0') {
		if (*temp == '(' || *temp == '{' || *temp == '[') {
			array[i++] = *temp;
			counter++;
		} else if (i == 0) { 
		return 0;
		} else if (*temp == ')') {
			if (array[i - 1] == '(') {
				counter--;
				i--;
			} else {
				return 0;
			}
		} else if (*temp == '}') {
			if (array[i - 1] == '{') {
				counter--;
				i--;
			} else {
				return 0;
			}
		} else if (*temp == ']') {
			if (array[i - 1] == '[') {
				counter--;
				i--;
			} else {
				return 0;
			}
		}
		temp++;
	}
	if (counter == 0) {
		return 1;
	}
	return 0;
}

int main(int argc,  char **argv) {
	char * test = argv[1];
	int t = isbalanced(test);
	if (t == 0) {
		printf("string is not balanced\n");
	} else if (t == 1) {
		printf("string is balanced\n");
	}
	return 0;
}

