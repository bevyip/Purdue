#include "BinarySearchDictionary.h"
#include <assert.h>

BinarySearchDictionary::BinarySearchDictionary(int maxEntries) {
	this->maxEntries = maxEntries;
	n = 0;
	BinSearchItem * t = new BinSearchItem[maxEntries];
	sorted = true;
}

void BinarySearchDictionary::insert(char * key, void * item) {
	assert(n < maxEntries);
	
	table[n].key = strdup(key);
	table[n].item = item;
	n++;
	
	sorted = false;
}

void * BinarySearchDictionary::find(char * key) {
	if (!sorted) {
		heapSort();
		sorted = true;
	}

	int low = 0;
	int high = n - 1;

	while (high > low) {
		int mid = (low + high) / 2;
		int result = strcmp(key, table[mid].key);

		if (result == 0) {
			return table[mid].item;
		} else if (result < 0) {
			high = mid - 1;
		} else if (result > 0) {
			low = mid + 1;
		}
	}
	return NULL;
}
