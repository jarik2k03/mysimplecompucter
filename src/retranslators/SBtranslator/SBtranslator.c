#include "SBtranslator.h"
#include <bstree/bstree.h>
#include <draw/draw.h>
#include <malloc.h>
#include <retranslators/SAtranslator/SAtranslator.h>
#include <stdio.h>
#include <string.h>

int8_t *full_mem = NULL;

struct bstree *variables = NULL;

// struct mem *variables = NULL;
const char commandSet[][8]
    = { "REM", "INPUT", "PRINT", "GOTO", "IF", "LET", "END" };

int16_t SBcounter = 0;

int8_t lastnum = 0;
FILE *tempSA = NULL;

struct operations
{
  int8_t priority;
  int8_t sign;
  int8_t pos;
  char loper;
  char roper;
};

int8_t
sb_read_program (char *in)
{
  if (sb_check_filename (in) == -1)
    {
      erropenfile ("Принимаются только *.sb файлы!");
      return -1;
    }
  if (tempSA == NULL)
    tempSA = fopen ("sc_files/assembler/~temp.sa", "a+");
  char path[] = "sc_files/basic/";
  strcat (path, in);
  FILE *SBprog = fopen (path, "r+");

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
sb_write_program (char *in)
{
  if (sb_check_filename (in) == -1)
    {
      erropenfile ("Принимаются только *.sb файлы!");
      return -1;
    }
  if (tempSA == NULL)
    tempSA = fopen ("sc_files/assembler/~temp.sa", "a+");
  char path[] = "sc_files/basic/";
  strcat (path, in);
  FILE *SBprog = fopen (path, "r+");

  if (SBprog == NULL)
    {
      erropenfile ("Указанный файл не найден.");
      return -1;
    }
  char command[8];
  int16_t num;
  char *args = calloc (100, sizeof (char));
  full_mem = calloc (100, sizeof (int8_t));
  variables = bstree_create ("!", -1);

  char line[120];
  while (!feof (SBprog))
    {

      fgets (line, 120, SBprog);
      sscanf (line, "%hd %s %[^\n]s ", &num, command, args);
      printf ("%hd %s %s\n", num, command, args);
      sb_write (num, command, args);
    }
  free (variables);
  fclose (SBprog);
  fclose (tempSA);
  return 0;
}

int8_t
sb_write (int16_t num, char *command, char *args)
{
  if (strcmp (command, "INPUT") == 0)
    INPUT (args);
  else if (strcmp (command, "PRINT") == 0)
    PRINT (args);
  else if (strcmp (command, "LET") == 0)
    LET (args);
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

int8_t
sb_string_check (char *str)
{
  char command[8];
  char *args = calloc (100, sizeof (char));
  int16_t num;
  sscanf (str, "%hd %s %[^\n]s ", &num, command, args);
  // printf ("%hd %s %s\n", num, command, args);
  int8_t returns = 0;
  if (sb_cell_check (num, lastnum) == -1)
    returns = -1;
  else if (sb_command_check (command) == -1)
    returns = -1;

  lastnum = num;
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
sb_cell_check (int16_t num, int16_t lastnum)
{
  if (num <= lastnum)
    {
      erropenfile ("Ошибка - ячейки должны быть последовательны");
      return -1;
    }
  if (num < 0 || num > 99)
    {
      erropenfile ("Ошибка - указанная ячейка не в пределах памяти.");
      return -1;
    }
  return 0;
}

int8_t
alloc (char var)
{
  struct bstree *allocated = bstree_lookup (variables, &var);
  if (allocated != NULL)
    return allocated->value;

  for (int8_t i = 100; i >= 0; i--)
    if (full_mem[i] == 0)
      {
        full_mem[i] = 1;
        bstree_add (variables, &var, i);
        return i;
      }
  return -1;
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
  full_mem[SBcounter] = 1;
  fprintf (tempSA, "%d READ\t\t%d\n", SBcounter, alloc (args[0]));
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

  struct bstree *var = bstree_lookup (variables, &args[0]);
  if (var == NULL)
    {
      erropenfile ("PRINT -- необъявленная переменная");
      return -1;
    }
  fprintf (tempSA, "%d WRITE\t\t%d\n", SBcounter, var->value);
  SBcounter++;

  return 0;
}

int8_t
LET (char *args)
{
  if (args[0] < 'A' || args[0] > 'Z')
    {
      erropenfile ("LET -- переменная начинается с заг. буквы");
      return -1;
    }
  fprintf (tempSA, "%d LOAD\t\t%d\n", SBcounter, alloc (args[0]));
  SBcounter++;
  struct operations o[8] = { 0 }; // до 8 операций

  if (args[2] != '=')
    {
      erropenfile ("LET -- ожидалось '='");
      return -1;
    }
  int8_t i = 4;
  int8_t priority = 0, next = 1;
  while (args[i] != '\0')
    {
      if (args[i] == '(')
        priority++;
      else if (args[i] == ')')
        priority--;

      if (args[i] >= 'A' && args[i] <= 'Z')
        {
          o[next - 1].roper = args[i];
          o[next].loper = args[i];
        }

      if (args[i] == '*' || args[i] == '/')
        {
          o[next].priority = priority + 1;
          o[next].sign = args[i];
          o[next].pos = i;
          next++;
        }
      if (args[i] == '+' || args[i] == '-')
        {
          o[next].priority = priority;
          o[next].sign = args[i];
          o[next].pos = i;
          next++;
        }
      i++;
    }

  for (next = 0; next < 8; next++)
    {
      printf ("\n%d | %c %c %c", o[next].priority, o[next].loper, o[next].sign,
              o[next].roper);
    }

  // for (size_t i = 0; i < count; i++)
  // {
  //   /* code */
  // }

  if (priority != 0)
    {
      erropenfile ("LET -- ошибка в расставлении скобок");
      return -1;
    }

  return 0;
}

// int8_t
// calc (char *args, int8_t i)
// {
//   struct bstree *variables = NULL;

//   for (; args[i] != '\0'; i++)
//     {
//       if (args[i] == '(')
//         calc (args, i++);
//       if (args[i] >= 'A' || args[i] <= 'Z')
//         printf ("%c ", args[i]);
//       printf ("i: %d\n", i);
//       if (args[i] == '*' || args[i] == '/')
//         printf ("%c ", args[i]);
//       else if (args[i] == '+' || args[i] == '-')
//         printf ("%c ", args[i]);

//       if (args[i] == ')')
//         return 0;
//     }
// }

// char
// preCalcProcessing (char *args)
// {
//   char *ptr = strtok (args, " =");
//   char val;
//   sscanf (ptr, "%c", &val);
//   if (!((val) >= 'A' && (val) <= 'Z'))
//     {
//       fprintf (stderr, "NOT CORRECT!\n");
//       exit (EXIT_FAILURE);
//     }
//   ptr = strtok (NULL, " ");
//   char *eq = ptr;
//   if (strcmp (eq, "=") != 0)
//     {
//       fprintf (stderr, "Wrong expression!\n");
//       exit (EXIT_FAILURE);
//     }
//   ptr = strtok (NULL, "");
//   char *exp = ptr;
//   int i = 0;
//   int pos = 0;
//   int j = 0;
//   int operat = 0;
//   int flg = 1;
//   int m = 0;
//   char *assign = (char *)malloc (sizeof (char) * 255);
//   for (int k = 0; k < strlen (exp); k++)
//     {
//       if (exp[k] == '-' && flg)
//         {
//           assign[m] = '0';
//           m++;
//         }
//       flg = 0;
//       if (exp[k] == '+' || exp[k] == '-' || exp[k] == '/' || exp[k] == '*')
//         {
//           operat++;
//         }
//       if (exp[k] == '+' || exp[k] == '-' || exp[k] == '/' || exp[k] == '*'
//           || exp[k] == '(')
//         {
//           flg = 1;
//         }

//       assign[m] = exp[k];
//       m++;
//     }
//   if (operat == 0) // 0+ перед ним, если перед минусом нет аргумента, то
//   пишем
//                    // 0 перед миунсом
//     {
//       sprintf (exp, "0 + %s", assign);
//     }
//   else
//     {
//       sprintf (exp, "%s", assign);
//     }
//   while (exp[i] != '\n' && exp[i] != '\0')
//     {
//       if (exp[i] >= '0' && exp[i] <= '9')
//         {
//           char num[256];
//           j = 0;
//           num[j] = exp[i];
//           j++;
//           pos = i;
//           exp[i] = ' ';
//           i++;
//           while (exp[i] >= '0' && exp[i] <= '9')
//             {
//               num[j] = exp[i];
//               j++;
//               exp[i] = ' ';
//               i++;
//             }
//           num[j] = '\0';
//           exp[pos] = intToConstant (atoi (num));
//         }
//       i++;
//     }
//   sprintf (args, "%s", exp);

//   return val;
// }