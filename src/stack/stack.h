typedef struct NODE {
  int data;
  struct NODE *next;
} Node;

void stack_push(int data, Node **top);
int stack_pop(Node **top);
int stack_top(Node *top);
void stack_print(Node *top);
int priority(char c);