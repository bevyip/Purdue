
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
#include <sys/stat.h>
#include <fcntl.h>
#include <regex.h>
#include <pwd.h>
#include "command.h"

SimpleCommand::SimpleCommand()
{
	// Create available space for 5 arguments
	_numOfAvailableArguments = 5;
	_numOfArguments = 0;
	_arguments = (char **) malloc( _numOfAvailableArguments * sizeof( char * ) );
}

char * SimpleCommand::checkExpansion(char * argument) {
	char * arg = strdup(argument);
	char * checkDollar = strchr(arg, '$');
	char * checkBraces = strchr(arg, '{');

	char * replace = (char *) malloc (sizeof(argument) + 50);
	char * temp = replace;

	if (checkDollar && checkBraces) {
		while (*arg != '$') {
			*temp = *arg;
			temp++; arg++;
		}
		*temp = '\0';

		while (checkDollar) {
			if (checkDollar[1] == '{' && checkDollar[2] != '}') {
				char * temporary = checkDollar + 2;
				char * env = (char *) malloc (20);
				char * envtemp = env;

				while (*temporary != '}') {
					*envtemp = *temporary;
					envtemp++; temporary++;
				}
				*envtemp = '\0';

				char * get = getenv(env);

				strcat(replace, get);

				while (*(arg-1) != '}') arg++;

				char * buf = (char *) malloc (20);
				char * tbuf = buf;

				while (*arg != '$' && *arg) {
					*tbuf = *arg;
					tbuf++; arg++;
				}
				*tbuf = '\0';
				strcat(replace, buf);
			}
			checkDollar++;
			checkDollar = strchr(checkDollar, '$');
		}
		argument = strdup(replace);
		return argument;
	}
	return NULL;
}

char * SimpleCommand::tilde(char * argument) {
	if (argument[0] == '~') {
		
		if (strlen(argument) == 1) {
		
			argument = strdup(getenv("HOME"));
			return argument;
		
		} else {

			if (argument[1] == '/') {
				char * dir = strdup(getenv("HOME"));
				argument++;
				argument = strcat(dir, argument);
				return argument;
			}

			char * nArgument = (char *) malloc (strlen(argument) + 20);
			char * uName = (char *) malloc (50);
			char * user = uName;
			char * temp = argument;

			temp++;

			while (*temp != '/' && *temp) *(uName++) = *(temp++);
			*uName = '\0';


			if (*temp) {

				nArgument = strcat(getpwnam(user)->pw_dir, temp);
				argument = strdup(nArgument);
				return argument;

			} else {
				
				argument = strdup(getpwnam(user)->pw_dir);
				return argument;

			}
		}

	}

	return NULL;
}

void
SimpleCommand::insertArgument( char * argument )
{
	if ( _numOfAvailableArguments == _numOfArguments  + 1 ) {
		// Double the available space
		_numOfAvailableArguments *= 2;
		_arguments = (char **) realloc( _arguments,
				  _numOfAvailableArguments * sizeof( char * ) );
	}

	// Variable expansion implementation
	char * exp = checkExpansion(argument);

	if (exp) argument = strdup(exp);
	
	exp = tilde(argument);

	if (exp) argument = strdup(exp);

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
Command:: clear()
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
	}

	printf( "\n\n" );
	printf( "  Output       Input        Error        Background\n" );
	printf( "  ------------ ------------ ------------ ------------\n" );
	printf( "  %-12s %-12s %-12s %-12s\n", _outFile?_outFile:"default",
		_inFile?_inFile:"default", _errFile?_errFile:"default",
		_background?"YES":"NO");
	printf( "\n\n" );

}

int Command::builtInCheck(int i) {

	// Check if a builtin command is called in the shell

	if (!strcmp(_simpleCommands[i]->_arguments[0], "setenv")) {
		int error = setenv(_simpleCommands[i]->_arguments[1], _simpleCommands[i]->_arguments[2], 1);
		if (error)
			perror("setenv");

		clear();
		prompt();
		return 1;
	}

	if (!strcmp(_simpleCommands[i]->_arguments[0], "unsetenv")) {
		int error = unsetenv(_simpleCommands[i]->_arguments[1]);
		if (error)
			perror("unsetenv");

		clear();
		prompt();
		return 1;
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
		return 1;
	}

	return 0;
}

void
Command::execute()
{
	// Don't do anything if there are no simple commands
	if ( _numOfSimpleCommands == 0 ) {
		prompt();
		return;
	}

	// Check if there is the exit command is called
	if (!strcmp(_simpleCommands[0]->_arguments[0], "exit") || !strcmp(_simpleCommands[0]->_arguments[0], "bye")) {
		//printf("Bye Bye!\n");
		exit(1);
	}

	// Check for ambiguous output redirect
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
	int fdout;
	int fderr;
	int ret;

	if (_inFile) {
		fdin = open(_inFile, O_RDONLY);
	} else {
		fdin = dup(tmpin);
	}

	if (_errFile) {
		if (_append) {
			fderr = open(_errFile, O_WRONLY | O_CREAT | O_APPEND, 0600);
		} else {
			fderr = open(_errFile, O_WRONLY | O_CREAT | O_TRUNC, 0600);
		}
	} else {
		fderr = dup(tmperr);
	}
	dup2(fderr, 2);
	close(fderr);

	for (int i = 0; i < _numOfSimpleCommands; i++) {

		// Change the file descriptor for the input and then close the file descriptor
		dup2(fdin, 0);
		close(fdin);

		// Check for builtins
		if (builtInCheck(i)) return;

		// Check if the command is the last command
		// If it is then redirect output to file or stdout instead of a pipe
		if (i == _numOfSimpleCommands - 1) {

			if (_outFile) {
				if (_append) {
					fdout = open(_outFile, O_WRONLY | O_CREAT | O_APPEND, 0600);
				} else {
					fdout = open(_outFile, O_WRONLY | O_CREAT | O_TRUNC, 0600);
				}
			} else {
				fdout = dup(tmpout);
			}

		} else {

			//If the command isn't the last command, then make a pipe
			int fdpipe[2];
			pipe(fdpipe);

			//Make the fdin and fdout variables to point to the sides of the pipe
			fdout = fdpipe[1];
			fdin = fdpipe[0];

		}

		//Redirect the output
		dup2(fdout, 1);
		close(fdout);

		//Execute the command
		ret = fork();

		if (ret == 0) {

			// Child Process

			// Check if the printenv function is called
			if (!strcmp(_simpleCommands[i]->_arguments[0], "printenv")) {
				char ** env = environ;
				while (*env) {
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

	if(!_background) {
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
	if (isatty(0)) {
		printf(" > ");
		fflush(stdout);
	}
}

Command Command::_currentCommand;
SimpleCommand * Command::_currentSimpleCommand;

int yyparse(void);

extern "C" void controlC(int sig) {
	printf("\n");
	Command::_currentCommand.prompt();
}

extern "C" void zombie(int sig) {
	int pid = wait3(0, 0, NULL);
	while (waitpid(-1, NULL, WNOHANG) > 0);
}

main() {

	int error;

	//Control-C
	struct sigaction sa1;
	sa1.sa_handler = controlC;
	sigemptyset(&sa1.sa_mask);
	sa1.sa_flags = SA_RESTART;
	error = sigaction(SIGINT, &sa1, NULL);
	if (error) {
		perror("sigaction");
		exit(-1);
	}

	//Zombie Process
	struct sigaction sa2;
	sa2.sa_handler = zombie;
	sigemptyset(&sa2.sa_mask);
	sa2.sa_flags = SA_RESTART;
	error =  sigaction(SIGCHLD, &sa2, NULL);
	if (error) {
		perror("sigaction");
		exit(-1);
	}


	Command::_currentCommand.prompt();
	yyparse();

}
