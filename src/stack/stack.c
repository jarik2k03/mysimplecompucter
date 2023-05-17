#include "stack.h"

#include <malloc.h>

void stack_push(int data, Node **top) {
  Node *tmp = (Node *)malloc(sizeof(Node));
  tmp->data = data;
  tmp->next = *top;
  *top = tmp;
}

int stack_pop(Node **top) {
  Node *tmp;
  int d;
  if (*top == NULL)
    return -1;
  else {
    tmp = *top;
    *top = tmp->next;
    d = tmp->data;
    free(tmp);
    return d;
  }
}

// printf ("%s\n", expr);
int stack_top(Node *top) {
  if (top != NULL) return top->data;
  return '\n';
}

void stack_print(Node *top) {
  if (top == NULL) {
    printf("\n");
    return;
  }
  printf("[%d] ", top->data);
  stack_print(top->next);
}

int priority(char c) {
  switch (c) {
    case '*':
      return 4;
    case '/':
      return 4;
    case '+':
      return 3;
    case '-':
      return 2;
    case '(':
      return 1;
    case ')':
      return 1;
  }
}