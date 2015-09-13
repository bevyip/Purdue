#include "HashTable.h"

HashTable::HashTable() {
	_maxSize = _tableSize;
	_currentEntry = 0;
	_buckets = (HashTableEntry **) malloc (_maxSize * sizeof(HashTableEntry *));
	for (int i = 0; i < _maxSize; i++) {
		_buckets[i] = NULL;
	}
}

int HashTable::hash(const char * key) {
	int h = 0;
	const char * p = key;
	while (*p) {
		h += *p;
		p++;
	}
	return h % _maxSize;
}

void HashTable::rehash() {
	int _oldSize = _maxSize;
	_maxSize = 2 * _maxSize;
	HashTableEntry ** _newBuckets = (HashTableEntry **) malloc (_maxSize * sizeof(HashTableEntry *));

	for (int i = 0; i < _maxSize; i++) {
		_newBuckets[i] = NULL;
	}

	for (int j = 0; j < _oldSize; j++) {
		HashTableEntry * e = _buckets[j];
		while (e != NULL) {
			HashTableEntry * next = e->next;
			int h = hash(e->key);
			e->next = _newBuckets[h];
			_newBuckets[h] = e;
			e = next;
		}
	}
	free(_buckets);
	_buckets = _newBuckets;
}

bool HashTable::insertItem(const char * key, char * data) {
	HashTableEntry * e;
	int h = hash(key);
	e = _buckets[h];
	
	if (_currentEntry == _maxSize) rehash();
	
	while (e != NULL) {
		if (!strcmp(e->_key, key)) {
			e->_data = strdup(data);
			return true;
		}
		e = e->_next;
	}

	e = new HashTableVoidEntry;
	e->_key = strdup(key);
	e->_data = strdup(data);
	e->_next = _buckets[h];
	_buckets[h] = e;
	_currentEntry++;
	return false;
}

bool HashTable::find(const char * key, char ** data) {
	HashTableEntry * e;
	int h = hash(key);
	e = _buckets[h];

	while (e != NULL) {
		if (!strcmp(e->_key, key)) {
			*data = strdup(e->_data);
			return true;
		}
		e = e->_next;
	}
	return false;
}

bool HashTable::removeElement() {
	HashTableEntry * e;
	HashTableEntry * prev = NULL;
	int h = hash(key);
	e = _buckets[h];
	while (e != NULL) {
		if (!strcmp(e->_key, key)) {
			if (prev == NULL) {
				_buckets[h] = e->_next;
			} else {
				prev->_next = e->_next;
			}
			_currentEntry--;
			delete e;
			return true;
		}
		prev = e;
		e = e->_next;
	}
	return false;
}
