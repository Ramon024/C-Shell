#include "parser.h"

static int isShellSymb(char c) {
    return (c)=='|' || (c)=='<' || (c)=='>' || (c) == '&';
}
static int isSeparator(char c) {
    return isspace(c) || isShellSymb(c);
}

int Parse(string line, commandLine* sc)
{
    int argc = 0;

    assert(strlen(line) < MAX_LINE);
    strcpy(sc->buf, line);
    line = sc->buf;

    sc->infile = sc->outfile = sc->argv[0] = NULL;
    sc->append = sc->numCommands = 0;
    sc->cmdStart[0] = 0;    /* the 0th command starts at argv[0] */
    sc->background = 0;

    while(argc < MAX_ARGS-1 && sc->numCommands < MAX_COMMANDS)
    {
    
        while(isspace(*line)) 
            ++line;

        if(*line == '\0')   
        
        switch(*line)
        {
            case '|':
                *line++ = '\0';
                sc->argv[argc++] = NULL;   
                sc->cmdStart[++sc->numCommands] = argc;
                continue;  

            case '<':
                *line++ = '\0';
                while(isspace(*line))
                    ++line;
                sc->infile = line;
                break;

            case '>':
                *line++ = '\0'; 
	            if(*line == '>') 
	            {
		            sc->append = 1;
		            line++;
	            }
                while(isspace(*line)) 
                    ++line;
                sc->outfile = line;
                break;

            case '&':
                *line++ = '\0';
                while(isspace(*line))  
                    ++line;
                sc->background = 1;
                break;

            default:    
                sc->argv[argc++] = line;
                break;
        }
        

        while(*line && !isSeparator(*line))  
            ++line;

        if(*line == '\0')    
            break;
	    else if(isShellSymb(*line))
	        continue;	
        else
            *line++ = '\0';
    }

    sc->argv[argc] = NULL; 
    sc->argv[argc + 1] = NULL;  
    if(argc > 0)
        ++sc->numCommands;

    return 0;
}
