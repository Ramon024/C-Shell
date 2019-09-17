#include "helpers.h"

void display_shell()
{   
    printf("? ");
}

void error(string msg)
{
    fprintf(stderr, msg);
    exit(EXIT_FAILURE);
}

void print(commandLine* cl)
{
    int num = cl->numCommands;
    printf("%d: ", num);

    if (cl->infile)
        printf("< '%s' ", cl->infile);

    for (int i = 0; i < num; i++)
    {
        for (int j = cl->cmdStart[i]; cl->argv[j] != NULL; j++)
            printf("'%s' ", cl->argv[j]);
        if (i < num - 1)
            printf("| ");
    }

    if (cl->append)
    {
        assert(cl->outfile);
        printf(">");
    }

    if (cl->outfile)
        printf(">'%s' ", cl->outfile);

    printf("\n");
}

void cd(string path)
{
    if(chdir(path) == 0)
    {
        char s[100];
        getcwd(s,100);
    }
    else
        fprintf(stderr,"Directory does not exist\n");
}
