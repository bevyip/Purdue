
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "search-engine.h"

DictionaryType dictType;

SearchEngine::SearchEngine( int port, DictionaryType dictionaryType):
  MiniHTTPD(port)
{
  // Create dictionary of the indicated type

  // Populate dictionary and sort it if necessary

	if (dictionaryType == ArrayDictionaryType)
		_wordToURLList = new ArrayDictionary();
	else if (dictionaryType == HashDictionaryType)
		_wordToURLList = new HashDictionary();
	else if (dictionaryType == AVLDictionaryType)
		_wordToURLList = new AVLDictionary();
	else if (dictionaryType == BinarySearchDictionaryType)
		_wordToURLList = new BinarySearchDictionary();
	else
		_wordToURLList = NULL;
		
	dictType = dictionaryType;

	URLRecord ** records = new URLRecord *[1024];

	for (int i = 0; i < 1024; i++) {
		records[i] = new URLRecord();
	}

	char * line = (char *) malloc (1000 * sizeof(char));

	FILE * uFile = fopen("url.txt", "r");

	int alternate = 0;
	int uIndex;
	while (fgets(line, 500, uFile)) {
		if (alternate == 0) {
			char * temp = line;
			char * ind = (char *) malloc (5 * sizeof(char));
			char * i = ind;
			while (*temp != ' ') {
				*i = *temp;
				i++;
                temp++;
           	}
			*i = '\0';
			uIndex = atoi(ind);
          	while (*temp == ' ') {
           		temp++;
            }
			records[uIndex]->_url = strdup(temp);
			printf("index: %d url: %s\n", uIndex, records[uIndex]->_url);
			alternate = 1;
			continue;
		} else if (alternate == 1) {
			records[uIndex]->_description = strdup(line);
			printf("index: %d description: %s\n", uIndex, records[uIndex]->_description);
			alternate = 2;
			continue;
		} else {
			alternate = 0;
			continue;
		}
	}
	fclose(uFile);

	FILE * wFile = fopen("word.txt", "r");
	char * word = (char *) malloc (100 * sizeof(char));
	char * tLine = (char *) malloc (500 * sizeof(char));
	char * mWord = (char *) malloc (100 * sizeof(char));
	int flag = 0;
	int wIndex;

	URLRecordList * head;
	URLRecordList * prev = new URLRecordList();

	while (fgets(line, 500, wFile)) {
		tLine = strdup(line); 
		while ((word = nextWord(tLine)) != NULL) {
			if (flag == 0) {

				head = NULL;

				prev = NULL;

				strcpy(mWord, word);
				printf("word: %s\n", mWord);
				flag = 1;
				continue;
			}


			wIndex = atoi(word);

			if (records[wIndex]->_url == NULL) {
				continue;
			}

			URLRecordList * entry = new URLRecordList();

			if (head == NULL) {
				head = entry;
			}

			entry->_urlRecord = records[wIndex];
			entry->_next = NULL;

			if (prev != NULL) {
				prev->_next = entry;
			}
			//printf("yolo\n");
			prev = entry;
			//printf("yolo\n");
			printf("%d\n", wIndex);
		}
		_wordToURLList->addRecord(mWord, (URLRecordList*)head);
		printf("\n");
		flag = 0;
	}
	fclose(wFile);
}


char * SearchEngine::nextWord(char * &c) {
	char * yod = (char *) malloc (sizeof(char) * 100);
	//memset(yod, 0, strlen(yod));
	int i = 0;
	//if (c == NULL) {
	//	return NULL;
	//}
	while (*c != '\0') {
		//printf("I'm in!\n");
		if (*c == ' ' || *c == '\n' || *c == '+') {
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

bool SearchEngine::match(char * &yolo) {
        if (!strncmp(yolo, "/search?word=", 13)) {
                yolo += 13;
                return true;
        }
        return false;
}

void
SearchEngine::dispatch( FILE * fout, const char * documentRequested)
{
  if (strcmp(documentRequested, "/")==0) {
    // Send initial form
    fprintf(fout, "<TITLE>Boiler Search</TITLE>\r\n");
	fprintf(fout, "<BODY alink=\"green\" vlink=\"#663399\" link=\"black\" background=\"https://news.uns.purdue.edu/images/%2B2005/ranking05-purdue.jpg\">\n");
    fprintf(fout, "<CENTER><H1><FONT FACE=\"VERDANA\" size=16>Boiler <FONT FACE=\"VERDANA\" size=16 color=\"#ffd700\">Search</FONT></FONT></H1>\n");
    fprintf(fout, "<H2>\n");
    fprintf(fout, "<FORM ACTION=\"search\">\n");
    fprintf(fout, "Search:\n");
    fprintf(fout, "<INPUT TYPE=\"text\" NAME=\"word\" MAXLENGTH=\"80\"><P>\n");
    fprintf(fout, "</H2>\n");
	fprintf(fout, "<BR>");
	fprintf(fout, "<h2><a href=\"http://www.google.com\">-Shreyaansh Bassi</a></h2>");
    fprintf(fout, "</FORM></CENTER>\n");
	fprintf(fout, "</BODY>\n");
    return;
  }

  // TODO: The words to search in "documentRequested" are in the form
  // /search?word=a+b+c
  //
  // You need to separate the words before search
  // Search the words in the dictionary and find the URLs that
  // are common for al the words. Then print the URLs and descriptions
  // in HTML. Make the output look nicer.

  // Here the URLs printed are hardwired
  
  const int nurls=2;

	if (strlen(documentRequested) < 13) {
		return;
	}

	char * search = new char[strlen(documentRequested) + 1];
	strcpy(search, documentRequested);

	char ** searchWords = new char * [50];

	for (int i = 0; i < 50; i++) {
		searchWords[i] = NULL;
	}

	int nWords = 0;

	char * temp = (char *) malloc (50 * sizeof(char));

	if (match(search)) {
		while ((temp = nextWord(search)) != NULL) {
			searchWords[nWords++] = strdup(temp);
			printf("%d %s\n", nWords, temp);
		}
	}

	char * words = new char[1000 * sizeof(char)];

	strcpy(words, "");

	for (int i = 0; i < nWords; i++) {
		strcat(words, searchWords[i]);
		strcat(words, " ");
	}

	printf("Words to search for: %s\n", words);

  fprintf( stderr, "Search for words: \"%s\"\n", words);

  fprintf( fout, "<TITLE>Search Results</TITLE>\r\n");
  fprintf(fout, "<BODY link=\"#FF4500\" alink=\"#FF4500\" vlink=\"green\" background=\"http://10steps.sg/wp-content/uploads//2012/11/White-christmas-light-background.jpg\" style=\"background-attachment: fixed; background-repeat: no-repeat;\">\n");
  fprintf(fout, "<font face=\"verdana\">");
  fprintf(fout, "<CENTER><H1><FONT FACE=\"VERDANA\" size=16>Boiler <FONT FACE=\"VERDANA\" size=16 color=\"#ffd700\">Search</FONT></FONT></H1>\n");
  fprintf( fout, "<H2> <marquee>Search Results for \"%s\"</marquee></center></H2>\n", words );
  fprintf(fout, "<H2>\n");
  fprintf(fout, "<FORM ACTION=\"search\">\n");
  fprintf(fout, "<a href=\"%s\">Search:</a>\n", "http://data.cs.purdue.edu:8888");
  fprintf(fout, "<INPUT TYPE=\"text\" NAME=\"word\" MAXLENGTH=\"80\"><P>\n");
  fprintf(fout, "</H2>\n");
  fprintf(fout, "</FORM><HR>\n");

	int counter;
	counter = 0;
	
	int listCount;
	listCount = 0;

	URLRecord **list = new URLRecord*[500];

	for (int i = 0; i < nWords; i++) {
		URLRecordList* data;
		data = (URLRecordList*)_wordToURLList->findRecord(searchWords[i]);
		
		while (data != NULL) {
			int exists = 0;
			
			int j;
			for (j = 0; j < listCount; j++) {
				if (list[j] == data->_urlRecord) {
					exists = 1;
					break;
				}
			}
			
			if (exists == 0) {
				list[listCount] = data->_urlRecord;
				listCount++;
			}
			
			data = data->_next;
		}
	}
	
	for (int i = 0; i < listCount; i++) {
		for (int j = 0; j < nWords; j++) {
			URLRecordList* data;
			data = (URLRecordList*)_wordToURLList->findRecord(searchWords[j]);
			
			int exists = 0;
			
			while (data != NULL) {
				if (data->_urlRecord == list[i]) {
					exists = 1;
				}
				
				data = data->_next;
			}
			
			if (exists == 0)
				list[i] = NULL;
		}
	}

  for ( int i = 0; i < listCount; i++ ) {

	if (list[i] == NULL) {
		continue;
	}

    fprintf(fout, "<h3>%d. <a href=\"%s\">%s</a><h3>\n", counter+1, list[i]->_url, list[i]->_url);
	fprintf(fout, "<blockquote>%s<p></blockquote>\n", list[i]->_description);

	counter++;
  }

	char * uu = (char *) malloc (30 * sizeof(char *));
	strcpy(uu, "http://data.cs.purdue.edu:8888");
  // Add search form at the end
  fprintf(fout, "</font>");
  fprintf(fout, "</BODY>\n");
}

void
printUsage()
{
  const char * usage =
    "Usage: search-engine port (array | hash | avl | bsearch)\n"
    "  It starts a search engine at this port using the\n"
    "  data structure indicated. Port has to be larger than 1024.\n";

  fprintf(stderr, "%s", usage);
}

int main(int argc, char ** argv)
{
  if (argc < 3) {
    printUsage();
    return 1;
  }

  // Get port
  int port;
  sscanf( argv[1], "%d", &port);

  // Get DictionaryType
  const char * dictType = argv[2];
  DictionaryType dictionaryType;
  if (!strcmp(dictType, "array")) {
    dictionaryType = ArrayDictionaryType;
  }
  else if (!strcmp(dictType, "hash")) {
    dictionaryType = HashDictionaryType;
  }
  else if (!strcmp(dictType, "avl")) {
    dictionaryType = AVLDictionaryType;
  }
  else if (!strcmp(dictType, "bsearch")) {
    dictionaryType = BinarySearchDictionaryType;
  }
  else {
    printUsage();
    return 0;
  }
  
  SearchEngine httpd(port, dictionaryType);
  
  httpd.run();

  return 0;
}
