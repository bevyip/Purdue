#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct DLNode {
	int data;
	struct DLNode *next;
	struct DLNode *prev;
} DLNode;

class DLList {
	DLList *head;

	public:
		DLList();
		~DLList();

		void insertFront(int data);
		void insertEnd(int data);
		bool removeFront(int &data);
		bool removeEnd(int &data);
		bool isEmpty();
		void print();
};
