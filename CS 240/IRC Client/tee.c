#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(void) {
  char st[] ="Where there is will, there is a way.";
    char *ch;
      //clrscr();
        printf("Split \"%s\"\n", st);
	  ch = strtok(st, " ");
	    while (ch != NULL) {
	      printf("%s\n", ch);
	        ch = strtok(NULL, " ,");
		  }
		    //getch();
		      return 0;
		      }
