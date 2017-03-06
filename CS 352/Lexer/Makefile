
JAVACC=/homes/cs352/javacc-6.0/bin/javacc
JAVAc=javac

all: git-commit DecimalParser.class FloatParser.class SimpleC.class 

DecimalParser.class: DecimalParser.java
	$(JAVAC) DecimalParser.java

FloatParser.class: FloatParser.java
	$(JAVAC) FloatParser.java

SimpleC.class: SimpleC.jj
	$(JAVACC) SimpleC.jj 
	javac SimpleC.java

git-commit:
	git add *.jj *.java tests/*.c Makefile >> .local.git.out  || echo
	git add *.pdf 2>/dev/null >> .local.git.out  || echo
	git commit -a -m 'Commit' >> .local.git.out || echo

clean:
	rm SimpleC.java SimpleCConstants.java SimpleCharStream.java TokenMgrError.java ParseException.java SimpleCTokenManager.java Token.java *.class
