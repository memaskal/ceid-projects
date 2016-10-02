#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#define TOK_BUFF_REALLOC 5
#define WHITESPACE " \t\r\n\v\f\a"

/* Function declarations*/
int execCommand(char**,int);
char**parseLine(char*, const char*, int*);
char*trimLine(char*);

/* Built in shell functions */
int closeMysh(char**);
int changedir(char**);

/* Shell function handlers*/
const int SH_COMMANDS = 2;

typedef struct {
	char *name;
	int(*call)(char**);
} Handlers;

Handlers funh[] = {
	{ "exit", &closeMysh },
	{ "cd", &changedir }
};


/**
*  main function
**/
int main(int argc, char **argv) {

	size_t bufflen = 0;
	int status = 1, numOfPipes;
	char *trim, *buffer = NULL;

	/* Main Loop */
	do {
		printf("$");
		if (getline(&buffer, &bufflen, stdin) < 2 ||								// Check if input is empty
			*(trim = trimLine(buffer)) == 0) continue;
	
		if(trim[0] == '|' || trim[strlen(trim) - 1] == '|') {
			fprintf(stderr, "Syntax error\n");
			continue;
		}
		
		char **pipeArgs = parseLine(trim, "|", &numOfPipes);
		if (--numOfPipes >= 0) {		   
			status = execCommand(pipeArgs, numOfPipes);
		} else {
			fprintf(stderr, "Syntax error\n");
		}
		free(pipeArgs);	
	} while (status);

	/* Deallocate heap memory */
	if (buffer != NULL)free(buffer);

	return EXIT_SUCCESS;
}


/**
*  Function parseLine creates a NULL terminated token array,
*  by splitting the parameter char *str on white space until
*  a pipe is found or it reaches the end of *str
**/
char **parseLine(char *str, const char* TOK_DELIM, int *len) {

	int tknum = 0, size = TOK_BUFF_REALLOC, error = 0;
	char *tmp, *next, **tokens = (char**)malloc(size * sizeof(char*));

	/* Allocate memory to token array */
	if (tokens == NULL) {
		fprintf(stderr, "Allocation error\n");
		exit(EXIT_FAILURE);
	}

	/* Split tmp into tokens */
	tmp = strtok_r(str, TOK_DELIM, &next);
	while (tmp != NULL) {
		if (strcmp(TOK_DELIM, "|") == 0 && *next == '|') {							// Two pipes in row
			error = 1;
			break;
		}		
		tokens[tknum++] = tmp;
		if (tknum >= size) {														// Reallocate extra memory to token array if needed
			size += TOK_BUFF_REALLOC;
			tokens = (char**)realloc(tokens, size * sizeof(char*));
			if (tokens == NULL) {
				fprintf(stderr, "Allocation error\n");
				exit(EXIT_FAILURE);
			}
		}
		tmp = strtok_r(NULL, TOK_DELIM, &next);
	}
	
	*len = (error)? -1 : tknum;
	tokens[tknum] = NULL;															// Null terminated token array
	return tokens;
}


/**
*  Function trimLine, removes leading/trailing
*  white space from *str
**/
char *trimLine(char *str) {

	char *end;
	while (isspace(*str)) str++;
	if (*str == 0) return str;

	end = str + strlen(str) - 1;
	while (end > str && isspace(*end)) end--;

	*(end + 1) = '\0';
	return str;
}


/**
*  Function closeMysh, performs the exit command
**/
int closeMysh(char **argv) {
	return 0;
}


/**
*  Function changedir, performs the cd command
**/
int changedir(char **argv) {

	if (argv[1] == NULL) {
		fprintf(stderr, "You must supply a path to cd\n");
	}
	else if (chdir(argv[1]) < 0) {
		perror("cd error");
	}
	return 1;
}


/**
*  Function execCommand, executes #argc commands 
*  supplied by the argv array
**/
int execCommand(char **argv, int argc) {

	int i, j, fds[argc][2];
	char **args;
	
	/* Create the appropriate pipes */
	for (i = 0; i < argc; ++i) {
		if (pipe(fds[i]) < 0) {																
			perror("pipe");
			exit(EXIT_FAILURE);
		}
	}
	
	/* Run for multiple commands */
	for (i = 0; i <= argc; ++i, ++argv) {
		/* Split argv to white spaces */
		args = parseLine(*argv, WHITESPACE, &j); 
		
		if (*args != NULL) {			
			/* Search for built in functions exit-cd */
			if (argc == 0) {
				for (j = 0; j < SH_COMMANDS; ++j) {
					if (strcmp(*args, funh[j].name) == 0)
						return funh[j].call(args);
				}
			}		
			/* Create a child to execute each command */
			switch(fork()) {
			case 0:																			// Child process
				if (i) dup2(fds[i-1][0], STDIN_FILENO);										// If not first, copy last output to input
				if (i < argc) dup2(fds[i][1], STDOUT_FILENO);								// If not last, copy output to fd[1]
				for(j = 0; j < argc; ++j) {
					close(fds[j][0]);
					close(fds[j][1]);
				}			
				execvp(*args, args);
				fprintf(stderr, "-mysh: %s: command not found\n", *args);					// Error at exec, exit
				exit(EXIT_FAILURE);
			case -1:																		// Error in forking
				fprintf(stderr, "Error while forking\n");
				exit(EXIT_FAILURE);
			}
		} else {
			fprintf(stderr, "Null Command\n");
		}
		free(args);
	}
	
	/* Close all open file descriptors */
	for (i = 0; i < argc; ++i) {
		close(fds[i][1]);
		close(fds[i][0]);
	}
	/* Wait for processes to end */
	while(waitpid(-1, NULL, 0) > 0);
	return 1;
}
