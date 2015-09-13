#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct HashTableEntry {
	const char * _key;
	char * _data;
	HashTableEntry * _next;
} HashTableEntry;

class HashTable {

	public:
		enum {_tableSize = 2039};
		HashTableEntry ** _buckets;
		int _maxSize;
		int _currentEntry;

		HashTable();
		int hash(const char * key);
		void rehash();
		bool insertItem(const char * key, char * data);
		bool find(const char * key, char ** data);
		bool removeElement(const char * key);
};
