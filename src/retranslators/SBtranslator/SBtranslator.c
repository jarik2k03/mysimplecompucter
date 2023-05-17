#include "SBtranslator.h"

#include <bstree/bstree.h>
#include <draw/draw.h>
#include <retranslators/SAtranslator/SAtranslator.h>
#include <stack/stack.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char commandSet[7][8] = {"REM", "INPUT", "PRINT", "GOTO",
                               "IF",  "LET",   "END"};

int8_t sb_read_program(char *in) {
  if (sb_check_filename(in) == -1) {
    erropenfile("Принимаются только *.sb файлы!");
    return -1;
  }
  char path1[50] = "sc_files/basic/";
  strcat(path1, in);

  FILE *SBprog = fopen(path1, "r+");

  if (SBprog == NULL) {
    erropenfile("Указанный файл не найден.");
    return -1;
  }

  char line[120];
  while (!feof(SBprog)) {
    fgets(line, 120, SBprog);
    if (sb_string_check(line) == -1) return -1;
  }
  fclose(SBprog);
  return 0;
}
int8_t sb_string_check(char *str) {
  char command[8];
  char *args = calloc(100, sizeof(char));
  int16_t num;
  sscanf(str, "%hd %s %[^\n]s ", &num, command, args);
  int8_t returns = 0;
  if (sb_cell_check(num) == -1)
    returns = -1;
  else if (sb_command_check(command) == -1)
    returns = -1;

  free(args);
  return returns;
}
int8_t sb_command_check(char *command) {
  for (uint8_t i = 0; i < 8; i++)
    if (strcmp(command, commandSet[i]) == 0) return 0;

  erropenfile("Ошибка - неопознанная команда.");
  return -1;  // не нашли
}
int8_t sb_cell_check(int16_t num) {
  if (num < 0 || num > 99) {
    erropenfile("Ошибка - указанная ячейка не в пределах памяти.");
    return -1;
  }
  return 0;
}
int8_t sb_check_filename(char *in) {
  uint8_t ext = strlen(in) - 3;
  char exp_in[] = ".sb";
  for (uint8_t i = 0; i < 3; i++)
    if (in[ext + i] != exp_in[i]) return -1;

  return 0;
}

/* Основные операции */

static struct bstree *jump_addresses = NULL;

static struct bstree *variables = NULL;

int16_t SBcounter = 0;
FILE *tempSA = NULL;

int16_t allocPosition = 98;

FILE *debug = NULL;

int8_t sb_write(char *num, char *command, char *args) {
  bstree_add(jump_addresses, num, SBcounter);
  if (strcmp(command, "INPUT") == 0)
    INPUT(args);
  else if (strcmp(command, "PRINT") == 0)
    PRINT(args);
  else if (strcmp(command, "LET") == 0)
    LET(args);
  else if (strcmp(command, "IF") == 0)
    IF(args);
  else if (strcmp(command, "END") == 0)
    END(args);
  else if (strcmp(command, "GOTO") == 0)
    GOTO(args);
  else
    return -1;
}

int8_t sb_write_program(char *in) {
  if (sb_check_filename(in) == -1) {
    erropenfile("Принимаются только *.sb файлы!");
    return -1;
  }
  char path1[50] = "sc_files/basic/";
  char path2[50] = "sc_files/assembler/";
  strcat(path1, in);
  strcat(path2, in);
  path2[strlen(path2) - 1] = 'a';

  if (tempSA == NULL) tempSA = fopen(path2, "w");

  if (variables == NULL) variables = bstree_create("ROOT", 0);

  if (jump_addresses == NULL) jump_addresses = bstree_create("ROOT", 0);

  FILE *SBprog = fopen(path1, "r+");

  if (SBprog == NULL) {
    erropenfile("Указанный файл не найден!");
    return -1;
  }
  char command[8];
  char num[4];
  char *args = calloc(100, sizeof(char));

  debug = fopen("sc_files/latest.log", "w");

  while (!feof(SBprog)) {
    fscanf(SBprog, "%s %s %[^\n]s ", num, command, args);
    // fprintf (debug, "%hd %s %s\n", num, command, args);
    sb_write(num, command, args);
  }
  print_tree_as_list(variables);
  print_tree_as_list(jump_addresses);
  fclose(debug);
  fclose(SBprog);
  sb_reset();
  return 0;
}

int8_t alloc(char *var) {
  struct bstree *allocated = bstree_lookup(variables, var);

  if (allocated != NULL) return allocated->value;

  bstree_add(variables, var, allocPosition);
  return allocPosition--;
}

int8_t alloc_memstack(int num) {
  fprintf(tempSA, "%.2d =\t\t%.2d\t;(Инициализация быстрой переменной)\n",
          allocPosition, num);
  allocPosition--;
}

int8_t free_memstack() {
  fprintf(tempSA, "%.2d =\t\t00\t;(Очистка быстрой переменной)\n",
          allocPosition);
  ++allocPosition;
}

int8_t INPUT(char *args) {
  if (args[0] < 'A' || args[0] > 'Z') {
    erropenfile("INPUT -- переменная начинается с заг. буквы");
    return -1;
  }
  if (args[1] != ' ' && args[1] != '\0') {
    erropenfile("INPUT -- переменная только из одной буквы");
    return -1;
  }
  fprintf(tempSA, "%.2i READ \t%d\n", SBcounter, alloc(args));
  SBcounter++;

  return 0;
}

int8_t PRINT(char *args) {
  if (args[0] < 'A' || args[0] > 'Z') {
    erropenfile("PRINT -- переменная начинается с заг. буквы");
    return -1;
  }
  if (args[1] != ' ' && args[1] != '\0') {
    erropenfile("PRINT -- переменная только из одной буквы");
    return -1;
  }
  fprintf(tempSA, "%.2i WRITE \t%d\n", SBcounter, alloc(args));
  SBcounter++;
  return 0;
}

int8_t LET(char *args) {
  // char arr[120];
  // strcpy (arr, args);
  char *expr;
  expr = strtok(args, "=");
  fprintf(tempSA, "%.2d =\t\t%.2d\t;(Новая переменная заняла свою ячейку)\n",
          alloc(&expr[0]), 127);
  // printf ("let-arg1:%s\n", expr);
  expr = strtok(NULL, "=");
  // printf ("let-arg2:%s\n", expr);
  char *formatted_expr = infix_to_prefix(expr);
  calc_rpn(formatted_expr);

  return 0;
}

int8_t IF(char *args) { return 0; }

int8_t GOTO(char *args) {
  struct bstree *cell = bstree_lookup(jump_addresses, args);
  if (cell == NULL) {
    erropenfile("GOTO:: попытка перемещения в несуществующую инструкцию.\n");
    return -1;
  }
  fprintf(tempSA, "%.2i JUMP \t%.2d\n", SBcounter, cell->value);
  return 0;
}

int8_t END(char *args) {
  fprintf(tempSA, "%.2i HALT \t%d\n", SBcounter, atoi(args));
  SBcounter++;
}

char *infix_to_prefix(char *expr) {
  char *rpn_expr = malloc(120 * sizeof(char));
  for (int8_t a = 0; a < 120; a++) rpn_expr[a] = ' ';

  Node *stack = NULL;
  char token;
  int16_t next = 0;

  for (int16_t i = 0; expr[i] != '\0'; i++) {
    token = expr[i];
    if (token == ' ') continue;
    if (token >= '0' && token <= '9')  // записываем в выходной массив
      rpn_expr[next++] = token;
    else if (token >= 'A' && token <= 'Z')  // записываем в выходной массив
      rpn_expr[next++] = token;
    else if (token == '(')  // помещаем скобку в стек
      stack_push(token, &stack);
    else if (token == ')') {
      next++;
      // вытаскиваем из стека после откр скобки в массив
      for (; stack_top(stack) != '('; next++)
        rpn_expr[next++] = stack_pop(&stack);
      stack_pop(&stack);  // уничтожаем скобку
    } else if (token == '+' || token == '-' || token == '*' || token == '/') {
      if (stack == NULL)  // первый оператор помещаем сразу в стек
      {
        stack_push(token, &stack);
        continue;
      }
      next++;
      char head = stack_top(stack);
      int t = priority(token), h = priority(head);
      if (t > h)  // добавление в стек более приоритетного операнда
        stack_push(token, &stack);
      else if (t == h) {
        // откапывание верхнего и запись приоритетного операнда
        rpn_expr[next++] = stack_pop(&stack);
        stack_push(token, &stack);
      } else if (t < h) {
        // вытаскиваем из стека данные большего приоритета в массив
        for (; stack != NULL && (t < priority(stack->data)); next++)
          rpn_expr[next++] = stack_pop(&stack);
        stack_push(token, &stack);
      }
    }
  }
  for (; stack != NULL; next++)
    rpn_expr[next++] = stack_pop(&stack);  // извлечение оставшихся операций
  rpn_expr[next++] = '\0';  // конец
  return rpn_expr;
}

void calc_rpn(char *rpn_expr) {
  // for (int8_t i = 0; i < 120; i++)
  printf("%s\n", rpn_expr);

  Node *calculator = NULL;
  char *elements = strtok(rpn_expr, " ");
  int number, a, b;
  int16_t begin_address = allocPosition;

  while (elements != NULL) {
    stack_print(calculator);

    if ((number = atoi(elements)) != 0) {
      alloc_memstack(number);
      stack_push(allocPosition, &calculator);
    } else if (elements[0] >= 'A' && elements[0] <= 'Z') {
      struct bstree *exited_var = bstree_lookup(variables, &elements[0]);
      if (exited_var == NULL)
        return erropenfile("LET: используется необъявленная переменная");

      printf("exited: %s!\n", exited_var->key);
      stack_push(exited_var->value, &calculator);
    } else {
      b = stack_pop(&calculator);  // пред адреса - var
                                   // очищает константу из памяти
      a = stack_pop(&calculator);  // пред адреса - var
                                   // очищает константу из памяти
      printf("a:%d b:%d\n", a, b);

      fprintf(tempSA, "%.2d LOAD \t%.2d\t;(%s)\n", SBcounter++, a,
              ((a > begin_address) ? ("Загружаем переменную")
                                   : ("Загружаем константу")));
      if (elements[0] == '+')
        fprintf(tempSA, "%.2d ADD  \t%.2d\n", SBcounter++, b);
      else if (elements[0] == '-')
        fprintf(tempSA, "%.2d SUB  \t%.2d\n", SBcounter++, b);
      else if (elements[0] == '*')
        fprintf(tempSA, "%.2d MUL  \t%.2d\n", SBcounter++, ++b);
      else if (elements[0] == '/')
        fprintf(tempSA, "%.2d DIVIDE\t%.2d\n", SBcounter++, b);
      else
        return erropenfile("LET::неопознанный операнд!");

      if (b <= begin_address) free_memstack(b);
      stack_push(allocPosition, &calculator);
      fprintf(tempSA, "%.2d STORE\t%.2d\n", SBcounter++, allocPosition);
    }
    printf("cur element:%s.\n", elements);
    elements = strtok(NULL, " ");
  }
  printf("RESULT:%d\n", stack_pop(&calculator));
}

void sb_reset() {
  SBcounter = 0;
  allocPosition = 99;
  bstree_free(variables);
  variables = NULL;
  bstree_free(jump_addresses);
  jump_addresses = NULL;
  fclose(tempSA);
  tempSA = NULL;
}
