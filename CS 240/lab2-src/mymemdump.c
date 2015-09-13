#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void mymemdump(FILE * fd, char * p , int len) {
    int q;
    // Add your code here.
    // You may see p as an array.
    // p[0] will return the element 0 
    // p[1] will return the element 1 and so on
    for (int i = 0; i <= len/16; i++) {
    q = 16;
    if (len % 16 != 0 && i == len/16) {
    q = len%16;
    } else if (i == len/16) {
    break;
    }
    fprintf(fd, "0x%016lX: ", (unsigned long) p);
    
	for (int j = 0; j < q; j++) {
	int c = p[j]&0xFF;
    // Print first byte as hexadecimal
    fprintf(fd, "%02X ", c);
    }
    for (int l = 0; l < 16 - q; l++) {
    fprintf(fd, "   ");
    }
    fprintf(fd, " ");
	for (int k = 0; k < q; k++) {
    // Print first byte as character
int c = p[k]&0xFF;    
fprintf(fd, "%c", (c>=32&&c<=127)?c:'.');
}
    fprintf(fd,"\n");
p += 16;

}
}

