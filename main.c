#include "helpers.c"

void child_handler(int sig)
{
    int status;
    pid_t pid;
    while ( (pid = waitpid(-1, &status, WNOHANG)) > 0) 
        ;
}

int main(int argc, char *argv[])
{
    int exec_result;
    int exit_status;
    pid_t pid;
    pid_t wait_result;
    FILE* input;
       
    if (argc == 2)
    {
        input = fopen(argv[1], "r");
        if (input == NULL)
        {
            perror(argv[1]);
            exit(1);
        }
    }
    else
    {
        assert(argc == 1);
        input = stdin;
    }

    signal(SIGCHLD, child_handler);

    while(1)
    {
        string buffer = NULL;
        size_t buf_size = 0;
        if (argc == 1)
            display_shell();
        ssize_t nbytes = getline( (char **)&buffer, &buf_size, input);
            
        // no input
        if(nbytes == -1)
        {
            free(buffer);
            break;
        }
            
        // remove new line char from buffer 
        if (buffer[nbytes - 1] == '\n')
            buffer[nbytes - 1] = '\0'; 
            
        // if the string is empty
        if (strcmp(buffer,"") == 0)
        {
            free(buffer);
            continue;
        }
            
        commandLine cmdLine;
                
        // parses the buffer from stdin and stores the result in cmdLine
        Parse(buffer, &cmdLine);
            
        // checks to see if its the special commands exit or cd
        // if neither then it takes care of other shell commands
            
        if (strcmp(cmdLine.argv[0],"exit") == 0)
        {
            // terminates the shell, prints exit status and frees all memory
            free(buffer);
            wait_result = waitpid(pid, &exit_status, 0);
            int status = WEXITSTATUS(exit_status);
            printf("exit status: %d\n",status);
            return 0;      
        }
            
            // handles changing directory
        else if (strcmp(cmdLine.argv[0],"cd") == 0)
        {
            cd(cmdLine.argv[1]);
            free(buffer);
            continue; 
        }
        else
        {
            // do all of the work here
            //print(&cmdLine); 
            
            // save the the file descriptors 
            // 0: stdin and 1: stdout
            int tmpin = dup(0);    
            int tmpout = dup(1);
        
            //open stdin if it exists, if not then then set it to the default stdin
            int fdin;
            if (cmdLine.infile)
                fdin = open(cmdLine.infile,O_RDONLY);
            else
                fdin = dup(tmpin);
                
            int fdout;
            // loops through the number of commands
            for (int i = 0; i < cmdLine.numCommands; i++)
            {
                // loops through the command array
                // redirects the input
                dup2(fdin, 0);
                close(fdin);
                
                // checks to see if its the last command and get the output ready
                if (i == cmdLine.numCommands - 1)
                {
                    // checks to see if stdout is append, out or default
                    if (cmdLine.append)
                        fdout = open(cmdLine.outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
                    else if (cmdLine.outfile)
                        fdout = open(cmdLine.outfile, O_WRONLY | O_CREAT, 0644);
                    else
                        fdout = dup(tmpout);
                }
                // not the last command so create the pipe
                else
                {
                    int fd[2];
                    pipe(fd);
                    fdout = fd[1];
                    fdin = fd[0];
                }
                       
                //redirects the output
                dup2(fdout,1);
                close(fdout);
                
                //creates the  child proccess by forking
                pid = fork();
                if(pid == 0)
                {
                    //inside child proccess
                    exec_result = execvp(cmdLine.argv[cmdLine.cmdStart[i]], &cmdLine.argv[cmdLine.cmdStart[i]]);
                    // checks if an error has occured in exec if it does then print command not found to stderr
                    if (exec_result == -1)
                    {
                        fprintf(stderr, "-bash: %s: command not found\n",cmdLine.argv[cmdLine.cmdStart[i]]); 
                        exit(EXIT_FAILURE);
                    }
                    exit(EXIT_SUCCESS); 
                }

                // restore in and out to the defaults
                dup2(tmpin,0);
                dup2(tmpout,1); 

                // checks if its not a background procces
                if (!cmdLine.background)
                {
                    //waits for the pid
                    wait_result = waitpid(pid, &exit_status, 0);
                    if (wait_result == -1)
                        error("an error occured while waitng for a proccess\n"); 
                }
            }
        }
        free(buffer);
    }
            
    printf("\n");
    return 0;
}
