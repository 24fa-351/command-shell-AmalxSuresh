#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>
#include <fcntl.h>

//functions for builtin commands
void set_env_var(char *name, char *value);
void unset_env_var(char *name);
void echo_with_expansion(char *line);
void strip_newline(char *str);
void print_directory();
void change_directory(char *name);

void change_directory(char *name) { 
    if(name == NULL || strcmp(name, "~") == 0) { 
        char *home = getenv("HOME");
        if(home != NULL) {
            if(chdir(home) != 0) {
                perror("cd failed");
            }
        } else {
            fprintf(stderr, "HOME not set\n");
        }
    }
    else {
        if(chdir(name) != 0) {
            perror("cd failed");
        }
    }
}

void print_directory() {
    char cwd[1000];
    if(getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);
    }
    else {
        perror("getcwd failed");
    }
}


void set_env_var(char *name, char *value) {
    if (setenv(name, value, 1) != 0) { 
        perror("setenv");
    }
}

void unset_env_var(char *name) {
    if (unsetenv(name) != 0) {
        perror("unsetenv");  
    } else {
        printf("Environment variable '%s' has been unset.\n", name);
    }
}

void echo_with_expansion(char *line) {
    char *variable = line;
    char variable_name[1000];
    char *value;

    while (*variable != '\0') {
        if (*variable == '$') {
            variable++;  
            int ix = 0;

            // Extract variable name
            while (*variable != '\0' && (isalnum(*variable) || *variable == '_')) {
                variable_name[ix++] = *variable++;
            }
            variable_name[ix] = '\0';

            // Get the variable value
            value = getenv(variable_name);
            if (value != NULL) {
                printf("%s", value);
            }
        } else {
            putchar(*variable);
            variable++;
        }
    }
    printf("\n");
}
