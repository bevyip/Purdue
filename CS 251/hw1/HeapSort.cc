#include "HeapSort.h"

void HeapSort::heapSort(int a[], int n) {
	Heap heap(n);

	for (int i = 0; i < n; i++) {
		heap.insert(a[i]);
	}

	for (int j = 0; j < n; j++) {
		a[i] = heap.removeMin();
	}
}
