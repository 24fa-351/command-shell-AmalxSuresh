#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>
#include <fcntl.h>
#include "paths.c"
#include "builtin.c"

void parse_and_execute(char *line);
void add_character_to_string(char *str, char c);
void split(char *cmd, char *words[], char delimiter);

void parse_and_execute(char *line) {
    char *words[1000];
    strip_newline(line);
    split(line, words, ' ');
    if(words[0] == NULL) {
        return;
    }

    if(strcmp(words[0], "set") == 0 && words[1] != NULL && words[2] != NULL) {
        set_env_var(words[1], words[2]);
    }
    else if(strcmp(words[0], "unset") == 0 && words[1] != NULL) {
        unset_env_var(words[1]);
    }
    else if(strcmp(words[0], "echo") == 0 && words[1] != NULL) {
        echo_with_expansion(line + 5);
    }
    else if(strcmp(words[0], "pwd") == 0) {
        print_directory();
    }
    else if(strcmp(words[0], "cd") == 0) {
        change_directory(words[1]);
    }
    else {
        execute_external_command(words);
    }


    for(int ix = 0; words[ix] != NULL; ix++) {
        
    }
    
}
void add_character_to_string(char *str, char c) {
    int len = strlen(str);
    str[len] = c;
    str[len + 1] = '\0';
}

void split(char *cmd, char *words[], char delimiter) {
    int word_count = 0;
    char *next_char = cmd;
    char current_word[1000];
    strcpy(current_word, "");

    while(*next_char != '\0') {
        if(*next_char == delimiter) {
            words[word_count++] = strdup(current_word);
            strcpy(current_word, "");
        }
        else {
            add_character_to_string(current_word, *next_char);
        }
        ++next_char;
    }
    words[word_count++] = strdup(current_word);

    words[word_count] = NULL;
}

void strip_newline(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

