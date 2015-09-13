// Implementation of a dictionary using an array and sequential search
// The array will be dynamically resized if necessary

#include <stdlib.h>
#include <string.h>

#include "array-dictionary.h"

// constructor
ArrayDictionary::ArrayDictionary()
{
	maxNumber = 500;
	currentNumber = 0;
	
	array = new ArrayDictionaryNode[maxNumber];	
}

// add a record to the dictionary. Returns false if key already exists
bool ArrayDictionary::addRecord( KeyType key, DataType record) {
	for (int i = 0; i < currentNumber; i++)	{
		if (strcmp(array[i].key, key) == 0)	{
			array[i].data = (DataType*) record;
			return false;
		}
	}
	
	if (currentNumber == maxNumber) {
		int currentSize = maxNumber;
		maxNumber = maxNumber * 2;
		
		ArrayDictionaryNode *tmp = new ArrayDictionaryNode[maxNumber];

		for (int i = 0; i < currentSize; i++) {
			tmp[i].key = (char*)malloc(sizeof(char)*50);
			strcpy(tmp[i].key, array[i].key);
			tmp[i].data = new DataType;
			tmp[i].data = (DataType*)array[i].data;
		}
		
		delete [] array;
		
		array = tmp;
	}
	
	array[currentNumber].key = (char*)malloc(sizeof(char)*50);
	strcpy(array[currentNumber].key, key);
	array[currentNumber].data = new DataType;
	array[currentNumber].data = (DataType*)record;
	currentNumber++;
	
	return true;
}

// find a key in the dictionary and return corresponding record or NULL
DataType ArrayDictionary::findRecord(KeyType key) {
	for (int i = 0; i < currentNumber; i++) {
		if (strcmp(array[i].key, key) == 0) {
			return (DataType)array[i].data;
		}
	}
	
	return NULL;
}

// removes one element from the table
bool ArrayDictionary::removeElement(KeyType key) {
    int found;
	found = 0;
	
	int i;
	for (i = 0; i < currentNumber; i++) {
		if (strcmp(array[i].key, key) == 0) {
			found = 1;
		}
	}
	
	if (found == 0) {
		return false;
	}
    
    ArrayDictionaryNode *tmp = new ArrayDictionaryNode[maxNumber];
	
    int index;
    index = 0;
    
	for (i = 0; i < currentNumber; i++) {
        if (strcmp(array[i].key, key) == 0) {
			continue;
        }

	    tmp[index].key = (char*)malloc(sizeof(char)*50);
	    strcpy(tmp[index].key, array[i].key);
	    tmp[index].data = new DataType;
	    tmp[index].data = (DataType*)array[i].data;
        
        index++;
	}
	
	delete [] array;
	
	array = tmp;
    
    currentNumber--;
    
    return true;
}

// returns all the elements in the table as an array of strings.
// *n is the size of the table and it is returned by reference
KeyType * ArrayDictionary::keys(int * n) {
    KeyType* data = new KeyType[currentNumber];
	
	for (int i = 0; i < currentNumber; i++) {
		data[i] = (char*)malloc(sizeof(char)*20);
		strcpy((char*)data[i], array[i].key);
	}
	
	*n = currentNumber;
	
	return data;
}
