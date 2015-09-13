#include <stdlib.h>
#include "mystring.h"
    
// Type "man string" to see what every function expects.
    
    int mystrlen(char * s) {
    int i = 0;
    while (*s != '\0') {
        s++;
	i++;
    }
    return i;
}

char * mystrcpy(char * dest, char * src) {
    int i = 0;
    if (mystrlen(src) == 1) {
        return src;
    } else {
        while (src[i] != '\0') {
            dest[i] = src[i];
            i++;
        }
        dest[i] = '\0';
    }
    return dest;
}

char * mystrcat(char * dest, char * src) {
/*
	int i = 0;
    while (dest[i] != '\0') {
        i++;
    }
    int j = 0;
    while (src[j] != '\0') {
        dest[i] = src[j];
        j++;
    }
    return dest;
*/
//int l = mystrlen(dest) + dest;
//int k = mystrlen(src);
//int j = 0;
int i = 0;
int j = 0;
	while (dest[i] != '\0') {i++;}
		while (src[j] != '\0') {
			dest[i] = src[j];
			i++;
			j++;
		}
//	for (j = 0; src[j] != '\0'; j++) {
//		dest[i+j] = src[j];
//	}
	dest[i] = '\0';
	return dest;
}

int mystrcmp(char * s1, char * s2) {
/*
int i = 0;
    int k = 0;
    if (mystrlen(s1) == mystrlen(s2)) {
        while (s1[i] != '\0' && s2[i] != '\0') {
            if (s1[i] != s2[i]) {
                if (s1[i] > s2[i]) {
                    k--;
                } else {
                    k++;
                }
            }
            i++;
    } else if (mystrlen(s1) > mystrlen(s2)) {
        k = 1
    } else {
       k = -1;     
        }
*/
	int i = 0;
	if (mystrlen(s1) > mystrlen(s2))
		i = 1;
	else if (mystrlen(s1) < mystrlen(s2))
		i = -1;
	else
		i = 0;
	return i;
}

char * mystrstr(char * hay, char * needle) {
/*
    int k = 0;
    char * pointer;
    for (int i = 0; i < mystrlen(hay) - mystrlen(needle); i++) {
        for (int j = 0; j < mystrlen(needle); j++) {
            while (needle[j] == hay[i]) {
                k++;
                i++;
            }
            if (k == mystrlen(needle)) {
                pointer = 
                    break;
            } else
                continue;
        }
        if (k == mystrlen(needle))
            break;
    }
    if (k == mystrlen(needle)) {
        
    }
*/
int i,j;
int k;
for (i = 0; i < mystrlen(hay); i++) {
	if (hay[i] == needle[0] && k != mystrlen(needle)) {
		k = 0;
		for (j = 0; j < mystrlen(needle); j++) {
			if (hay[i+j] == needle[j]) {
				k++;
				if (k == mystrlen(needle)) {
					break;
				}
			}
		}
	}
	if (k == mystrlen(needle)) {
		return hay + i;
	}
}
    return NULL;
}

char * mystrdup(char * s) {
	char * d = malloc (sizeof(s));
	mystrcpy(d, s);
    return d;
}

char * mymemcpy(char * dest, char * src, int n) {
    int i;
    for (i = 0; i < n; i++) {
    	dest[i] = src[i];
    }
    dest[n] = '\0';
    return dest;
}
