#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "math_func.h"
#include "reader.h"
#include "token.h"

#define WIDTH 80
#define HEIGHT 25
#define X_MIN 0.0
#define X_MAX (4 * M_PI)
#define Y_MIN -1.0
#define Y_MAX 1.0

void set_UI(Token rpn[1024], int rpn_size);

int main() {
    char *input_str = read_input_data();
    if (!input_str) {
        printf("n/a");
        return 0;
    }

    Token tokens[1024];
    int token_count = str_to_token(input_str, tokens);

    if (token_count < 1) {
        printf("n/a\n");
        free(input_str);
        return 0;
    }
    free(input_str);

    Token rpn[1024];
    int rpn_size = 0;
    to_rpn(tokens, token_count, rpn, &rpn_size);
    set_UI(rpn, rpn_size);

    return 0;
}

void set_UI(Token rpn[1024], int rpn_size) {
    char screen[HEIGHT][WIDTH];
    for (int i = 0; i < HEIGHT; i++)
        for (int j = 0; j < WIDTH; j++) screen[i][j] = '.';

    int error_flag = 0;
    for (int col = 0; col < WIDTH; ++col) {
        double x = X_MIN + (X_MAX - X_MIN) * col / (WIDTH - 1);
        double y = -evaluate_rpn(rpn, rpn_size, x, &error_flag);

        if (error_flag) {
            printf("n/a\n");
            return;
        }
        int row = (int)round((Y_MAX - y) / (Y_MAX - Y_MIN) * (HEIGHT - 1));
        if (row >= 0 && row < HEIGHT) {
            screen[row][col] = '*';
        }
    }

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) putchar(screen[i][j]);
        putchar('\n');
    }
}
