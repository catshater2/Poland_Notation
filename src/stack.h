#ifndef STACK_H
#define STACK_H

#include "token.h"

typedef struct stack_node {
    Token data;
    struct stack_node* next;
} stack_node;

typedef struct {
    stack_node* top;
} Stack;

Stack* init_stack();
int push(Stack* s, Token data);
int pop(Stack* s, Token* data);
int peek(Stack* s, Token* data);
int is_empty(Stack* s);
void destroy_stack(Stack* s);

#endif