#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "parse.c"

#define MAX_LEN 1000
#define MAX_ARGS 100
#define MAX_ENV_VARS 100

// Main shell loop
int main(int argc, char argv[]) {
    char line[MAX_LEN];

    while (1) {
        printf("xsh# ");
        fgets(line, MAX_LEN, stdin);
        if(!strcmp(line, "exit\n")) {
            break;
        }
        //printf("You entered: %s", line);

        // Split command line into tokens and execute
        parse_and_execute(line);
    }

    return 0;
}
