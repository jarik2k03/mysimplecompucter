typedef struct NODE
{
  char data;
  struct NODE *next;
} Node;

void stack_push (char data, Node **top);
char stack_pop (Node **top);
char stack_top (Node *top);
int priority (char c);