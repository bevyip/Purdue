#include <stdio.h>
#include <stdlib.h>
#include "array.h"
// Return sum of the array
double sumArray(int n, double * array) {
  double sum = 0;
  
  double * p = array;
  double * pend = p+n;

  while (p < pend) {
    sum += *p;
    p++;
  }

  return sum;
}

// Return maximum element of array
double maxArray(int n, double * array) {
	double * maxmin = array;
	double max = -345.00;
	double * pend = maxmin + n;
	while (maxmin < pend) {
		if (max < *maxmin) {
			max = *maxmin;
		}
		maxmin++;
	}
  return max;
}

// Return minimum element of array
double minArray(int n, double * array) {
	double * minmax = array;
	double min = 345.00;
	double * pend = minmax + n;
	while (minmax < pend) {
		if (min > *minmax) {
			min = *minmax;
		}
		minmax++;
	}
  return min;
}

// Find the position int he array of the first element x 
// such that min<=x<=max or -1 if no element was found
int findArray(int n, double * array, double min, double max) {
	double *p = array;
	double *pend = p + n;
	int i;
	for (i = 0; i < n; i++) {
		if (*(p + i) >= min && *(p + i) <= max) {
			return i;
		}
	}
  return -1;
}

// Sort array without using [] operator. Use pointers 
// Hint: Use a pointer to the current and another to the next element
int sortArray(int n, double * array) {

	int i, j;
	double * temp = array;
	double tmp;
	for (i = 0; i < n; i++) {
		for (j = i + 1; j < n; j++) {
	       		if ( * (temp + i) > * (temp + j)) {
		        	tmp = * (temp + i);
			        * (temp + i) = * (temp + j);
			        * (temp + j) = tmp;
			}
		}
	}
	return 0;
}

// Print array
void printArray(int n, double * array) {
	double * p =array;
	for (int i = 0; i < n; i++) {
		printf("%d:%f\n", i, * (p + i));
	}
}
