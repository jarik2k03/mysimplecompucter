#include "stack.h"
#include <malloc.h>

void
stack_push (char data, Node **top)
{
  Node *tmp = (Node *)malloc (sizeof (Node));
  tmp->data = data;
  tmp->next = *top;
  *top = tmp;
}

char
stack_pop (Node **top)
{
  Node *tmp;
  char d;
  if (*top == NULL)
    return -1;
  else
    {
      tmp = *top;
      *top = tmp->next;
      d = tmp->data;
      free (tmp);
      return d;
    }
}

// printf ("%s\n", expr);
char
stack_top (Node *top)
{
  if (top != NULL)
    return top->data;
  return '\n';
}

int
priority (char c)
{
  switch (c)
    {
    case '*':
      return 4;
    case '/':
      return 4;
    case '+':
      return 2;
    case '-':
      return 2;
    }
}