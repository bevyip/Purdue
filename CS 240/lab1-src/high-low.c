#include <stdio.h>

int main(int argc, char **argv) {
  printf("Welcome to the High Low game...\n");

  // Write your implementation here...
  int high, low;
  char c;
  
  //high = 100;
  //low = 1;

  
  while (1) {
  high = 100;
  low = 1;
  printf("Think of a number between 1 and 100 and press press <enter>");
  getchar();
  while (high >= low) {
    
    int mid = (low + high)/2;
    printf("Is it higher than %i? (y/n)\n", mid);
    c = getchar();
    getchar();
    if (c == 'y') {
      low = mid + 1;
    } else if (c == 'n') {
      high = mid - 1;
    } else {
      printf("Type y or n\n");
    }
  }
  printf("\n>>>>>> The number is %d\n", low);
  printf("\nDo you want to continue playing (y/n)?");
  char d;
  d = getchar();
  getchar();
  if (d == 'n') {
    printf("Thanks for playing!!!");
    break;
  } else if (d == 'y') {
    continue;
  }
  }
}

