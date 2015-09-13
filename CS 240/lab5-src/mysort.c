#include "mysort.h"
#include <alloca.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

//
// Sort an array of element of any type
// it uses "compFunc" to sort the elements.
// The elements are sorted such as:
//
// if ascending != 0
//   compFunc( array[ i ], array[ i+1 ] ) <= 0
// else
//   compFunc( array[ i ], array[ i+1 ] ) >= 0
//
// See test_sort to see how to use mysort.
//
/*
int compInt(void * e1, void * e2) {
	int * p1 = (int *) e1;
	int * p2 = (int *) e2;
	if (*p1 > *p2) {
		return 1;
	} else if (*p2 > *p1) {
		return -1;
	} else {
		return 0;
	}
	return 0;
}

int compStr(void * e1, void * e2) {
	char * p1 = (char *) e1;
	char * p2 = (char *) e2;
	int test = strcmp(e1, e2);
	if (test == 0) {
		return 1;
	} else if (test > 0) {
		return -1;
	} else {
		return 0;
	}
	return 0;
}*/

void mysort( int n,                      // Number of elements
             int elementSize,            // Size of each element
             void * array,               // Pointer to an array
             int ascending,              // 0 -> descending; 1 -> ascending
             CompareFunction compFunc )  // Comparison function.
{ 
        void * temp = malloc(elementSize);
        int i,j;
        for(i = 0; i < (n-1); i++) {
                for(j = 0; j < (n-i-1); j++) {
                        void *e1 = (void*) ((char*)array+j*elementSize);
                        void *e2 = (void*) ((char*)array+(j+1)*elementSize);
                        if(ascending != 0) {
                                if((*compFunc)(e1,e2) >= 0) {
                                        memcpy(temp, e1, elementSize);
                                        memcpy(e1, e2, elementSize);
                                        memcpy(e2, temp, elementSize);
                                }
                        } else {
                                if((*compFunc)(e1,e2) <= 0) {  
                                        memcpy(temp, e1, elementSize);
                                        memcpy(e1, e2, elementSize);
                                        memcpy(e2, temp, elementSize);
                                }
                        }
                }
        }
        free(temp);
}

