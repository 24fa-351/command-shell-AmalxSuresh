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
void pipe_it(char *words[]);

void execute_external_command(char *words[]) {
    //find and print the absolute path of the command
    char *cmd_path = find_command_path(words[0]);
    printf("%s\n", cmd_path);
    if (cmd_path == NULL) {
        fprintf(stderr, "Command not found: %s\n", words[0]);
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

//Find absolute path method
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

//input and output redirect commnands
void pipe_it(char *words[]) {
    char *input_file = NULL;
    char *output_file = NULL;

    for (int ix = 0; words[ix] != NULL; ix++) {
        if (strcmp(words[ix], "<") == 0) {
            input_file = words[ix + 1];
            words[ix] = NULL;  
        } else if (strcmp(words[ix], ">") == 0) {
            output_file = words[ix + 1];
            words[ix] = NULL;  
        }
    }

    pid_t pid = fork();
    if (pid == 0) {
        //handle input redirect
        if (input_file) {
            int fd_input = open(input_file, O_RDONLY);
            if (fd_input < 0) {
                perror("Failed to open input file");
                exit(EXIT_FAILURE);
            }
            dup2(fd_input, STDIN_FILENO);
            close(fd_input);
        }
        //handle output redirect
        if (output_file) {
            int fd_output = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd_output < 0) {
                perror("Failed to open output file");
                exit(EXIT_FAILURE);
            }
            dup2(fd_output, STDOUT_FILENO);
            close(fd_output);
        }
        execute_external_command(words);
        exit(EXIT_SUCCESS);
    } else if (pid > 0) {
        wait(NULL);  
    } else {
        perror("fork failed");
    }
}
