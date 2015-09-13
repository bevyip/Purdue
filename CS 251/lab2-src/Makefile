
all: gethttp webcrawler git-commit

gethttp: gethttp.cpp openhttp.cpp SimpleHTMLParser.cpp
	g++ -o gethttp -g gethttp.cpp openhttp.cpp SimpleHTMLParser.cpp

webcrawler: webcrawler.cpp gethttp.cpp openhttp.cpp SimpleHTMLParser.cpp
	g++ -o webcrawler -g openhttp.cpp SimpleHTMLParser.cpp webcrawler.cpp HashTableTemplate.h

git-commit:
	git add Makefile *.h *.cpp  >> .local.git.out
	git commit -a -m "Commit web-crawler" >> .local.git.out

clean:
	rm -f *.o gethttp core
