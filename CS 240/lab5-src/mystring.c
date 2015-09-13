#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Adds src at the end of dest. Return the value of dest.
char *mystrcat(char * dest, char * src) {
	char * p = dest;
	char * q = src;
	while (*p != '\0') {
		p++;
	}
	while (*q != '\0') {
		*p = *q;
		p++;
		q++;
	}
	*p = '\0';
	return dest;
}


// Return a pointer to the first occurrence of the substring needle in
// the string haystack. 
// Return NULL if the string is not found.
char * mystrstr(char * haystack, char * needle) {
	char * p = haystack;
	char * q = needle;
	char * sexy = needle;
	while (*p != '\0') {
		sexy = p;
		while (*p == *q) {
			if (*(q + 1) == '\0') {
				return sexy;
			}
			p++;
			q++;
		}
		p = sexy;
		p++;
		q = needle;
	}
	return NULL;
}


// Trims any spaces that are at the beginning or the end of s and returns s.
// Use the same memory passed in s. 
// Example:
// char s1[20];
// strcpy(s1,      hello    );
// strtrim(s1);
// printf(<s1=%s>\n, s1); // it will print <s1=hello>  without the spaces.
char * mystrtrim(char * s) {
	char * p = s;
	char * temp = s;
	while (*temp != '\0') {
		if (*temp != ' ') {
			*p = *temp;
			p++;
		}
		temp++;
	}
	*p = '\0';
	return s;
}


// Returns a new string that will substitute the first occurrence of the substring from
//  to the substring to in the string src
// This call will allocate memory for the new string and it will return the new string.
// Example:
// char a[6];
// strcpy(a, apple);
// char * b = strreplace(a, pp, orange);
// printf(<b=%s>\n, b); // It will print <b=aorangele>  

int mystrlen(char * tatti) {
	int i = 0;
	while (tatti != '\0') {
		i++;
		tatti++;
	}
	i = i - 1;
	return i;
}

char * mystrreplace(char * src, char * from, char * to)  {
	char * source = src;
	char * frm = from;
	char * tu = to;
	char * temp = mystrstr(src, from);
	char * tatti = (char *) malloc (10000 * sizeof(char));
	char * newvar = tatti;
	int length = strlen(from);
	char * srcone = src;
	while (mystrstr(temp, from) != NULL) {
		temp = mystrstr(temp, from);
		while (srcone != temp) {
			*tatti = *srcone;
			srcone++; tatti++;
		}
		tu = to;
		while (*tu != '\0') {
			*tatti = *tu;
			tatti++; tu++;
		}
		srcone += length;
		temp += length;
	}
	while (*temp) {
		*tatti = *temp;
		tatti++; temp++;
	}
	return newvar;
}
