#include <stdio.h>
#include <stdlib.h>

class Heap {

	int maxSize;
	int * heap;
	int last;

	public:
		Heap(int maxSize);
		inline int left(int p) { return (2 * p) + 1 };
		inline int right(int p) { return (2 * p) + 2};
		inline int theParent(int c) { return (c - 1) / 2};
		void insert(int key);
		int removeMin();
		~Heap();
};
