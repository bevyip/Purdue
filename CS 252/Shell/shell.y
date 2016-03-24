
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

%token 	NOTOKEN GREAT NEWLINE GREATGREAT GREATGREATAMPERSAND GREATAMPERSAND PIPE LESS AMPERSAND

%union	{
		char   *string_val;
	}

%{
//#define yylex yylex
#include <stdio.h>
#include "command.h"
#include <string.h>
#include <stdlib.h>
#include <regex.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <signal.h>

void yyerror(const char * s);
int yylex();
void expandWildCardsIfNecessary(char * arg);
void expandWildcards(char * prefix, char * suffix, char * extra);
int cmpfunc(const void *file1, const void *file2);

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
	pipe_list io_modifier_list background_opt NEWLINE {
		//printf("   Yacc: Execute command\n");
		Command::_currentCommand.execute();
	}
	| NEWLINE
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
                //printf("It goes here!\n");
                expandWildCardsIfNecessary($1);
	}
	;

command_word:
	WORD {
               //printf("   Yacc: insert command \"%s\"\n", $1);
               Command::_currentSimpleCommand = new SimpleCommand();
               Command::_currentSimpleCommand->insertArgument($1);
	}
	;

io_modifier_opt:
	GREAT WORD		{
		//printf("   Yacc: insert output \"%s\"\n", $2);
		Command::_currentCommand._outFile = strdup($2);
        Command::_currentCommand._inCounter++;
	}

	| GREATGREAT WORD 	{
		//printf("   Yacc: insert output \"%s\"\n", $2);
		Command::_currentCommand._outFile = strdup($2);
		Command::_currentCommand._append = 1;
        Command::_currentCommand._outCounter++;
	}

	| GREATAMPERSAND WORD 	{
		//printf("   Yacc: insert output \"%s\"\n", $2);
		Command::_currentCommand._outFile = strdup($2);
		Command::_currentCommand._errFile = strdup($2);
        Command::_currentCommand._outCounter++;
	}

	| GREATGREATAMPERSAND WORD 	{
		//printf("   Yacc: insert output \"%s\"\n", $2);
		Command::_currentCommand._outFile = strdup($2);
		Command::_currentCommand._errFile = strdup($2);
        Command::_currentCommand._append = 1;
        Command::_currentCommand._outCounter++;
	}

	| LESS WORD 	{
		//printf("   Yacc: insert output \"%s\"\n", $2);
		Command::_currentCommand._inFile = strdup($2);
        Command::_currentCommand._inCounter++;
	}


	;


pipe_list:
pipe_list PIPE command_and_args
| command_and_args
;

io_modifier_list:
io_modifier_list io_modifier_opt
| /*empty*/
;
background_opt:
AMPERSAND {
	Command::_currentCommand._background = 1;
}
| /*empty*/
;

%%

void expandWildCardsIfNecessary(char * arg) {
    //printf("It Comes here!\n");
    //printf("The argument coming here is: %s\n", arg);
    char * checkStar = NULL;
    char * checkQuestionMark = NULL;
    if ((checkStar = strchr(arg, '*')) || (checkQuestionMark = strchr(arg, '?'))) {
        //printf("Found a star!\n");
        //expandWildcards("lab3-src/t/a/s", "ty*", "fd*");
        expandWildcards(NULL, arg, NULL);
    } else {
        //printf("Didn't find a star!\n");
        Command::_currentSimpleCommand->insertArgument( arg );
    }
    //printf("The character contains a strchr\n");
    return;
}

//int numEntries = 0;

int cmpfunc (const void *file1, const void *file2) {
	const char *_file1 = *(const char **)file1;
	const char *_file2 = *(const char **)file2;
	return strcmp(_file1, _file2);
}

void expandWildcards(char * prefix, char * suffix, char * extra) {
    //Let's get prof Gustavo's code working
    //printf("It came here\n");

    //printf("%s\n", suffix);

    //Start recursion #goodGames

    char before[100];
    char after[100];
    char * newSuffix = (char *) malloc (100);
    char * directory = (char *) malloc (100);

    if (prefix == NULL) {
        printf("prefix\n");
        //printf("Suffix: %s\n", suffix);
        int len = strlen(suffix);
        //printf("Length: %d\n", len);

        //Add a NULL terminating character
        suffix[len] = '\0';

        directory = strdup(".");

        int i = 0;
        int j = 0;

        char * tempSuffix = suffix;
        printf("suffix: %s, tempSuffix: %s\n", suffix, tempSuffix);
        char * temp;

        if (strchr(suffix, '/') != NULL) {
            printf("Can be expanded!\n");
            //get the string before the first suffix

            //printf("Will it go in here?\n");

            char * container = (char *) malloc (100);
            char * tmp = (char *) malloc (100);

            int k;
            int count = 0;

            printf("here\n");

            while (*tempSuffix != '\0') {
                k = 0;
                while (*tempSuffix != '/' && *tempSuffix != '\0') {
                    container[k++] = *tempSuffix;
                    tempSuffix++;
                }
                container[k] = '\0';
                printf("container: %s\n", container);
                if (strchr(container, '*')) {
                    break;
                } else {
                    count++;
                    strcat(tmp, "/");
                    strcat(tmp, container);
                }
                tempSuffix++;
            }

            if (count == 0) {
                printf("First container star\n");
                printf("ContainEr: %s\n", container);
                newSuffix = strdup(container);
                int l = 0;
                printf("tempSuffix: %s\n", tempSuffix);
                tempSuffix++;
                while (*tempSuffix != '/' && *tempSuffix != '\0') {
                    container[l++] = *tempSuffix;
                    tempSuffix++;
                }
                container[l] = '\0';
                printf("container: %s\n", container);
                extra = strdup(container);

                if (*tempSuffix != '\0') {
                    int l = 0;
                    printf("tempSuffix: %s\n", tempSuffix);
                    tempSuffix++;
                    while (*tempSuffix != '\0') {
                        container[l++] = *tempSuffix;
                        tempSuffix++;
                    }
                    container[l] = '\0';
                    printf("container: %s\n", container);
                    //if (container[0] != '\0') {
                    //    extra = strdup(container);
                    //}
                }

            } else {

                printf("there\n");

                //clear direct
                while (*tmp != '/') {
                    tmp++;
                }
                printf("direct: %s\n", tmp);

                tempSuffix--;

                //fix tempsuffixprob
                while (*tempSuffix != '/' && *tempSuffix != suffix[0]) {
                    tempSuffix--;
                }

                printf("tmpSuffix: %s\n", tempSuffix);

                //Get the extra
                char * wasd = tempSuffix;
                wasd++;
                while(*wasd != '/' && *wasd!='\0') { wasd++; }
                if (*wasd == '\0') {
                    extra = NULL;
                } else {
                    wasd++;
                    extra = strdup(wasd);
                }
                printf("extras: %s\n", wasd);

                //Fix start problem
                char * tt = tmp;
                int i = 0;
                while (tmp[i] != '\0') {
                    tmp[i] = tmp[++i];
                }
                i = 0;
                printf("dir: %s\n", tt);

                //before[--i] = '\0';
                //printf("directory: %s\n", before);
                printf("tempSuffixNow: %s\n", tmp);

                /*if (*tempSuffix == '/') {
                    printf("Suffix can be extracted\n");
                    tempSuffix++;
                    while (*tempSuffix != '/' && *tempSuffix != '\0') {
                        //printf("%c\n", *tempSuffix);
                        after[j] = *tempSuffix;
                        j++; tempSuffix++;
                    }
                    after[j] = '\0';
                    //printf("newSuffix: %s\n", after);
                    newSuffix = strdup(after);
                }*/
                //printf("It comes here!\n");
                if (before != NULL) {
                    //printf("directory: %s\n", tt);
                    directory = strdup(tt);
                }
                newSuffix = strdup(container);

                if (suffix[0] == '/') {
                    directory = strdup("/");
                }
            }
        } else {
            printf("can't be expanded!\n");
            if (suffix[0] == '.' && suffix[1] == '*') {
                printf("case 1\n");
                newSuffix = strdup(".*");
                directory = strdup(".");
            } else if (suffix[0] == '*') {
                printf("case 2\n");
                newSuffix = strdup("*");
                directory = strdup(".");
            } else {
                printf("case 3\n");
                char * ttt = (char *) malloc (100);
                char * tmpSuffix = suffix;
                int i = 0;
                while (*tmpSuffix != '/' && *tmpSuffix != '\0') {
                    ttt[i++] = *tmpSuffix;
                    tmpSuffix++;
                }
                ttt[i] = '\0';
                printf("ttt: %s\n", ttt);
                newSuffix = strdup(ttt);
            }
        }
    } else {
        printf("It comes here!\n");
        directory = strdup(prefix);
        printf("dir: %s, suffix: %s, extra: %s\n", directory, suffix, extra);
        newSuffix = strdup(suffix);
    }

    char * reg = (char *) malloc (2 * strlen(suffix) + 10);
    char * a = newSuffix;
    char * r = reg;

    printf("before\n");


    while (*a) {
        if (*a == '*') { *r = '.'; r++; *r = '*'; r++; }
        else if (*a == '?') { *r = '.'; r++; }
        else if (*a == '.') { *r = '\\'; r++; *r = '.'; r++; }
        else { *r = *a; r++; }
        a++;
    }

    *r = '\0';

    printf("after\n");

    char regExpComplete[1024];
    sprintf(regExpComplete, "^%s$", reg);

    //printf("%s\n", regExpComplete);

    regex_t re;

    int expbuf = regcomp(&re, regExpComplete, REG_EXTENDED|REG_NOSUB);
    //printf("expbuf %d \n ", expbuf);
    //error checking skip for now

    printf("Final Directory: %s\n", directory);
    printf("Final suffix: %s\n", newSuffix);
    printf("Final extra(s): %s\n", extra);

    DIR * dir = opendir(directory);
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

    //ent = readdir(dir);

    if (ent == NULL) { printf("ent is null\n"); }
    else { printf("It isn't NULL"); }
    while ((ent = readdir(dir)) != NULL) {
	//printf("It enters here!\n");
	expbuf = regexec(&re, ent->d_name, 1, &match, 0);
	//printf("expbuf: %d\n", expbuf);
        if (expbuf == 0) {
            //printf("It enters here!\n");
	        if (ent->d_name[0] == '.') {
                if (newSuffix[0] == '.') {
                    //printf("yolo\n");
                    if (!strcmp(directory, ".")) {
                        //printf("Yolo case\n");
                        fileList[numEntries++] = strdup(ent->d_name);
                    } else {
                        //printf("yolobolo\n");
                        char * yy = (char *) malloc (1024);
                        strcat(yy, "");
                        strcat(yy, directory);
                        if (strcmp(directory, "/")) {
                            strcat(yy, "/");
                        }
                        strcat(yy, ent->d_name);
                        strcat(yy, "\0");
                        //printf("yy: %s\n", yy);
                        while (*yy != directory[0]) { yy++; }
                        fileList[numEntries++] = strdup(yy);
                    }
                }
            } else if (newSuffix[0] == '*') {
                if (!strcmp(directory, ".")) {
                    fileList[numEntries++] = strdup(ent->d_name);
                } else {
                    char * yy = (char *) malloc (1024);
                    strcpy(yy, directory);
                    if (strcmp(directory, "/")) {
                        strcat(yy, "/");
                    }
                    strcat(yy, ent->d_name);
                    strcat(yy, "\0");
                    //printf("yy: %s\n", yy);
                    //get yy to correct position
                    while (*yy != directory[0]) { yy++; }
                    fileList[numEntries++] = strdup(yy);
                    free(yy);
                }
            } else {
                //Command::_currentSimpleCommand->insertArgument(strdup(ent->d_name));
                //strcat(yy, directory);
                //strcat(yy, "/");
                //strcat(yy, ent->d_name);
                //printf("dir: %s\nyy: %s\nname: %s\n", directory, yy, ent->d_name);
                //printf("Comes here\n");

                if (!strcmp(directory, ".")) {
                    fileList[numEntries++] = strdup(ent->d_name);
                } else {
                    char * yy = (char *) malloc (1024);
                    strcat(yy, "");
                    strcat(yy, directory);
                    if (strcmp(directory, "/")) {
                        strcat(yy, "/");
                    }
                    strcat(yy, ent->d_name);
                    strcat(yy, "\0");
                    while (*yy != directory[0]) { yy++; }
                    //printf("yy: %s\n", yy);

                    fileList[numEntries++] = strdup(yy);
                    //fileList[numEntries++] = strdup(ent->d_name);
    		        //printf("Dir: %s\n", ent->d_name);
                }
            }
        }
    }

    closedir(dir);

    //printf("Number of entries: %d\n", numEntries);

    //Sort the entries
    qsort(fileList, numEntries, sizeof(char *), cmpfunc);

    for(int i = 0; i < numEntries; i++) {
        //printf("%d\n", i);
        //printf("%s\n", fileList[i]);
        Command::_currentSimpleCommand->insertArgument(fileList[i]);
    }
    //printf("\nAll Done\n");
    free(fileList);
    numEntries = 0;
    return;
}

void
yyerror(const char * s)
{
	fprintf(stderr,"%s", s);
}

#if 0
main()
{
	yyparse();
}
#endif
