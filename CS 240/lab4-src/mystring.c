
#include <stdlib.h>
#include "mystring.h"

int mystrlen(char * s) {
	int i = 0;
	while (*s != '\0') {
		i++;s++;
	}
	return i;
}

char * mystrcpy(char * dest, char * src) {
	char * p = dest;
	while (*src != '\0') {
		*(p++) = *(src++);
	}
	*p = '\0';
	return dest;
}

char * mystrcat(char * dest, char * src) {
	char * p = dest;
	char * q = src;
	while (*p != '\0') {p++;}
	while (*q != '\0') {
		*p = *q;
		p++; q++;
	}
	*p = '\0';
	return dest;
}

int mystrcmp(char * s1, char * s2) {
	if (mystrlen(s1) > mystrlen(s2)) {
		return 1;
	} else if (mystrlen(s1) < mystrlen(s2)) {
		return -1;
	}
	return 0;
}

char * mystrstr(char * hay, char * needle) {
	int i, k, j;
	char * fhay;
	char * fneedle;
	for (i = 0; i < mystrlen(hay); i++) {
		fhay = hay;
		fneedle = needle;
		k = 0;
		if (*(fhay + i) == *fneedle) {
			for (j = 0; j < mystrlen(needle); j++) {
				if (*(fhay + i + j) == *(fneedle + j)) {
				k++;
				}
				if (k == mystrlen(needle)) {
					return hay + i;
				}
			}
		}
	}
	return NULL;
}

char * mystrdup(char * s) {
	char * d = malloc(sizeof(s));
	mystrcpy(d, s);
	return d;
}

char * mymemcpy(char * dest, char * src, int n) {
	char * a = dest;
	char * b = src;
	int i;
	for (i = 0; i < n; i++) {
		* (a++) = * (b++);
	}
	*a = '\0';
	return dest; 
}

