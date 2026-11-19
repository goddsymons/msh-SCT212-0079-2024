#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    char *line = NULL;
    size_t len = 0;

    for (;;) {
        printf("msh> ");
        fflush(stdout);  // displays the prompt  immediately

        ssize_t nread = getline(&line, &len, stdin);

        if (nread == -1) {  // Checks for EOF \input error
            printf("\n");
            break;
        }

        printf("You typed: %s", line);  // Displays the user's input
    }

    free(line);  // This releases dynamically allocated memory
    return 0;
}
