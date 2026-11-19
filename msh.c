#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
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

/* Checks whether the entered command is built-in.
it  Returns -1 to exit, 
1 if handled, 
or 0 if it is not built-in.
 */
int handle_builtin(char **args)
{
    if (strcmp(args[0], "exit") == 0) {
        return -1;  // Signals  to exit
    }

    if (strcmp(args[0], "cd") == 0) {
        const char *dir = args[1];

        if (dir == NULL)
            dir = getenv("HOME");   

        if (dir == NULL) {
            fprintf(stderr, "msh: cd: HOME not set\n");
            last_status = 1;
        } else if (chdir(dir) != 0) {
            perror("msh: cd");
            last_status = 1;
        } else {
            last_status = 0;
        }

        return 1;
    }

    if (strcmp(args[0], "help") == 0) {
        printf("msh - a minimal shell\n");
        printf("Built-in commands: cd [dir], help, exit\n");
        printf("All other commands are run as external programs.\n");

        last_status = 0;
        return 1;
    }

    return 0;  // Not a built-in command
}


// Collects background processes that have finished.
 
void reap_zombies(void)
{
    int status;
    pid_t pid;

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        printf("[done] PID %d\n", pid);
    }
}

    void run_command(char **args, int bg)
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

        if (bg){

            printf("[bg] PID %d\n", pid);  // Displays the background process ID
            return;
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
       reap_zombies();  //Checks for completed background processes before displaying a new prompt
       printf("msh> ");  // This displays the shell prompt

        fflush(stdout);  // displays the prompt  immediately

        ssize_t nread = getline(&line, &len, stdin);

        if (nread == -1) {  // Checks for EOF \input error.Stops when there is no more input
            printf("\n");
            break;
        }


        int argc = parse_line(line, args);  // Parses the input line into arguments

        if (argc == 0) {
            continue;  // If no command was entered, prompt again
        }

        //check whether the command should run in the background
        int bg = 0;

        if (strcmp(args[argc - 1], "&") == 0) {
            bg = 1;  
            args[argc - 1] = NULL;  // Removes & from the arguments. to ensure execvp doesn't treat it as a command 
            argc--;  // Decreases the argument count

        }
        if (argc == 0) {
            continue;  // If no command was entered after removing &, prompt again
        }

        int builtin = handle_builtin(args);  // Checks if the command is built-in

        if (builtin == -1) {
            break;  // Exit the shell 
        }
        if (builtin == 0) {
            run_command(args, bg);  // Executes external commands
            
        }
    
    }
    free(line);  // This releases dynamically allocated memory
    return 0;
}
