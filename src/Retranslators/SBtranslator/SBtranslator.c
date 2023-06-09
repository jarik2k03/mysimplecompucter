#include "SBtranslator.h"

#include <Console/draw/draw.h>
#include <Retranslators/SAtranslator/SAtranslator.h>
#include <Retranslators/bstree/bstree.h>
#include <Retranslators/stack/stack.h>
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
int8_t tempSBcounter;
FILE *tempSA = NULL;

int16_t allocPosition = 99;

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

  if (tempSA == NULL) tempSA = fopen(path2, "w+");

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
  sb_replace_jumps();
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
  fprintf(tempSA, "%.2d =\t\t%.2d\t;(Добавление литерала)\n", SBcounter++, num);
  fprintf(tempSA, "%.2d STORE\t%.2d\t;(Загрузка литерала)\n", SBcounter++,
          allocPosition--);
}

int8_t free_memstack() {
  // fprintf(tempSA, "%.2d =\t\t%.2d\t;(CLEAR)\n", SBcounter++, 0);
  // fprintf(tempSA, "%.2d STORE\t%.2d\t;(CLEAR)\n", SBcounter++,
  //        allocPosition - 1);
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
  char *expr;
  expr = strtok(args, "=");

  char v[2] = {expr[0], '\0'};

  struct bstree *exited = bstree_lookup(variables, v);

  if (!exited) {
    fprintf(tempSA, "%.2d =\t\t%.2d\t;(let:Объявление)\n", SBcounter++, 127);
    fprintf(tempSA, "%.2d STORE\t%.2d\t;(let:Задание значения)\n", SBcounter++,
            alloc(v));
  }
  expr = strtok(NULL, "=");
  char *formatted_expr = infix_to_prefix(expr);
  calc_rpn(formatted_expr, v);

  return 0;
}

int8_t IF(char *args) {
  char *split = strtok(args, "THEN");  // условие

  char sign;
  char expr[64] = " ";
  uint8_t next = 0;
  int8_t i = 0;

  expr[next++] = '(';  // выделяем левое выражение скобками
  for (; split[i] < '<' || split[i] > '>'; i++) expr[next++] = split[i];
  expr[next++] = ')';  // выделяем левое выражение скобками
  expr[next++] = '-';  // минус между выражениями
  expr[next++] = '(';  // выделяем правое выражение скобками
  sign = split[i++];  // знак
  for (; split[i] != '\0'; i++) expr[next++] = split[i];
  expr[next++] = ')';  // выделяем правое выражение скобками
  expr[next++] = '\0';

  printf("infix expr = %s; sign : %c;\n", expr, sign);
  char *rpn_expr = infix_to_prefix(expr);
  printf("prefix expr = %s; sign : %c;\n", rpn_expr, sign);

  split = strtok(NULL, " ");
  printf("then : %s\n", split);

  char argument[256], command[8];
  if (strcmp(split, "HEN") != 0) return erropenfile("Ожидалось THEN");
  split = strtok(NULL, " ");
  strcpy(command, split);
  split = strtok(NULL, " ");
  strcpy(argument, split);

  fprintf(tempSA, "%.2d =\t\t%.2d\t;(if:Объявление)\n", SBcounter++, 127);
  fprintf(tempSA, "%.2d STORE\t%.2d\t;(if:Задание значения)\n", SBcounter++,
          alloc("IF"));
  int8_t load_address = calc_rpn(rpn_expr, "IF");
  fprintf(tempSA, "%.2d LOAD \t%.2d\t;(Результат в аккумулятор)\n", SBcounter++,
          alloc("IF"));
  if (sign == '=')
    fprintf(tempSA, "%.2d JZ   \t%.2d\t;(Прыжок если равно)\n", SBcounter++,
            69);
  else if (sign == '<')
    fprintf(tempSA, "%.2d JNEG \t%.2d\t;(Прыжок если меньше)\n", SBcounter++,
            69);
  else if (sign == '>') {
    fprintf(tempSA, "%.2d NOT  \t%.2d\t;(Сначала инверсия)\n", SBcounter++,
            load_address);
    fprintf(tempSA, "%.2d JNEG \t%.2d\t;(Прыжок если больше)\n", SBcounter++,
            69);
  }
  tempSBcounter = SBcounter;
  SBcounter = 70;
  printf("command: %s\n", command);
  printf("args: %s\n", argument);

  sb_write("IF", command, argument);
  SBcounter = tempSBcounter;
  return 0;
}

int8_t GOTO(char *args) {
  fprintf(tempSA, "%.2i JUMP \t%s\n", SBcounter++, args);
  return 0;
}

int8_t END(char *args) {
  fprintf(tempSA, "%.2i HALT \t%d\n", SBcounter++, atoi(args));
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

int8_t calc_rpn(char *rpn_expr, char *var) {
  // printf("%s\n", rpn_expr);
  Node *calculator = NULL;
  char *elements = strtok(rpn_expr, " ");
  int number, a, b;
  const int16_t begin_address = allocPosition;
  int clear0 = begin_address, clearn = begin_address;
  if (!strcmp(var, "IF")) clearn--;

  while (elements != NULL) {
    stack_print(calculator);

    if ((number = atoi(elements)) != 0) {
      stack_push(allocPosition, &calculator);
      alloc_memstack(number);
    } else if (elements[0] >= 'A' && elements[0] <= 'Z') {
      struct bstree *exited_var = bstree_lookup(variables, elements);
      if (exited_var == NULL)
        return erropenfile("LET: используется необъявленная переменная");

      printf("exited: %s!\n", exited_var->key);
      stack_push(exited_var->value, &calculator);
    } else {
      b = stack_pop(&calculator);
      a = stack_pop(&calculator);
      printf("a:%d b:%d\n", a, b);

      fprintf(tempSA, "%.2d LOAD \t%.2d\t;(%s)\n", SBcounter++, a,
              ((a > begin_address) ? ("Загружаем переменную")
                                   : ("Загружаем литерал")));
      if (elements[0] == '+')
        fprintf(tempSA, "%.2d ADD  \t%.2d\t;(a + b)\n", SBcounter++, b);
      else if (elements[0] == '-')
        fprintf(tempSA, "%.2d SUB  \t%.2d\t;(a - b)\n", SBcounter++, b);
      else if (elements[0] == '*')
        fprintf(tempSA, "%.2d MUL  \t%.2d\t;(a * b)\n", SBcounter++, b);
      else if (elements[0] == '/')
        fprintf(tempSA, "%.2d DIVIDE\t%.2d\t;(a / b)\n", SBcounter++, b);
      else
        return erropenfile("LET::неопознанный операнд!");
      if (a <= begin_address) free_memstack(a);
      if (b <= begin_address) free_memstack(b);
      fprintf(tempSA, "%.2d STORE\t%.2d\t;(Выгружаем)\n", SBcounter++,
              allocPosition);

      if (b < clear0) clear0 = b;
      stack_push(allocPosition--, &calculator);
    }
    // printf("cur element:%s.\n", elements);
    elements = strtok(NULL, " ");
  }
  stack_print(calculator);
  stack_pop(&calculator);
  fprintf(tempSA, "%.2d STORE\t%.2d\t;(Запись в окончательную переменную)\n",
          SBcounter++, alloc(var));

  fprintf(tempSA, "%.2d =\t\t0\t;(Аккумулятор = 0)\n", SBcounter++);
  printf("cl0 :%d; cln :%d\n", clear0, clearn);
  while (clear0 <= clearn)
    fprintf(tempSA, "%.2d STORE\t%.2d\t;(Очистка стека)\n", SBcounter++,
            clear0++);
  allocPosition++;
  return clearn;
}

void sb_replace_jumps() {
  int num;
  char command[8], args[64];
  char line[128];
  struct bstree *replace_address = NULL;
  fseek(tempSA, 0, SEEK_SET);
  while (!feof(tempSA)) {
    fgets(line, 128, tempSA);

    sscanf(line, "%d %s %[^\n]s", &num, command, args);
    if (!strcmp(command, "JUMP")) {
      fseek(tempSA, -3, SEEK_CUR);  // перетирает два числа и '\n'
      replace_address = bstree_lookup(jump_addresses, args);
      fprintf(tempSA, "%.2d\n", replace_address->value - 1);
    }
  }
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
