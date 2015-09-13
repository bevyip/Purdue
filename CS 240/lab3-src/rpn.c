#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <math.h>

#include "rpn.h"
#include "nextword.h"
#include "stack.h"

double rpn_eval(char * fileName, double x) {
	double a,b,c,d;
	FILE * fd;
	char * word;	
	
	fd = fopen(fileName, "r");

	while ((word = nextword(fd)) != NULL) {
		switch (word[0]) {
			case '+': b = stack_pop(); a = stack_pop(); stack_push(a + b); break;
			case '-': if (strlen(word) > 1) {d = atof(word); stack_push(d);} else {b = stack_pop(); a = stack_pop(); stack_push(a - b);} break;
			case '*': b = stack_pop(); a = stack_pop(); stack_push(a * b); break;
			case '/': b = stack_pop(); a = stack_pop(); stack_push(a / b); break;
			case 'l': a = stack_pop(); stack_push(log(a)); break;
			case 's': a = stack_pop(); stack_push(sin(a)); break;
			case 'c': a = stack_pop(); stack_push(cos(a)); break;
			case 'p': b = stack_pop(); a = stack_pop(); stack_push(pow(a, b)); break;
			case 'e': a = stack_pop(); stack_push(exp(a)); break;
			case 'x': stack_push(x); break;
			default: c = atof(word); stack_push(c);
		}
	}
	if (stack_top() <= 0) {
		printf("Stack underflow\n");
		exit(0);
	}
	if (stack_top() > 1) {
		printf("Elements remain in the stack\n");
		exit(0);
	}

	fclose(fd);
	
	double result = stack_pop();
	return result;
}
