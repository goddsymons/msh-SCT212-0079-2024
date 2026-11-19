#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

//
#include <sys/wait.h>
#include <unistd.h>

#define MAX_ARGS 128 // this limits the number of arguments

/*This splits the input line into individual words using space and tabs.
Then returns the number of arguments thats found then stores them in args[]*/

int parse_line(char*line, char **args)
{
    int count = 0;
    char *token = strtok(line, " \t\n");  // Splits the line into tokens

    //this keeps extratcing the words until the limit is reached
    while (token != NULL && count < MAX_ARGS - 1)//or there are no more tokens
     {
        args[count++] = token;  // tokens stored in args[]
        token = strtok(NULL, " \t\n");  // Gets the next token
    }

    args[count] = NULL;  //This marks the end of the arguments list
    return count;
}
    int last_status =0;

    void run_command(char **args)
    {
        pid_t pid = fork();  // Creates a new process

        if (pid < 0) {  // Checks for fork error
            perror("msh: fork");
            return;
        }
        // Child process
        if (pid == 0) {
            execvp(args[0], args);  // Executes the command

            perror(args[0]);  // If execvp evp fails, this run

            exit(127);
        }

            int status;

            if (waitpid(pid, &status, 0) == -1) {  // Waits for the child process to finish
                perror("msh: waitpid");
                return;
        }

        if (WIFEXITED(status)) {  // Checks if the child process exited normally
            last_status = WEXITSTATUS(status);  // Gets the exit status of the child process
        } else if (WIFSIGNALED(status)) {  // Checks if the child process was terminated by a signal
            last_status = 128 + WTERMSIG(status);  // Gets the signal number            last_status = -1;  // Indicates abnormal termination
        }
}

int main(void)
{
    char *line = NULL;
    size_t len = 0;
    char *args[MAX_ARGS];  // This will hold the parsed arguments

    for (;;) {
        printf("msh> ");
        fflush(stdout);  // displays the prompt  immediately

        ssize_t nread = getline(&line, &len, stdin);

        if (nread == -1) {  // Checks for EOF \input error.Stops when there is no more input
            printf("\n");
            break;
        }


        int argc = parse_line(line, args);  // Parses the input line into arguments

        if (argc == 0) //ignores blank lines and dsiplays the prompt again
            continue;

        run_command(args);  // Executes the command with the parsed arguments
        }
    

    free(line);  // This releases dynamically allocated memory
    return 0;
}
