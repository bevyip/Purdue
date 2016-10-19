
/*
 * CS-252
 * shell.y: parser for shell
 *
 * This parser compiles the following grammar:
 *
 *	cmd [arg]* [> filename]
 *
 * you must extend it to understand the complete shell grammar
 *
 */

%token	<string_val> WORD

%token 	NOTOKEN NEWLINE GREATGREATAMPERSAND GREATGREAT GREATAMPERSAND AMPERSAND GREAT PIPE LESS

%union	{
		char   *string_val;
	}

%{
//#define yylex yylex
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <regex.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include "command.h"
void yyerror(const char * s);
int yylex();

void expandWildCardsIfNecessary(char * arg);
void expandWildCards(char * prefix, char * arg);
int cmpfunc(const void * file1, const void * file2);
bool is_dir(const char * path);

%}

%%

goal:	
	commands
	;

commands: 
	command
	| commands command 
	;

command: simple_command
        ;

simple_command:	
	pipe_list iomodifier_list background_optional NEWLINE {
		//printf("   Yacc: Execute command\n");
		Command::_currentCommand.execute();
	}
	| NEWLINE { 
	} 
	| error NEWLINE { yyerrok; }
	;

command_and_args:
	command_word argument_list {
		Command::_currentCommand.
			insertSimpleCommand( Command::_currentSimpleCommand );
	}
	;
	
argument_list:
	argument_list argument
	| /* can be empty */
	;

argument:
	WORD {
        	//printf("   Yacc: insert argument \"%s\"\n", $1);
		expandWildCardsIfNecessary($1);
	}
	;

command_word:
	WORD {
               //printf("   Yacc: insert command \"%s\"\n", $1);
	       Command::_currentSimpleCommand = new SimpleCommand();
	       Command::_currentSimpleCommand->insertArgument( $1 );
	}
	;

pipe_list:
	pipe_list PIPE command_and_args
	| command_and_args
	;

iomodifier_opt:
	GREAT WORD {
		//printf("   Great Word Yacc: insert output \"%s\"\n", $2);
		Command::_currentCommand._outFile = strdup($2);
		Command::_currentCommand._outCounter++;
	}
	| GREATGREAT WORD {
		//printf("   GreatGreat Word Yacc: insert output \"%s\"\n", $2);
		Command::_currentCommand._outFile = strdup($2);
		Command::_currentCommand._append = 1;
		Command::_currentCommand._outCounter++;
	}
	| GREATAMPERSAND WORD {
		//printf("   GreatAmpersand Word Yacc: insert output \"%s\"\n", $2);
		Command::_currentCommand._outFile = strdup($2);
		Command::_currentCommand._errFile = strdup($2);
		Command::_currentCommand._outCounter++;

	}
	| GREATGREATAMPERSAND WORD {
		//printf("   GreatGreatAmpersand Word Yacc: insert output \"%s\"\n", $2);
		Command::_currentCommand._outFile = strdup($2);
		Command::_currentCommand._errFile = strdup($2);
		Command::_currentCommand._outCounter++;
		Command::_currentCommand._append = 1;
	}
	| LESS WORD {
		//printf("   Less WordYacc: insert input \"%s\"\n", $2);
		Command::_currentCommand._inFile = strdup($2);
		Command::_currentCommand._inCounter++;
	}
	;

iomodifier_list:
	iomodifier_list iomodifier_opt
	| iomodifier_opt
	|
	;

background_optional:
	AMPERSAND {
		Command::_currentCommand._background = 1;
	}
	|
	;

%%

bool is_dir(const char * path) {
	struct stat buf;
	stat(path, &buf);
	return S_ISDIR(buf.st_mode);
}

void expandWildCardsIfNecessary(char * arg) {

	if (strchr(arg, '*') || strchr(arg, '?')) expandWildCards(NULL, arg);
	else Command::_currentSimpleCommand->insertArgument(arg);
	return;
}

void expandWildCards(char * prefix, char * arg) {
	
	char * dir = NULL;
	
	if (prefix) dir = strcat(prefix, "/");

	char * temp = arg;
	char * tDir = (char *) malloc (100);
	char * save = tDir;

	while (*temp != '/' && *temp) *(tDir++) = *(temp++);
	*tDir = '\0';

	if (strchr(save, '*') || strchr(save, '?')) {
		//if (dir) save = strcat(prefix, save);
		//printf("preprefix is: %s\n", save);
		//if (*temp) expandWildCards(save, ++temp);
		char * reg = (char *) malloc (2 * strlen(arg) + 10);
		char * a = arg;
		char * r = reg;
		*(r++) = '^';

		while (*a) {
			if (*a == '*') { *(r++) = '.'; *(r++) = '*'; }
			else if (*a == '?') { *(r++) = '.'; }
			else if (*a == '.') { *(r++) = '\\'; *(r++) = '.'; }
				else { *(r++) = *a; }
			a++;
		}

		*(r++) = '$'; *r = '\0';

		regex_t re;

		int expbuf = regcomp(&re, reg, REG_EXTENDED|REG_NOSUB);

		DIR * dir = opendir((prefix) ? prefix:".");
		if (dir == NULL) {
			perror("opendir");
			return;
		}

		struct dirent * ent;
		regmatch_t match;

		char ** fileList = (char **) malloc (10000 * sizeof(char *));
		int maxEntries = 100;
		int numEntries = 0;

		char yy[1000];

		while ((ent = readdir(dir)) != NULL) {
			expbuf = regexec(&re, ent->d_name, 1, &match, 0);
			if (expbuf == 0) {
				
			}
		}
	} else {
		if (dir) save = strcat(prefix, save);
		printf("prefix is: %s\n", save);
		if (*temp) expandWildCards(save, ++temp);
		else Command::_currentSimpleCommand->insertArgument(save);
	}
}

void
yyerror(const char * s)
{
	fprintf(stderr,"Hello world: %s", s);
}

#if 0
main()
{
	yyparse();
}
#endif
