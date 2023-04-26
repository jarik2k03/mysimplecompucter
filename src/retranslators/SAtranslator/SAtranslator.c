#include "SAtranslator.h"
#include <bstree/bstree.h>
#include <draw/draw.h>
#include <mySimpleComputer/mySimpleComputer.h>
#include <myTerm/myTerm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct bstree *commands = NULL;
int *tempmem = NULL;

int8_t
read_program (char *in)
{
  if (check_filename (in) == -1)
    {
      erropenfile ("Принимаются только *.sa файлы!");
      return -1;
    }
  char path[] = "sc_files/assembler/";
  strcat (path, in);
  FILE *SAprog = fopen (path, "r+");
  if (SAprog == NULL)
    {
      erropenfile ("Указанный файл не найден.");
      return -1;
    }
  char command[8];
  int16_t num, operand;

  char line[120];
  init_commands ();
  while (!feof (SAprog))
    {
      fgets (line, 120, SAprog);
      if (string_check (line, &num, command, &operand) == -1)
        return -1;
    }

  return 0;
}

int8_t
write_program (char *in)
{
  if (check_filename (in) == -1)
    {
      erropenfile ("Принимаются только *.sa файлы!");
      return -1;
    }
  char path[] = "sc_files/assembler/";
  strcat (path, in);
  FILE *SAprog = fopen (path, "r+");
  if (SAprog == NULL)
    {
      erropenfile ("Указанный файл не найден.");
      return -1;
    }
  char command[8];
  int16_t num, operand;
  tempmem = calloc (100, sizeof (int32_t));
  char line[120];

  while (!feof (SAprog))
    {
      fgets (line, 120, SAprog);
      sscanf (line, "%hd %s %hd", &num, command, &operand);
      write_to_memory (&num, command, &operand);
    }

  save_memory (in);
  fclose (SAprog);
  free (tempmem);
}

int8_t
check_filename (char *in)
{
  uint8_t ext = strlen (in) - 3;
  char exp_in[] = ".sa";
  for (uint8_t i = 0; i < 3; i++)
    if (in[ext + i] != exp_in[i])
      return -1;

  return 0;
}

int8_t
save_memory (char *in)
{
  uint8_t ext = strlen (in) - 3;
  char o[] = ".o";
  for (uint8_t i = 0; i < 3; i++)
    in[ext + i] = o[i];

  char path[] = "sc_files/binary/";
  strcat (path, in);
  FILE *obj = fopen (path, "wb");
  fwrite (tempmem, sizeof (int), 100, obj);
  fclose (obj);
  // printf ("%s\t", in);
  return 0;
}

int8_t
write_to_memory (int16_t *num, char *command, int16_t *operand)
{
  int32_t value;

  struct bstree *com = bstree_lookup (commands, command);

  value = *operand;
  if (com->key != "=")
    sc_commandEncode (com->value, *operand, &value);

  tempmem[*num] = value;
  // mainpos_cursor ();
  // printf ("%02X%02X ", *command, *operand);
  // print_cell (*num, value, com->value >> 1, *operand);
}

int8_t
string_check (char *str, int16_t *num, char *command, int16_t *operand)
{
  char *comment = calloc (100, sizeof (char));
  sscanf (str, "%hd %s %hd %[^\n]s ", num, command, operand, comment);
  int8_t returns = 0;
  if (cell_check (*num) == -1)
    returns = -1;
  else if (command_check (command) == -1)
    returns = -1;
  else if (operand_check (*operand, command) == -1)
    returns = -1;
  else if (comment_check (comment) == -1)
    returns = -1;

  // printf ("\n%hd: %s %hd %s  ", *num, command, *operand, comment);
  free (comment);
  return returns;
}

int8_t
comment_check (char *com)
{
  int8_t a = 1, b = strlen (com) - 1;
  if (b < 0) // пустой комментарий
    return 0;
  if (com[0] != ';' || com[a] != '(' || com[b] != ')')
    {
      erropenfile ("Синт. ошибка - комментарий не определен.");
      return -1;
    }
  return 0;
}

int8_t
command_check (char *command)
{
  struct bstree *lookup = bstree_lookup (commands, command);
  if (lookup == NULL)
    {
      erropenfile ("Ошибка - неопознанная команда.");
      return -1;
    }
  return 0;
}

int8_t
cell_check (int16_t num)
{
  if (num < 0 || num > 99)
    {
      erropenfile ("Ошибка - указанная ячейка не в пределах памяти.");
      return -1;
    }
  return 0;
}

int8_t
operand_check (int16_t operand, char *command)
{
  struct bstree *lookup = bstree_lookup (commands, command);
  if (operand > 127 && lookup->value != 0)
    {
      erropenfile ("Ошибка - переполнение операнда.");
      return -1;
    }
  return 0;
}

int8_t
init_commands ()
{
  if (commands != NULL)
    return 0;
  FILE *saved_commands = fopen ("commands.txt", "r+");
  if (saved_commands == NULL)
    {
      erropenfile ("Не удалось найти словарь!");
      return -1;
    }

  char key[8];
  uint8_t value;
  commands = bstree_create ("=", 0);
  while (!feof (saved_commands))
    {
      fscanf (saved_commands, "%s %hhd", key, &value);
      bstree_add (commands, key, value);
    }
  return 0;
}