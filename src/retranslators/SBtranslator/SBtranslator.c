#include "SBtranslator.h"
#include <bstree/bstree.h>
#include <draw/draw.h>
#include <retranslators/SAtranslator/SAtranslator.h>
#include <stack/stack.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char commandSet[7][8]
    = { "REM", "INPUT", "PRINT", "GOTO", "IF", "LET", "END" };

int8_t
sb_read_program (char *in)
{
  if (sb_check_filename (in) == -1)
    {
      erropenfile ("Принимаются только *.sb файлы!");
      return -1;
    }
  char path1[50] = "sc_files/basic/";
  strcat (path1, in);

  FILE *SBprog = fopen (path1, "r+");

  if (SBprog == NULL)
    {
      erropenfile ("Указанный файл не найден.");
      return -1;
    }

  char line[120];
  while (!feof (SBprog))
    {
      fgets (line, 120, SBprog);
      if (sb_string_check (line) == -1)
        return -1;
    }
  fclose (SBprog);
  return 0;
}
int8_t
sb_string_check (char *str)
{
  char command[8];
  char *args = calloc (100, sizeof (char));
  int16_t num;
  sscanf (str, "%hd %s %[^\n]s ", &num, command, args);
  int8_t returns = 0;
  if (sb_cell_check (num) == -1)
    returns = -1;
  else if (sb_command_check (command) == -1)
    returns = -1;

  free (args);
  return returns;
}
int8_t
sb_command_check (char *command)
{
  for (uint8_t i = 0; i < 8; i++)
    if (strcmp (command, commandSet[i]) == 0)
      return 0;

  erropenfile ("Ошибка - неопознанная команда.");
  return -1; // не нашли
}
int8_t
sb_cell_check (int16_t num)
{
  if (num < 0 || num > 99)
    {
      erropenfile ("Ошибка - указанная ячейка не в пределах памяти.");
      return -1;
    }
  return 0;
}
int8_t
sb_check_filename (char *in)
{
  uint8_t ext = strlen (in) - 3;
  char exp_in[] = ".sb";
  for (uint8_t i = 0; i < 3; i++)
    if (in[ext + i] != exp_in[i])
      return -1;

  return 0;
}

/* Основные операции */

static struct bstree *jump_addresses = NULL;

static struct bstree *variables = NULL;

int16_t SBcounter = 0;
FILE *tempSA = NULL;

int16_t allocPosition = 99;

FILE *debug = NULL;

int8_t
sb_write (char *num, char *command, char *args)
{
  bstree_add (jump_addresses, num, SBcounter);
  if (strcmp (command, "INPUT") == 0)
    INPUT (args);
  else if (strcmp (command, "PRINT") == 0)
    PRINT (args);
  else if (strcmp (command, "LET") == 0)
    LET (args);
  else if (strcmp (command, "IF") == 0)
    IF (args);
  else if (strcmp (command, "END") == 0)
    END (args);
  else if (strcmp (command, "GOTO") == 0)
    GOTO (args);
  else
    return -1;
}

int8_t
sb_write_program (char *in)
{
  if (sb_check_filename (in) == -1)
    {
      erropenfile ("Принимаются только *.sb файлы!");
      return -1;
    }
  char path1[50] = "sc_files/basic/";
  char path2[50] = "sc_files/assembler/";
  strcat (path1, in);
  strcat (path2, in);
  path2[strlen (path2) - 1] = 'a';

  if (tempSA == NULL)
    tempSA = fopen (path2, "w");

  if (variables == NULL)
    variables = bstree_create ("ROOT", 0);

  if (jump_addresses == NULL)
    jump_addresses = bstree_create ("ROOT", 0);

  FILE *SBprog = fopen (path1, "r+");

  if (SBprog == NULL)
    {
      erropenfile ("Указанный файл не найден!");
      return -1;
    }
  char command[8];
  char num[4];
  char *args = calloc (100, sizeof (char));

  debug = fopen ("sc_files/latest.log", "w");

  while (!feof (SBprog))
    {
      fscanf (SBprog, "%s %s %[^\n]s ", num, command, args);
      // fprintf (debug, "%hd %s %s\n", num, command, args);
      sb_write (num, command, args);
    }
  print_tree_as_list (variables);
  print_tree_as_list (jump_addresses);
  fclose (debug);
  fclose (SBprog);
  sb_reset ();
  return 0;
}

int8_t
alloc (char var)
{
  char variable[8] = { 0 };
  variable[0] = var;
  struct bstree *allocated = bstree_lookup (variables, variable);

  if (allocated != NULL)
    return allocated->value;

  bstree_add (variables, variable, allocPosition);
  allocPosition--;
  return allocPosition + 1;
}

int8_t
INPUT (char *args)
{
  if (args[0] < 'A' || args[0] > 'Z')
    {
      erropenfile ("INPUT -- переменная начинается с заг. буквы");
      return -1;
    }
  if (args[1] != ' ' && args[1] != '\0')
    {
      erropenfile ("INPUT -- переменная только из одной буквы");
      return -1;
    }
  fprintf (tempSA, "%.2i READ \t\t%d\n", SBcounter, alloc (args[0]));
  SBcounter++;

  return 0;
}

int8_t
PRINT (char *args)
{
  if (args[0] < 'A' || args[0] > 'Z')
    {
      erropenfile ("PRINT -- переменная начинается с заг. буквы");
      return -1;
    }
  if (args[1] != ' ' && args[1] != '\0')
    {
      erropenfile ("PRINT -- переменная только из одной буквы");
      return -1;
    }
  fprintf (tempSA, "%.2i WRITE \t\t%d\n", SBcounter, alloc (args[0]));
  SBcounter++;
  return 0;
}

int8_t
LET (char *args)
{
  // char arr[120];
  // strcpy (arr, args);
  char *expr;
  expr = strtok (args, "=");
  // printf ("let-arg1:%s\n", expr);
  expr = strtok (NULL, "=");
  // printf ("let-arg2:%s\n", expr);
  char *formatted_expr = infix_to_prefix (expr);

  return 0;
}

int8_t
IF (char *args)
{
  return 0;
}

int8_t
GOTO (char *args)
{
  struct bstree *cell = bstree_lookup (jump_addresses, args);
  if (cell == NULL)
    {
      erropenfile (
          "GOTO:: попытка перемещения в несуществующую инструкцию.\n");
      return -1;
    }
  fprintf (tempSA, "%.2i JUMP \t\t%.2d\n", SBcounter, cell->value);
  return 0;
}

int8_t
END (char *args)
{
  fprintf (tempSA, "%.2i HALT \t\t%d\n", SBcounter, atoi (args));
  SBcounter++;
}

char *
infix_to_prefix (char *expr)
{
  char *rpn_expr = calloc (120, sizeof (char));
  Node *stack = NULL;
  char token;
  int16_t i = 0;

  for (; expr[i] != '\0'; i++)
    {
      token = expr[i];
      if (token == ' ')
        continue;

      if (token >= '0' && token <= '9')
        rpn_expr[i] = token;
      else if (token >= 'A' && token <= 'Z')
        rpn_expr[i] = token;
      else if (token == '(')
        stack_push (token, &stack);
      else if (token == ')')
        {
          while (stack_top (stack) != '(')
            {
              rpn_expr[i++] = stack_pop (&stack);
            }
          stack_pop (&stack);
        }
      else if (token == '+' || token == '-' || token == '*' || token == '/')
        {
          char head = stack_top (stack);
          if (stack == NULL || head == '(' || head == ')')
            {
              stack_push (token, &stack);
              continue;
            }
          int t = priority (token), h = priority (head);
          if (t > h)
            stack_push (token, &stack);
          else if (t == h)
            {
              rpn_expr[i] = stack_pop (&stack);
              stack_push (token, &stack);
            }
          else if (t < h)
            {
              while (stack != NULL && (t < priority (stack->data)))
                {
                  rpn_expr[i++] = stack_pop (&stack);
                }
              stack_push (token, &stack);
            }
        }
    }
  while (stack != NULL)
    rpn_expr[i++] = stack_pop (&stack);

  for (i = 0; i < 120; i++)
    {
      printf ("%c", rpn_expr[i]);
    }
}

// printf ("%s\n", expr);

void
rpn_write (char *rpn_expr)
{
}

void
sb_reset ()
{
  SBcounter = 0;
  allocPosition = 99;
  bstree_free (variables);
  variables = NULL;
  bstree_free (jump_addresses);
  jump_addresses = NULL;
  fclose (tempSA);
  tempSA = NULL;
}
