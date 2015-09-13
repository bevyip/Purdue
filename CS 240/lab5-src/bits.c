#include<stdio.h>

// It prints the bits in bitmap as 0s and 1s.
void printBits(unsigned int bitmap) {
	int i = 0;
	// Add your code here
	for (i = 31; i >= 0; i--) {
		printf("%d", getBitAt(bitmap, i));
	}
	printf("\n");
	int j;
	for (j = 31; j >= 0; j--) {
		if (j >= 0 && j < 10) {
			printf("%d", j);
		} else if (j >= 10 && j < 20) {
			printf("%d", j - 10);
		} else if (j >= 20 && j < 30) {
			printf("%d", j - 20);
		} else if (j >= 30) {
			printf("%d", j - 30);
		}
	}
	printf("\n");
}


// Sets the ith bit in *bitmap with the value in bitValue (0 or 1)
void setBitAt( unsigned int *bitmap, int i, int bitValue ) {
	// Add your code here
	unsigned int mask = bitValue;
	if (bitValue == 1) {
		mask = mask << i;
		*bitmap |= mask;
	} else {
		mask = -2 << i;
		*bitmap &= mask;
	}
	return;
}

// It returns the bit value (0 or 1) at bit i
int getBitAt( unsigned int bitmap, unsigned int i) {
	// Add your code here
	unsigned int mask = 1;
	mask = mask << i;
	bitmap &= mask;
        unsigned int t = bitmap >> i;
	return t;
}

// It returns the number of bits with a value "bitValue".
// if bitValue is 0, it returns the number of 0s. If bitValue is 1, it returns the number of 1s.
int countBits( unsigned int bitmap, unsigned int bitValue) {
	// Add your code here
	int i = 0;
	int count = 0;
	for (i = 0; i < 32; i++) {
		if (getBitAt(bitmap, i) == bitValue) {
			count++;
		}
	}
	return count;
}

// It returns the number of largest consecutive 1s in "bitmap".
// Set "*position" to the beginning bit index of the sequence.
int maxContinuousOnes(unsigned int bitmap, int * position) {
	// Add your code here
	int i, count, pos;
	int max = 0;
	for (i = 0; i < 32; i++) {
		if (getBitAt(bitmap, i) == 1) {
			count++;
			if (count == 1) {
				pos = i;
			}
		} else {
			if (count > max) {
				max = count;
				*position = pos;
			}
			count = 0;
		}
	}
	return max;
}
