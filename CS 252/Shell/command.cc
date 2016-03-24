/*
 * CS252: Shell project
 *
 * Template file.
 * You will need to add more code here to execute the command table.
 *
 * NOTE: You are responsible for fixing any bugs this code may have!
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>

#include "command.h"

extern char **environ;

int * backgroundPIDs;

SimpleCommand::SimpleCommand() {
	// Create available space for 5 arguments
	_numOfAvailableArguments = 5;
	_numOfArguments = 0;
	_arguments = (char **) malloc( _numOfAvailableArguments * sizeof( char * ) );
}

void SimpleCommand::insertArgument( char * argument ) {
	if ( _numOfAvailableArguments == _numOfArguments  + 1 ) {
		// Double the available space
		_numOfAvailableArguments *= 2;
		_arguments = (char **) realloc( _arguments,
				  _numOfAvailableArguments * sizeof( char * ) );
	}

	/*char * regularExp = "^.*${[^}][^}]*}.*$";
	regex_t re;
	regmatch_t match;
	int result = regcomp(&re, buffer, 0);

	if (result != 0) {
		perror("regcomp");
		return;
	}*/

	//Try to do the C way
	char * arg = strdup(argument);
	//printf("Argument: %s\n", arg);
	char * checkDollar = strchr(arg, '$');
	char * temp = arg;

	char * concatenation = (char *) malloc (strlen(argument) + 100);
	char * concat = concatenation;

	if (checkDollar != NULL) {
		while (*arg != '$') {
			*concat = *arg;
			concat++;
			arg++;
		}
		*concat = '\0';
		//printf("Concat: %s\n", concatenation);
	}

	while (checkDollar != NULL) {
		//printf("Found Dollar!\n");
		//printf("CheckDollar: %s\n", checkDollar);
		if (checkDollar[1] == '{' && checkDollar[2] != '}') {
			char * temp = checkDollar;
			temp += 2;
			char * environment = (char *) malloc (20 * sizeof(char));
			char * envtemp = environment;
			//printf("It checks out!\n");
			while (*temp != '}') {
				*envtemp = *temp;
				envtemp++;
				//printf("%c", *temp);
				temp++;
			}
			*envtemp = '\0';
			//printf("\nEnvironment: %s\n", environment);
			char * get = getenv(environment);
			strcat(concatenation, get);
			//printf("\nconcatenation now: %s", concatenation);
			//printf("\nGet: %s", get);
			//printf("\n");

			//printf("arg now1: %s\n", arg);

			//arg++;
			while (*arg != '}') {
				arg++;
			}
			arg++;

			//printf("arg now2: %s\n", arg);

			char * newArgument = (char *) malloc (100);
			char * yolo = newArgument;
			//printf("It comes here!\n");

			while (*arg != '$' && *arg != '\0') {
				*newArgument = *arg;
				newArgument++; arg++;
			}
			*newArgument = '\0';
			//printf("newArgument: %s\n", yolo);
			strcat(concatenation, yolo);
			argument = strdup(concatenation);
		} else {
			//printf("It doesn't check out!\n");
		}
		//printf("Before\n");
		//while (*arg != '$' || *arg != '\0') {
		//	arg++;
		//}
		//printf("After\n");
		checkDollar++;
		checkDollar = strchr(checkDollar, '$');
	}

	//printf("Final Concatenation: %s\n", argument);

	_arguments[ _numOfArguments ] = argument;

	// Add NULL argument at the end
	_arguments[ _numOfArguments + 1] = NULL;

	_numOfArguments++;
}

Command::Command()
{
	// Create available space for one simple command
	_numOfAvailableSimpleCommands = 1;
	_simpleCommands = (SimpleCommand **)
		malloc( _numOfSimpleCommands * sizeof( SimpleCommand * ) );

	_numOfSimpleCommands = 0;
	_outFile = 0;
	_inFile = 0;
	_errFile = 0;
	_background = 0;
	_append = 0;
	_inCounter = 0;
	_outCounter = 0;
}

void
Command::insertSimpleCommand( SimpleCommand * simpleCommand )
{
	if ( _numOfAvailableSimpleCommands == _numOfSimpleCommands ) {
		_numOfAvailableSimpleCommands *= 2;
		_simpleCommands = (SimpleCommand **) realloc( _simpleCommands,
			 _numOfAvailableSimpleCommands * sizeof( SimpleCommand * ) );
	}

	_simpleCommands[ _numOfSimpleCommands ] = simpleCommand;
	_numOfSimpleCommands++;
}

void
Command::clear()
{
	for ( int i = 0; i < _numOfSimpleCommands; i++ ) {
		for ( int j = 0; j < _simpleCommands[ i ]->_numOfArguments; j ++ ) {
			free ( _simpleCommands[ i ]->_arguments[ j ] );
		}

		free ( _simpleCommands[ i ]->_arguments );
		free ( _simpleCommands[ i ] );
	}

	if ( _outFile ) {
		free( _outFile );
	}

	if ( _inFile ) {
		free( _inFile );
	}

	if ( _errFile ) {
		free( _errFile );
	}

	_numOfSimpleCommands = 0;
	_outFile = 0;
	_inFile = 0;
	_errFile = 0;
	_background = 0;
	_append = 0;
	_inCounter = 0;
	_outCounter = 0;
}

void
Command::print()
{
	printf("\n\n");
	printf("              COMMAND TABLE                \n");
	printf("\n");
	printf("  #   Simple Commands\n");
	printf("  --- ----------------------------------------------------------\n");

	for ( int i = 0; i < _numOfSimpleCommands; i++ ) {
		printf("  %-3d ", i );
		for ( int j = 0; j < _simpleCommands[i]->_numOfArguments; j++ ) {
			printf("\"%s\" \t", _simpleCommands[i]->_arguments[ j ] );
		}
		printf("\n");
	}

	printf( "\n\n" );
	printf( "  Output       Input        Error        Background        Append\n" );
	printf( "  ------------ ------------ ------------ ------------ ------------\n" );
	printf( "  %-12s %-12s %-12s %-12s %-12s\n", _outFile?_outFile:"default",
		_inFile?_inFile:"default", _errFile?_errFile:"default",
		_background?"YES":"NO", _append?"YES":"NO");
	printf( "\n\n" );

}

void
Command::execute()
{

	int i = 0;

	// Don't do anything if there are no simple commands
	if ( _numOfSimpleCommands == 0 ) {
		prompt();
		return;
	}

	if (!strcmp(_simpleCommands[0]->_arguments[0], "exit")) {
		printf("Good Bye!\n");
		exit(1);
	}

	if (_inCounter > 1 || _outCounter > 1) {
		printf("Ambiguous output redirect.\n");
		clear();
		prompt();
		return;
	}

	// Print contents of Command data structure
	//print();

	// Add execution here
	// For every simple command fork a new process
	// Setup i/o redirection
	// and call exec

	int tmpin = dup(0);
	int tmpout = dup(1);
	int tmperr = dup(2);

	int fdin;
	int fout;
	int ferr;

	if (_inFile) {
		fdin = open(_inFile, O_RDONLY,0600);
	} else {
		fdin = dup(tmpin);
	}

	if (_errFile) {
		if (_append) {
			ferr = open(_errFile, O_WRONLY | O_APPEND, 0600);
		} else {
			ferr = open(_errFile, O_WRONLY | O_CREAT | O_TRUNC, 0600);
		}
	} else {
		ferr = dup(tmperr);
	}
	dup2(ferr, 2);
	close(ferr);

	int ret;

	for (i = 0; i < _numOfSimpleCommands; i++) {
		dup2(fdin, 0);
		close(fdin);

		if (i == _numOfSimpleCommands - 1) {
			if (_outFile) {
				if (_append) {
					fout = open(_outFile, O_WRONLY | O_APPEND, 0600);
				} else {
					fout = open(_outFile, O_WRONLY | O_CREAT | O_TRUNC, 0600);
				}
			} else {
				fout = dup(tmpout);
			}
		} else {
			int fdpipe[2];
			pipe(fdpipe);
			fout = fdpipe[1];
			fdin = fdpipe[0];
		}

		dup2(fout, 1);
		close(fout);

		if (!strcmp(_simpleCommands[i]->_arguments[0], "setenv")) {
			int errorCheck = setenv(_simpleCommands[i]->_arguments[1], _simpleCommands[i]->_arguments[2], 1);
			if (errorCheck) {
				perror("setenv");
			}

			clear();
			prompt();
			return;
		}

		if (!strcmp(_simpleCommands[i]->_arguments[0], "unsetenv")) {
			int errorCheck = unsetenv(_simpleCommands[i]->_arguments[1]);
			if (errorCheck) {
				perror("unsetenv");
			}
		}

		if (!strcmp(_simpleCommands[i]->_arguments[0], "cd")) {
			int error;
			if (_simpleCommands[i]->_numOfArguments == 1) {
				error = chdir(getenv("HOME"));
			} else {
				error = chdir(_simpleCommands[i]->_arguments[1]);
			}

			if (error < 0) {
				perror("chdir");
			}
			clear();
			prompt();
			return;
		}

		int ret;
		ret = fork();
		if (ret == 0) {
			if (!strcmp(_simpleCommands[i]->_arguments[0], "printenv")) {
				char **env = environ;
				while (*env != NULL) {
					printf("%s\n", *env);
					env++;
				}
			}
			execvp(_simpleCommands[i]->_arguments[0], _simpleCommands[i]->_arguments);
			perror("execvp");
			_exit(1);
		} else if (ret < 0) {
			perror("fork");
			return;
		}
	}

	dup2(tmpin, 0);
	dup2(tmpout, 1);
	dup2(tmperr, 2);

	close(tmpin);
	close(tmpout);
	close(tmperr);

	if (!_background) {
		waitpid(ret, NULL, 0);
	}


	// Clear to prepare for next command
	clear();

	// Print new prompt
	prompt();
}

// Shell implementation

void
Command::prompt()
{
	if ( isatty(0) ) {
		printf("Sh-ell>");
		fflush(stdout);
	}
}

extern "C" void controlc(int sig) {
	printf("\n");
	Command::_currentCommand.prompt();
}

extern "C" void zombie(int sig) {
	int pid = wait3(0, 0, NULL);
    while(waitpid(-1, NULL, WNOHANG) > 0);
}

Command Command::_currentCommand;
SimpleCommand * Command::_currentSimpleCommand;

int yyparse(void);

main() {

	struct sigaction signalActionOne;
	signalActionOne.sa_handler = zombie;
	sigemptyset(&signalActionOne.sa_mask);
	signalActionOne.sa_flags = SA_RESTART;
	int error = sigaction(SIGCHLD, &signalActionOne, NULL );
	if ( error ) {
		perror( "sigaction" );
		exit( -1 );
	}

	struct sigaction signalActionTwo;
	signalActionTwo.sa_handler = controlc;
	sigemptyset(&signalActionTwo.sa_mask);
	signalActionTwo.sa_flags = SA_RESTART;
	error = sigaction(SIGINT, &signalActionTwo, NULL );
	if ( error ) {
		perror( "sigaction" );
		exit( -1 );
	}

	Command::_currentCommand.prompt();
	yyparse();
}
