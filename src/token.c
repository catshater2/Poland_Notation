#include "token.h"

#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "math_func.h"
#include "reader.h"
#include "stack.h"

int priority(Token t) {
    if (t.type != TOKEN_OPERATOR) return 0;
    if (strcmp(t.str, "+") == 0 || strcmp(t.str, "-") == 0) return 1;
    if (strcmp(t.str, "*") == 0 || strcmp(t.str, "/") == 0) return 2;
    if (strcmp(t.str, "^") == 0) return 3;

    return 0;
}

int is_right_associative(Token t) {
    if (strcmp(t.str, "^") == 0) return 1;
    return 0;
}

void shunting_yard(const Token *infix, int infix_size, Token *output, int *output_size) {
    Stack *op_stack = init_stack();
    *output_size = 0;

    for (int i = 0; i < infix_size; i++) {
        Token t = infix[i];
        if (t.type == TOKEN_NUMBER || t.type == TOKEN_ARGUMENT) {
            output[(*output_size)++] = t;
        } else if (t.type == TOKEN_FUNCTION) {
            push(op_stack, t);
        } else if (t.type == TOKEN_OPERATOR) {
            Token top_op;
            while (!is_empty(op_stack) && peek(op_stack, &top_op) &&
                   ((priority(top_op) > priority(t)) ||
                    (priority(top_op) == priority(t) && !is_right_associative(t))) &&
                   top_op.type != TOKEN_LEFT_PAREN) {
                pop(op_stack, &top_op);
                output[(*output_size)++] = top_op;
            }
            push(op_stack, t);
        } else if (t.type == TOKEN_LEFT_PAREN) {
            push(op_stack, t);
        } else if (t.type == TOKEN_RIGHT_PAREN) {
            Token top_op;
            while (!is_empty(op_stack) && peek(op_stack, &top_op) && top_op.type != TOKEN_LEFT_PAREN) {
                pop(op_stack, &top_op);
                output[(*output_size)++] = top_op;
            }
            if (!is_empty(op_stack)) pop(op_stack, &top_op);
        }
    }
    Token t;
    while (!is_empty(op_stack) && pop(op_stack, &t)) {
        if (t.type == TOKEN_LEFT_PAREN || t.type == TOKEN_RIGHT_PAREN) {
        }
        output[(*output_size)++] = t;
    }
    destroy_stack(op_stack);
}

int to_rpn(Token *tokens, int token_count, Token *output, int *output_size) {
    shunting_yard(tokens, token_count, output, output_size);
    return 0;
}

double evaluate_rpn(const Token *tokens, int size, double x, int *error_flag) {
    *error_flag = 0;
    double stack[MAX_LEN_TOKEN];
    int stack_top = -1;

    for (int i = 0; i < size; i++) {
        Token t = tokens[i];
        if (t.type == TOKEN_NUMBER) {
            stack[++stack_top] = t.value;
        } else if (t.type == TOKEN_ARGUMENT) {
            stack[++stack_top] = x;
        } else if (t.type == TOKEN_OPERATOR) {
            if (stack_top < 1) {
                *error_flag = 1;
                return 0;
            }
            double b = stack[stack_top--];
            double a = stack[stack_top--];

            switch (t.str[0]) {
                case '+':
                    stack[++stack_top] = add(a, b);
                    break;
                case '-':
                    stack[++stack_top] = subtract(a, b);
                    break;
                case '*':
                    stack[++stack_top] = multiply(a, b);
                    break;
                case '/':
                    if (b == 0) {
                        *error_flag = 1;
                        return 0;
                    }
                    stack[++stack_top] = divide(a, b);
                    break;
                case '^':
                    stack[++stack_top] = power(a, b);
                    break;
                default:
                    break;
            }
        } else if (t.type == TOKEN_FUNCTION) {
            if (stack_top < 0) {
                *error_flag = 1;
                return 0;
            }
            double a = stack[stack_top--];

            if (strcmp(t.str, "sin") == 0)
                stack[++stack_top] = sin(a);
            else if (strcmp(t.str, "cos") == 0)
                stack[++stack_top] = cos(a);
            else if (strcmp(t.str, "tan") == 0)
                stack[++stack_top] = tan(a);
            else if (strcmp(t.str, "log") == 0)
                stack[++stack_top] = log(a);
            else if (strcmp(t.str, "ln") == 0)
                stack[++stack_top] = log10(a);
            else if (strcmp(t.str, "sqrt") == 0)
                stack[++stack_top] = sqrt(a);
            else if (strcmp(t.str, "ctg") == 0)
                stack[++stack_top] = 1 / tan(a);
            else if (strcmp(t.str, "exp") == 0)
                stack[++stack_top] = exp(a);
            else {
                *error_flag = 1;
                return 0;
            }
        }
    }

    if (stack_top != 0) {
        *error_flag = 1;
        return 0;
    }
    return stack[0];
}

int str_to_token(char *str, Token *cnt_token) {
    int iter = 0, pos = 0;
    while (str[pos] != '\0' && iter < MAX_LEN_TOKEN) {
        if (is_space(str[pos]) == 1) pos++;

        if (is_digit(str[pos]) == 1 || str[pos] == '.') {
            char tmp[32] = {0};
            int len = 0;

            while (is_digit(str[pos]) == 1 || str[pos] == '.') tmp[len++] = str[pos++];
            tmp[len] = '\0';

            cnt_token[iter].type = TOKEN_NUMBER;
            cnt_token[iter].value = atof(tmp);
            strcpy(cnt_token[iter++].str, tmp);
        } else if (is_alpha(str[pos]) == 1) {
            char tmp[16] = {0};
            int len = 0;

            while (is_alpha(str[pos]) == 1) tmp[len++] = str[pos++];

            if (strcmp(tmp, "x") == 0) {
                cnt_token[iter].type = TOKEN_ARGUMENT;
                cnt_token[iter++].str[0] = 'x';
            } else if (strcmp(tmp, "sin") == 0 || strcmp(tmp, "cos") == 0 || strcmp(tmp, "tan") == 0 ||
                       strcmp(tmp, "log") == 0 || strcmp(tmp, "sqrt") == 0 || strcmp(tmp, "ctg") == 0 ||
                       strcmp(tmp, "exp") == 0 || strcmp(tmp, "ln") == 0) {
                cnt_token[iter].type = TOKEN_FUNCTION;
                strcpy(cnt_token[iter++].str, tmp);
            } else {
                return -1;
            }

        } else {
            char s = str[pos++];
            switch (s) {
                case '+':
                case '-':
                case '*':
                case '/':
                case '^': {
                    cnt_token[iter].type = TOKEN_OPERATOR;
                    cnt_token[iter++].str[0] = s;
                } break;
                case '(':
                    cnt_token[iter].type = TOKEN_LEFT_PAREN;
                    cnt_token[iter++].str[0] = '(';
                    break;
                case ')':
                    cnt_token[iter].type = TOKEN_RIGHT_PAREN;
                    cnt_token[iter++].str[0] = ')';
                    break;
                default:
                    break;
            }
        }
    }
    return iter;
}