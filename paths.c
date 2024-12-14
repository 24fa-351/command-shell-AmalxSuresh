#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>
#include <fcntl.h>

void execute_external_command(char *words[]);
char *find_command_path(char *command);


void execute_external_command(char *words[]) {
    char *cmd_path = find_command_path(words[0]);
    if (cmd_path == NULL) {
        fprintf(stderr, "Command not found: %sxx\n", words[0]);
        return;
    }

    pid_t pid = fork();
    if (pid == 0) {
        execve(cmd_path, words, NULL);  
        perror("exec failed");  
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        int status;
        waitpid(pid, &status, 0);  
    } else {
        perror("fork failed");
    }

    free(cmd_path); 
}

char *find_command_path(char *command) {
    
    char *path = getenv("PATH");
    if (!path) {
        fprintf(stderr, "PATH environment variable is not set.\n");
        return NULL;
    }

    char *path_copy = strdup(path);
    char *dir = strtok(path_copy, ":");
    while (dir != NULL) {
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", dir, command);
        if (access(full_path, X_OK) == 0) {
            free(path_copy);
            return strdup(full_path);  
        }
        dir = strtok(NULL, ":");
    }
    free(path_copy);
    return NULL;
}