#include "reader.h"

#include <stdio.h>
#include <stdlib.h>

#define MAX_LEN_STR 256

char *read_input_data() {
    char *input_string = malloc(MAX_LEN_STR);
    if (!input_string) return NULL;
    int len = 0;
    char s;
    while (scanf("%c", &s) == 1 && s != '\n' && len < MAX_LEN_STR - 1) {
        input_string[len++] = s;
    }
    input_string[len] = '\0';
    return input_string;
}

int is_space(char s) { return s == ' ' || s == '\t'; }

int is_digit(char s) { return s >= '0' && s <= '9'; }

int is_alpha(char s) { return (s >= 'A' && s <= 'Z') || (s >= 'a' && s <= 'z'); }