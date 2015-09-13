
all: test_mystring test_slist isbalanced countwords test_mysort test_bits

test_mystring: mystring.c test_mystring.c
	gcc -g -o test_mystring mystring.c test_mystring.c

test_slist: test_slist.c slist.c
	gcc -g -o test_slist test_slist.c slist.c

isbalanced: isbalanced.c
	gcc -g -o isbalanced isbalanced.c

countwords: countwords.c
	gcc -g -o countwords countwords.c

test_mysort: mysort.c test_mysort.c
	gcc -g -o test_mysort mysort.c test_mysort.c

test_bits: bits.c test_bits.c
	gcc -g -o test_bits bits.c test_bits.c

org:
	cp test_mystring test_mystring.org
	cp test_slist test_slist.org
	cp isbalanced isbalanced.org
	cp countwords countwords.org
	cp test_mysort test_mysort.org
	cp test_bits test_bits.org 

clean:
	rm -f test_mystring isbalanced test_slist countwords test_mysort test_bits *.out

