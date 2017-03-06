
JAVACC=/homes/cs352/javacc-6.0/bin/javacc
JAVAc=javac

all: git-commit ExprEval.class SimpleC.class Tokens.class Lexer.class

Lexer.class: Lexer.java
	$(JAVAC) Lexer.java

Tokens.class: Tokens.java
	$(JAVAC) Tokens.java

ExprEval.class: ExprEval.java
	$(JAVAC) ExprEval.java

SimpleC.class: SimpleC.jj
	$(JAVACC) SimpleC.jj 
	javac SimpleC.java

git-commit:
	git add *.jj *.java tests/*.c Makefile >> .local.git.out  || echo
	git add *.pdf 2>/dev/null >> .local.git.out  || echo
	git commit -a -m 'Commit' >> .local.git.out || echo

clean:
	rm SimpleC.java SimpleCConstants.java SimpleCharStream.java TokenMgrError.java ParseException.java SimpleCTokenManager.java Token.java *.class
