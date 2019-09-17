#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdio.h>
#include <assert.h>
#include <sys/wait.h>
#include <sys/types.h>

#ifndef PARSER_H
#define PARSER_H

typedef char* string;
#define MAX_LINE 10240
#define MAX_COMMANDS 32
#define MAX_ARGS 32

typedef struct commandline {
    
    char buf[MAX_LINE];
    int numCommands;
    int cmdStart[MAX_COMMANDS];
    string argv[ (MAX_ARGS+1) * MAX_COMMANDS + 1];
    string infile, outfile;
    char append;
    int background;

} commandLine;

int Parse(string line, commandLine* sc);

#endif
