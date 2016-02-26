#include "webcrawler.h"

// Add your implementation here

int urlRecNum = 0;
char *word;
//char *description;
int inserted;
URLRecord * _urlArray;
char * bss;
int flagone = 0;
int flagtwo = 0;
int mi = 0;
//char * ptr;
char * buffert = (char *) malloc (10000 * sizeof(char));
char * ptr = buffert;

//call of duty modern warfare mac

void WebCrawler::onContentFound(char c) {
	//TITLE Tag title extraction
	if (c == '[') {
		*ptr = '\0';
		ptr = buffert;
		//printf("Title: %s\n", ptr);

		if (_urlArray[_headURL]._description == NULL) {
			//printf("Atleast it comes here! %d\n", _headURL);
			_urlArray[_headURL]._description = strdup(ptr);
			//printf("%s YOLOY\n", _urlArray[_headURL]._description);
		} else {
			strcat(_urlArray[_headURL]._description, " ");
			strcat(_urlArray[_headURL]._description, ptr);
			//printf("%s YOLOY\n", _urlArray[_headURL]._description);
		}
		
	} else if (c == ']') {
		memset(buffert,0,strlen(buffert));
		ptr = buffert;
	} else if (c == '"'){
		//Do nothing
	} else {
		*ptr = c;
		ptr++;
	}
}

void WebCrawler::onAnchorFound(char * url) {
	if (_tailURL >= _maxUrls) {
		return;
	}
	
	const char * http = "http://";
	const char * https = "https://";
	const char * yol = "//";
	const char * hashtag = "#";
	bss = new char[200];

	if (strncmp(http, url, 7) == 0) {
		for (int i = 0; i < _tailURL; i++) {
			if (strcmp(url, _urlArray[i]._url) == 0) {
				flagone = 1;
				break;
			}
		}
		if (flagone == 1) {
			flagone = 0;
			return;
		} else {
			_urlArray[_tailURL]._url = strdup(url);
			_tailURL++;
			//printf("\n\n%s : Yolo Updated URL\n\n", _urlArray[_tailURL - 1]._url);
		}
	} else if (strncmp(url, yol, 2) == 0) {
	} else {
		if (strncmp(url, https, 8) && strncmp(url, hashtag, 1)) {
			bss = strdup(_urlArray[_headURL]._url);
			if (strncmp(url, "/", 1)) {
				bss = strcat(bss, "/");
			}
			bss = strcat(bss, url);
			_urlArray[_tailURL]._url = strdup(bss);
			_urlArray[_tailURL]._description = NULL;
			_tailURL++;
			//printf("\n\n%s : Yolo Updated URL\n\n", _urlArray[_tailURL - 1]._url);
		}
	}
}

char * yod = (char *) malloc (sizeof(char) * 1000);
char * WebCrawler::nextWord(char * &c) {
	//memset(yod, 0, strlen(yod));
	int i = 0;
	//if (c == NULL) {
	//	return NULL;
	//}
	while (*c != '\0') {
		//printf("I'm in!\n");
		if (*c == ' ' || *c == '.' || *c == ',' || *c == '-') {
			//printf("I'm out!\n");
			if (i == 0) {
				c++;
				continue;
			} else {
				yod[i] = '\0';
				return yod;
			}
		} else {
			//printf("I'm in!\n");
			yod[i++] = *c;
			//printf("Am I in?\n");
			c++;
		}
	}
	if (i > 0) {
		yod[i] = '\0';
		return yod;
	}
	return NULL;
}



WebCrawler::WebCrawler(int maxUrls, int nInitialUrls, const char **initialUrls) {
	_maxUrls = maxUrls;
	_urlArray = new URLRecord[nInitialUrls + maxUrls];
	_headURL = 0;
	_tailURL = nInitialUrls;
	char * word = (char *) malloc (100 * sizeof(char));
	for (int i = 0; i < nInitialUrls; i++) {
		_urlArray[i]._url = strdup(initialUrls[i]);
		_urlArray[i]._description = (char *) malloc (1000);
		_urlArray[i]._description = NULL;
	}

	for (int i = 0; i < nInitialUrls + maxUrls; i++) {
		_urlArray[i]._description = (char *) malloc (1000);
	}
	_urlToUrlRecord = new HashTableTemplate<int>;
	_wordToURLRecordList = new HashTableTemplate<URLRecordList*>();
	//_wordRecordIterator  = new HashTableTemplateIterator<URLRecordList *>(_wordToURLRecordList);
}

void WebCrawler::writeURLFile(const char * urlFileName) {
	FILE * file = fopen(urlFileName, "w");
	for (int i = 0; i < _tailURL; i++) {
		fprintf(file, "%d %s\n", i, _urlArray[i]._url);
		//if (_urlArray[i]._description != NULL) {
			printf("YH\n");
			printf("%s\n", _urlArray[i]._description);
			fprintf(file, "%s\n", _urlArray[i]._description);
		///} else {
		//	fprintf(file, "%s\n", "");
		//}
		fprintf(file, "\n");
	}
	fclose(file);
}

/*void
WebCrawler::writeWordFile(const char *wordFileName)
{
    FILE *file;
    file = fopen(wordFileName, "w");
    
    int *printed;
    
    int i;
    for (i = 0; i < 2039; i++)
    {
        if (_wordToURLRecordList->_buckets[i] == NULL)
            continue;
        
        fprintf(file, "%s", _wordToURLRecordList->_buckets[i]->_key);
        
        URLRecordList *curr = _wordToURLRecordList->_buckets[i]->_data;
        
        printed = new int[2039];
        
        int j;
        for (j = 0; j < 2039; j++)
        {
            printed[j] = -1;
        }
        
        while (curr != NULL)
        {
            if (curr->_urlRecordIndex < 0 ||
                curr->_urlRecordIndex > 20390)
            {
                break;
            }
            
            int found = 0;
            for (j = 0; j < 2039; j++)
            {
                if (printed[j] < 0) break;
                
                if (printed[j] == curr->_urlRecordIndex)
                {
                    found = 1;
                    break;
                }
            }
            
            if (found == 0)
            {
                fprintf(file, " %d", curr->_urlRecordIndex);
                
                for (j = 0; j < 2039; j++)
                {
                    if (printed[j] < 0)
                    {
                        printed[j] = curr->_urlRecordIndex;
                        break;
                    }
                }
            }
            
            curr = curr->_next;
        }
        
        fprintf(file, "\n");
    }
    
    fclose(file);
}*/

void WebCrawler::writeWordFile(const char * wordFileName) {
	FILE * file = fopen(wordFileName, "w");

	HashTableTemplateIterator<URLRecordList *> iterator(_wordToURLRecordList);

	const char * key;
	URLRecordList * data;
	int yolo = -1;

	while (iterator.next(key, data)) {
		fprintf(file, "%s ", key);
		URLRecordList * curr = data;
		while (curr != NULL) {
			if (yolo != curr->_urlRecordIndex) {
			fprintf(file, "%d ", curr->_urlRecordIndex);}
			yolo = curr->_urlRecordIndex;
			curr = curr->_next;
		}
		fprintf(file, "\n");
	}
	fclose(file);
}

/*void WebCrawler::writeWordFile(const char * wordFileName) {
	FILE * file = fopen(wordFileName, "w");
	int j = 0;
	for (int i = 0; i < 2039; i++) {
		if (_wordToURLRecordList->_buckets[i] == NULL) {
			continue;
		}
		fprintf(file, "%s ", _wordToURLRecordList->_buckets[i]->_key);
		URLRecordList * curr = NULL;

		curr = _wordToURLRecordList->_buckets[i]->_data;
		fprintf(file, "%d ", curr->_urlRecordIndex);
		while (curr != NULL) {
			fprintf(file, "%d ", curr->_urlRecordIndex);
			curr = curr->_next;
		}
		fprintf(file, "\n");
	}
	fclose(file);
}*/

void WebCrawler::wordToHashTable() {
	for (int i = 0; i < _tailURL; i++) {
		if (_urlArray[i]._description != NULL) {
			char * c;
			URLRecordList *temp = NULL;
			URLRecordList *curr;
			char * yodo = _urlArray[i]._description;
			while ((c = nextWord(yodo)) != NULL) {
				//printf("%s\n", c);
				if(_wordToURLRecordList->find(c, &temp) == false) {
					URLRecordList *data = new URLRecordList();
					data->_urlRecordIndex = i;
					data->_next = temp;
					printf("Added: %s\n", c);
					_wordToURLRecordList->insertItem(c, data);
				} else {
					printf("The other place: %s\n", c);
					int found = 0;
					URLRecordList * tmp = temp;
					while (tmp != NULL) {
						if (tmp->_urlRecordIndex == i) {
							found = 1;
							break;
						}
						tmp = tmp->_next;
					}

					if (found == 1) {
						//printf("Chutiya kaat raha hai %s\n", c);
						found = 0;
						continue;
					} else {
						printf("Yahan tak mai pahunch gaya hun: %s\n", c);
						URLRecordList *data = new URLRecordList();
						data->_urlRecordIndex = i;
						data->_next = temp;
						_wordToURLRecordList->insertItem(c, data);
					}
				}
			}
		}
	}
}

void WebCrawler::urlInHashTable() {
	for (int i = 0; i < _tailURL; i++) {
		//printf("\nYOLO\n");
		int temp;
		if (_urlToUrlRecord->find(_urlArray[i]._url, &temp) == false) {
			//printf("YOLO\n");
			_urlToUrlRecord->insertItem(_urlArray[i]._url, i);
		}
	}
}

void WebCrawler::crawl() {
	printf("Nice\n");
    while (_headURL < _tailURL) {
		printf("Nice in\n"); 
		char *curr = _urlArray[_headURL]._url;
		printf("Niceinone\n");
        int n;
		printf("Niceintwo\n");
        char *buffer = fetchHTML(curr, &n);
		printf("Niceinthree\n");
        if (buffer == NULL) {
            _headURL = _headURL + 1;
            continue;
        }
        
        parse(buffer, n);
		//printf("\nIt comes here %d\n", _headURL);
        _headURL = _headURL + 1;
    }
	
	//Call URLmethod
	printf("This Happens\n");
	urlInHashTable();

	//Testing nextWord

	//printf("TestingNextWord\n");

	//char * yo = (char *) malloc (10000);
	//yo = strdup(_urlArray[0]._description);
	//printf("%s\n", yo);
	//char * c = (char *) malloc (10000);
	//while ((c = nextWord(yo)) != NULL) {
		//printf("%s\n", c);
	//}
	for (int i = 0; i < _tailURL; i++) {
		printf("%i %s\n", i, _urlArray[i]._description);
	}

	printf("This also happens\n");
	wordToHashTable();
	printf("Does this happen?\n");
	const char * alpha = "url.txt";
	writeURLFile(alpha);
	writeWordFile("word.txt");
	//writeURLFile("url.txt");
}

int main (int argc, char ** argv) {
	if (argc < 2) {
		fprintf( stderr, "  Usage: webcrawler [-u <maxurls>] url-list\n");
		exit(1);
	}

	int maxUrls = 45;

	int inserted = 0;

	int start = 1;
	
	const char ** initialUrls;

	if (strcmp(argv[1], "-u") == 0) {
		maxUrls = atoi(argv[2]);
		start = 3;
	}

	initialUrls = new const char * [(argc - start)];

	int i;

	for (i = start; i < argc; i++) {
		initialUrls[i - start] = argv[i];
	}
	WebCrawler * crawler = new WebCrawler(maxUrls, (argc - start), initialUrls);
	crawler->crawl();
	//crawler->writeURLFile("url.txt");
	//crawler->writeWordFile("word.txt");
	
}
