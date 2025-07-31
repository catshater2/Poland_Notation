#ifndef TOKEN_H
#define TOKEN_H

#define TOKEN_NUMBER 1
#define TOKEN_ARGUMENT 2
#define TOKEN_OPERATOR 3
#define TOKEN_FUNCTION 4
#define TOKEN_LEFT_PAREN 5
#define TOKEN_RIGHT_PAREN 6

#define MAX_LEN_TOKEN 100

typedef struct {
    int type;
    double value;
    char str[10];
} Token;

int to_rpn(Token *tokens, int token_count, Token *output, int *output_size);
void shunting_yard(const Token *infix, int infix_size, Token *output, int *output_size);
double evaluate_rpn(const Token *tokens, int size, double x, int *error_flag);
int str_to_token(char *str, Token *cnt_token);

#endif