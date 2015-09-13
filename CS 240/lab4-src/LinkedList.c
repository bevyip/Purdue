#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "LinkedList.h"

//
// Initialize a linked list
//
void llist_init(LinkedList * list)
{
	list->head = NULL;
}

//
// It prints the elements in the list in the form:
// 4, 6, 2, 3, 8,7
//
void llist_print(LinkedList * list) {
	
	ListNode * e;

	if (list->head == NULL) {
		printf("{EMPTY}\n");
		return;
	}

	printf("{");

	e = list->head;
	while (e != NULL) {
		printf("%d", e->value);
		e = e->next;
		if (e!=NULL) {
			printf(", ");
		}
	}
	printf("}\n");
}

//
// Appends a new node with this value at the beginning of the list
//
void llist_add(LinkedList * list, int value) {
	// Create new node
	ListNode * n = (ListNode *) malloc (sizeof(ListNode));
	n->value = value;
	
	// Add at the beginning of the list
	n->next = list->head;
	list->head = n;
}

//
// Returns true if the value exists in the list.
//
int llist_exists(LinkedList * list, int value) {
	ListNode * e;
	e = list->head;
	while(e != NULL) {
		if (e->value == value)
			return 1;
		e = e->next;
	}
	
	return 0;
}

// It removes the entry with that value in the list.
//
int llist_remove(LinkedList * list, int value) {
	ListNode * e;
	e = list->head->next;
	ListNode * f = list->head;
	while (e != NULL) {
		if (e->value == value) {
			f->next = e->next;
			return 1;
		}
		f = f->next;
		e = e->next;
	}
	return 0;
}

//
// It stores in *value the value that correspond to the ith entry.
// It returns 1 if success or 0 if there is no ith entry.
//
int llist_get_ith(LinkedList * list, int ith, int * value) {
	//wake me up when september ends
	ListNode * e = list->head;
	for (int i = 0; i < ith; i++) {
		if (e != NULL) {
			e = e->next;
		} else {
			//Go fuck yourself
			return 0;
		}
	}
	* value = e->value;
	return 1;
}

//
// It removes the ith entry from the list.
// It returns 1 if success or 0 if there is no ith entry.
//
int llist_remove_ith(LinkedList * list, int ith) {
	ListNode * e = list->head;
	        for (int i = 0; i < ith - 1; i++) {
	                if (e != NULL) {
				e = e->next;
			} else {
				//Go fuck yourself again
				return 0;
			}
		}
		e->next = e->next->next;
		return 1;
	}

//
// It returns the number of elements in the list.
//
int llist_number_elements(LinkedList * list) {
	int i = 0;
	ListNode * e  = list->head;
	while (e != NULL) {
		i++;
		e = e->next;
	}
	return i;
}


//
// It saves the list in a file called file_name. The format of the
// file is as follows:
//
// value1\n
// value2\n
// ...
//
int llist_save(LinkedList * list, char * file_name) {
	FILE * fd = fopen(file_name, "w");
	ListNode * e = list->head;
	if (fd == NULL) {
		return 0;
	}
	while (e != NULL) {
		fprintf(fd, "%d\n", e->value);
		e = e->next;
	}
	fclose(fd);
	return 0;
}

//
// It reads the list from the file_name indicated. If the list already has entries, 
// it will clear the entries.
//
int llist_read(LinkedList * list, char * file_name) {
	FILE * fd = fopen(file_name, "r");
	ListNode * e = list->head;
	if (fd == NULL) {
		return 0;
	}
	int c;
	while (fscanf(fd, "%d", &c) != EOF) {
		llist_add(list, c);
	}
	fclose(fd);
	return 1;
}


int sortArray(int n, int * array) {
	int i, j;
	int * temp = array;
	int tmp;
	for (i = 0; i < n; i++) {
		for (j = i + 1; j < n; j++) {
			if (*(temp + i) > * (temp + j)) {
				tmp = * (temp + i); 
				* (temp + i) = * (temp + j);
				* (temp + j) = tmp;
			}
		}
	}
	return 0;
}														                              
//
// It sorts the list. The parameter ascending determines if the
// order si ascending (1) or descending(0).
//
void llist_sort(LinkedList * list, int ascending) {
	ListNode * e = list->head;
	int number = llist_number_elements(list);
	int * p = (int *) malloc (number * sizeof(int));
	int * a = p;
	int * b = p;
	int i = 0;
	while (e != NULL) {
		*(a + i) = e->value;
		e = e->next;
		i++;
	}
	sortArray(number, b);
	int j = 0;
	ListNode * f = list->head;
	if (ascending == 1) {
		while (f != NULL) {
			f->value = * (p + j);
			f = f->next;
			j++;
		}
	} else if (ascending == 0) {
		j = number - 1;
		while (f != NULL) {
			f->value = * (p + j);
			f = f->next;
			j--;
		}
	}
}

//
// It removes the first entry in the list and puts value in *value.
// It also frees memory allocated for the node
//
int llist_remove_first(LinkedList * list, int * value) {
	ListNode * e = list->head;
	if (list->head == NULL) {
		return 0;
	}
	*value = e->value;
	list->head = list->head->next;
	free(e);
	return 1;
}

//
// It removes the last entry in the list and puts value in *value/
// It also frees memory allocated for node.
//
int llist_remove_last(LinkedList * list, int * value) {
	ListNode * e = list->head;
	if (list->head == NULL) {
		return 0;
	}
	while (e->next->next != NULL) {
		e = e->next;
	}
	*value = e->next->value;
	free(e->next);
	e->next = NULL;
	return 1;
}

//
// Insert a value at the beginning of the list.
// There is no check if the value exists. The entry is added
// at the beginning of the list.
//
void llist_insert_first(LinkedList * list, int value) {
	llist_add(list, value);
}

//
// Insert a value at the end of the list.
// There is no check if the name already exists. The entry is added
// at the end of the list.
//
void llist_insert_last(LinkedList * list, int value) {
	ListNode * n = (ListNode *) malloc (sizeof(ListNode));
	ListNode * e = list->head;
	if (list->head == NULL) {
		llist_add(list, value);
		
	} else {
		while (e->next != NULL) {
			e = e->next;
		}
		e->next = n;
		n->next = NULL;
		n->value = value;
	}
}

//
// Clear all elements in the list and free the nodes
//
void llist_clear(LinkedList *list) {
	list->head = NULL;	
}
