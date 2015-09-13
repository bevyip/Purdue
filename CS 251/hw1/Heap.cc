#include <assert.h>
#include "Heap.h"

Heap::Heap(int maxSize) {
	this->maxSize = maxSize;
	heap = new heap[maxSize];
	int last = 0;
}

void Heap::insert(int key) {
	if (last >= maxSize) return;

	heap[last] = key;
	int child = last - 1;
	int parent = theParent(child);

	while (child > 0) {
		if (heap[parent] < heap[child]) {
			break;
		} else {
			int temp = heap[parent];
			heap[parent] = heap[child];
			heap[child] = temp;
		}
		child = parent;
		parent = theParent[child];
	}
}

int Heap::removeMin() {
	if (last == 0) return;
	//get the value to be removed in minKey
	int minKey = heap[0];
	//swap the last value with the first value
	heap[0] = heap[--last];
	//get the parent, left and the right value and store them in ints
	int parent = 0;
	int ileft = left(parent);
	int iright = right(parent);
	
	//start the loop and do ya thang
	while (ileft < last) {
		int minChild = ileft;
		
		if (iright < last && heap[iright] < heap[ileft]) {
			minChild = iright;
		}

		if (heap[parent] > heap[minChild]) {
			int temp = heap[parent];
			heap[parent] = heap[minchild];
			heap[minChild] = temp;
		} else {
			break;
		}

		//Change the values
		parent = minChild;
		ileft = left(parent);
		iright = right(parent);
	}
	return minKey;
}

Heap::~Heap() {
	delete []heap;
}
