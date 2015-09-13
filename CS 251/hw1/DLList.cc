#include "DLList.h"

DLList::DLList() {
	head = new DLNode();
	head->next = head;
	head->prev = head;
}

DLList::~DLList() {
	DLNode * e = head->next;

	while (e != NULL) {
		DLNode * next = e->next;
		delete e;
		e = next;
	}
}

void DLList::insertFront(int data) {
	DLNode * e = new DLNode();
	e->data = data;
	e->next = head->next;
	e->prev = head;
	head->next = e;
	e->next->prev = e;
}

void DLList::insertEnd(int data) {
	DLNode * e = new DLNode();
	e->data = data;
	e->next = head;
	e->prev = head->prev;
	head->prev = e;
	e->prev->next = e;
}

bool DLList::removeFront(int &data) {
	if (isEmpty()) return false;

	DLNode * e = head->next;
	data = e->data;
	head->next = e->next;
	e->next->prev = head;
	delete e;
	return true;
}

bool DLList::removeEnd(int & data) {
	if (isEmpty()) return false;

	DLNode * e = head->prev;
	head->prev = e->prev;
	e->prev->next = head;
	data = e->data;
	delete e;
	return true;
}

bool DLList::isEmpty() {
	return head->next == head && head->prev == head;
}

void DLList::print() {
	DLNode * e = head->next;
	while (e != head) {
		printf("%d\n", e->data);
		e = e->next;
	}
}

DLList::~DLList() {
	DLNode * e = head->next;
	while (e != head) {
		DLNode * next = e->next;
		delete e;
		e = next;
	}
	delete head;
}
