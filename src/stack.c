#include "stack.h"

#include <stdlib.h>

Stack* init_stack() {
    Stack* s = malloc(sizeof(Stack));
    s->top = NULL;
    return s;
}

int push(Stack* s, Token data) {
    stack_node* node = malloc(sizeof(stack_node));
    if (!node) return 0;
    node->data = data;
    node->next = s->top;
    s->top = node;
    return 1;
}

int pop(Stack* s, Token* data) {
    if (!s->top) return 0;
    stack_node* tmp = s->top;
    *data = tmp->data;
    s->top = tmp->next;
    free(tmp);
    return 1;
}

int peek(Stack* s, Token* data) {
    if (!s->top) return 0;
    *data = s->top->data;
    return 1;
}

int is_empty(Stack* s) { return s->top == NULL; }

void destroy_stack(Stack* s) {
    while (s->top) {
        stack_node* tmp = s->top;
        s->top = tmp->next;
        free(tmp);
    }
    free(s);
}