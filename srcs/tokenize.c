#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ARGS 10
#define MAX_TOKEN_LEN 20

int parse_input(char *input, char **args, int *background) {
    char *token;
    int i = 0;
    int in_quotes = 0;

    token = strtok(input, " \t\n");
    while (token != NULL) {
        if (i >= MAX_ARGS) {
            fprintf(stderr, "too many arguments\n");
            return -1;
        }

        if (strlen(token) >= MAX_TOKEN_LEN) {
            fprintf(stderr, "argument too long\n");
            return -1;
        }

        if (*token == '\"') {
            in_quotes = 1;
            token++;
        }

        if (*(token + strlen(token) - 1) == '\"') {
            *(token + strlen(token) - 1) = '\0';
            in_quotes = 0;
        }

        if (in_quotes) {
            strcat(args[i], " ");
            strcat(args[i], token);
        } else {
            args[i] = token;
            i++;
        }

        if (*token == '|') {
            args[i] = NULL;
            return i;
        }

        if (*token == '>') {
            *background = 0;
            return i;
        }

        if (*token == '<') {
            *background = 0;
            return i;
        }

        if (*token == '&') {
            *background = 1;
            return i;
        }

        token = strtok(NULL, " \t\n");
    }

    return i;
}
