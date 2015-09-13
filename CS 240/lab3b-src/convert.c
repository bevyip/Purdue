#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

int main(int argc, char * argv[]) {
	
	if (argc != 4) {
		printf("Usage:  convert <basefrom> <baseto> <number>\n");
		exit(0);
	}
	
	int baseFrom = atoi(argv[1]);
	int baseTo = atoi(argv[2]);
	char * number = argv[3];
	int len = strlen(number);

	printf("Number read in base %d: %s\n", baseFrom, number);
	
	if (baseFrom < 2 || baseFrom > 25 || baseTo < 2 || baseTo > 25) {
		exit(0);
	}
	int a;
	int num[strlen(number)];
	int i;
	for (i = 0; i < len; i = i + 1) {
		switch (number[i]) {
			case 'A': num[i] = 10; break;
			case 'B': num[i] = 11; break;
			case 'C': num[i] = 12; break;
			case 'D': num[i] = 13; break;
			case 'E': num[i] = 14; break;
			case 'F': num[i] = 15; break;
			case 'G': num[i] = 16; break;
			case 'H': num[i] = 17; break;
			case 'I': num[i] = 18; break;
			case 'J': num[i] = 19; break;
			case 'K': num[i] = 20; break;
			case 'L': num[i] = 21; break;
			case 'M': num[i] = 22; break;
			case 'N': num[i] = 23; break;
			case 'O': num[i] = 24; break;
			case 'P': num[i] = 25; break;
			case '0': num[i] = 0; break;
			case '1': num[i] = 1; break;
			case '2': num[i] = 2; break;
			case '3': num[i] = 3; break;
			case '4': num[i] = 4; break;
			case '5': num[i] = 5; break;
			case '6': num[i] = 6; break;
			case '7': num[i] = 7; break;
			case '8': num[i] = 8; break;
			case '9': num[i] = 9; break;
		}
	}
	
	for (a = 0; a < len; a = a + 1) {
	                  if (num[a] >= baseFrom) {
	                           printf("Wrong digit in number.\n");
	                            exit(0);
	                     }
	              }

//Convert to base 10 first;
           int baseTen = 0;
           int j;
           int k = 0;
	   int length = strlen(number);
          
                   if (baseFrom != 10) {
		   int bitch;
		   int m;
		   for (j = length - 1; j >= 0; j = j - 1) {
				       
                                                bitch = 1;
                                                 for (m = 0; m < k; m++) {
                                                          bitch = bitch * baseFrom;
                                                   }       
						          
                                             baseTen = baseTen + num[j]*bitch;
                                             k++;
                                       }
			} else {
				baseTen = atoi(number); 
				       }

//Convert from base 10 to any base;
	int temp = baseTen;
	int tempRem;
	char y[33];
	int e;
	char x[27] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P'};
				        
	for (e = 0; temp != 0; e++) {
		tempRem = temp%baseTo;
		y[e] = x[tempRem];
		temp = temp/baseTo;
	}
	y[e] = '\0';

	//Reverse Array
	int forward = 0;
	int reverse = strlen(y);
	char final[reverse];
	while (y[forward] != '\0') {
		final[reverse - forward - 1] = y[forward];
		forward++;
	}
	final[reverse] = '\0';

	printf ("Converted to base 10: %d\n", baseTen);
	printf ("Converted to base %d: %s\n", baseTo, final);	
	
	return 0;
}
