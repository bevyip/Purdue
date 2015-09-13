
#include <stdio.h>
#include <stdlib.h>

#include "slist.h"

void sllist_init(SLList * list) {
	list->head = NULL;
}

// Add int value to the end of list. The values may be repeated.
void sllist_add_end( SLList *list, int value )  {
	SLEntry * e = list->head;
	SLEntry * entry = (SLEntry *) malloc (sizeof(SLEntry));
	if (list->head == NULL) {
		entry->value = value;
		entry->next = NULL;
		list->head = entry;
	} else {
		while (e->next != NULL) {
			e = e->next;
		}
		entry->value = value;
		entry->next = NULL;
		e->next = entry;
	}
}


// Remove first occurrence of value in the list. It returns 1 if value is found or 0 otherwise.
int sllist_remove(SLList *list, int value) {
	SLEntry * e = list->head;
	if (list->head->value == value) {
		list->head = list->head->next;
		return 1;
	}
	while (e->next->next != NULL) {
		if (e->next->value == value) {
			e->next = e->next->next;
			return 1;
		}
		e = e->next;
	}
	if (e->next->next == NULL) {
		if (e->next->value == value) {
			e->next = NULL;
			return 1;
		}
	}
	return 0;
}


// Removes from the list and frees the nodes of all the items that are min <= value <= max
void sllist_remove_interval(SLList *list, int min, int max) {
	SLEntry * e = list->head;
	while (e != NULL) {
		if (e->value >= min && e->value <= max) {
			sllist_remove(list, e->value);
		}
		e = e->next;
	}
}

void sllist_add(SLList * list, int value) {
	SLEntry * entry = (SLEntry *) malloc (sizeof(SLEntry));
	entry->value = value;
	entry->next = list->head;
	list->head = entry;
}
// It allocates and returns a new list with the intersection of the two lists, that is the 
// list of all the items that are common to the two lists. The items in the intersection are
// unique. 
SLList * sllist_intersection(SLList *a, SLList *b) {
	SLEntry * one = a->head;
	SLEntry * two = b->head;
	int flag = 0;
	SLList * new = (SLList *) malloc (sizeof(SLList));
	SLEntry * qwerty = new->head;
	while (one != NULL) {
		while (two != NULL) {
			if (one->value == two->value) {
				while (qwerty != NULL) {
					if (qwerty->value == one->value) {
						flag = 1;
					}
					qwerty = qwerty->next;
				}
				qwerty = new->head;
				if (flag != 1) {
				sllist_add(new, one->value);
				}
				flag = 0;
			}
			two = two->next;
		}
		two = b->head;
		one = one->next;
	}
	return new;
}

void sllist_print(SLList *list) {
	// Move to the end
	SLEntry * e = list->head;

	printf("--- List ---\n");
	while (e!= NULL) {
		printf("val=%d\n", e->value);
		e = e->next;
	}
}

