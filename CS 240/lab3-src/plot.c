
#include <stdio.h>
#include <stdlib.h>

#include "rpn.h"

#define MAXCOLS 80
#define MAXROWS 40

char plot[MAXROWS][MAXCOLS];

void clearPlot()
{
  for (int i = 0; i < MAXROWS; i++) {
    for (int j = 0; j < MAXCOLS; j++) {
      plot[i][j] = ' ';
    }
  }
}

void printPlot()
{
  for (int i = 0; i < MAXROWS; i++) {
    for (int j = 0; j < MAXCOLS; j++) {
      printf("%c", plot[i][j]);
    }
    printf("\n");
  }
}

void plotXY(int x, int y, char c) {
  if ( x <0 || x>=MAXCOLS || y < 0 || y >=MAXROWS) return;
  plot[y][x]=c;
}

void createPlot( char * funcFile, double minX, double maxX) {
  int nvals = MAXCOLS;
  double yy[MAXCOLS];

  clearPlot();

  // Evaluate function and store in vector yy
	for (int i = 0; i < 80; i++) {
		yy[i] = rpn_eval(funcFile, minX + (i * ((maxX - minX)/80)));
	}
  //Compute maximum and minimum y in vector yy
  	double minY, maxY;
	for (int j = 0; j < 80; j++) {
		if (j == 0) {
			minY = maxY = yy[j];
		}
		if (minY > yy[j]) {
			minY = yy[j];
		} else if (maxY < yy[j]) {
			maxY = yy[j];
		}
	}
  //Plot x axis
	if (minY >= 0) {
		for (int k = 0; k < 80; k++) {
			plotXY(k, 39, '_');
		}
	} else {
		for (int l = 0; l < 80; l++) {
			plotXY(l, 19, '_');
		}
	}
  //Plot y axis
	for (int m = 0; m < 40; m++) {
		plotXY(40, m, '|');
	}
  // Plot function. Use scaling.
  	for (int n = 0; n < 80; n++) {
		plotXY(n, 39 - (int)((40*(yy[n] - minY))/(maxY - minY)), '*');
	}
  // minX is plotted at column 0 and maxX is plotted ar MAXCOLS-1
  // minY is plotted at row 0 and maxY is plotted at MAXROWS-1

  printPlot();

}

int main(int argc, char ** argv)
{
  printf("RPN Plotter.\n");
  
  if (argc < 4) {
    printf("Usage: plot func-file xmin xmax\n");
    exit(1);
  }
  
  // Get arguments
  
  createPlot(argv[1], atof(argv[2]), atof(argv[3]));
}
