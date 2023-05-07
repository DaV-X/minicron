#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <syslog.h>

#include "executor.h"

void execute(char *task, int mode, char *output_file) {
    char **commands;
    int num_commands;
    int i;
    int fds[2];
    pid_t pid;

    parse_command(task, &commands, &num_commands);

    
    // wykonanie kolejnych komend przy użyciu potoku
    for (i = 0; i < num_commands; i++) {
        // Create pipe
        //pipe (fds);
        if (fds[0] == NULL || fds[1] == NULL) {
            //syslog(LOG_DEBUG, "Pipe create failed");
            //exit(EXIT_FAILURE);
        }
        // Fork new process
        pid = fork();
        if (pid == (pid_t) 0) {
            /* This is the child process. Close our copy of the write end of the file descriptor. */
            //close (fds[1]);
            /* Connect the read end of the pipe to standard input. */
            //dup2 (fds[0], STDIN_FILENO);
            /* Replace the child process with the "sort” program. */
            //execlp ("sort", "sort", NULL);
            syslog(LOG_DEBUG, "Child");
            exit(EXIT_FAILURE);
        }
        else {
            /* This is the parent process. */
            //FILE* stream;
            /* Close our copy of the read end of the file descriptor. */
            //close (fds[0]);
            /* Convert the write file descriptor to a FILE object, and write to it. */
            //stream = fdopen (fds[1], "w");
            //fflush (stream);
            //close (fds[1]);
            /* Wait for the child process to finish. */
            syslog(LOG_DEBUG, "Wait %d",pid);
            waitpid (pid, NULL, 0);
        }
        //close(fds[0]);
        //close(fds[1]);
        //syslog(LOG_DEBUG, "One");
    }

    free(commands);
}

void parse_command(char *input, char ***commands, int *num_commands) {
    char *token;
    char *saveptr;
    int i;

    // podział wejścia na komendy oddzielone |
    *num_commands = 1;
    for (i = 0; input[i] != '\0'; i++) {
        if (input[i] == '|') {
            (*num_commands)++;
        }
    }
    *commands = malloc((*num_commands) * sizeof(char *));
    i = 0;
    token = strtok_r(input, "|", &saveptr);
    while (token != NULL) {
        (*commands)[i] = token;
        i++;
        token = strtok_r(NULL, "|", &saveptr);
    }
}