struct BinSearchItem {
	char * key;
	void * item;
};

class BinarySearchDictionary {
	int maxEntries;
	int n;
	BinSearchItem * table;
	bool sorted;
	
	public:
		BinarySearchDictionary(int maxEntries);
		void insert(char * key, void * item);
		void * find(char * key);
};
