#include "SAtranslator.h"
#include <bstree/bstree.h>
#include <ctype.h>
#include <draw/draw.h>
#include <myTerm/myTerm.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct bstree *commands = NULL;

int
read_program (char *in)
{
  if (check_filename (in) == -1)
    {
      erropenfile ("Принимаются только *.sa файлы!");
      return -1;
    }
  for (size_t i = 0; i < 30; i++)
    {
      printf ("%d ", in[i]);
    }

  FILE *SAprog = fopen (in, "r+");
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
string_check (char *str, int16_t *num, char *command, int16_t *operand)
{
  char *comment = calloc (100, sizeof (char));
  sscanf (str, "%hd %s %hd %[^\n]s ", num, command, operand, comment);
  if (comment_check (comment) == -1)
    return -1;
  if (command_check (command) == -1)
    return -1;

  printf ("\n%hd: %s %hd %s  ", *num, command, *operand, comment);
  return 0;
}

int8_t
comment_check (char *com)
{
  int8_t a = 1, b = strlen (com) - 1;
  if (b < 0) // пустой комментарий
    return 0;
  if (com[0] != ';' || com[a] != '(' || com[b] != ')')
    {
      erropenfile ("Синт. ошибка - комментарий не определен");
      return -1;
    }
  return 0;
}

int8_t
command_check (char *command)
{

  return 0;
}

int8_t
init_commands ()
{
  if (commands != NULL)
    return 0;
  // printf ("\ninit_commands");
  FILE *saved_commands = fopen ("commands.txt", "r+");
  if (saved_commands == NULL)
    {
      erropenfile ("Не удалось найти словарь!");
      return -1;
    }

  commands = bstree_create ("ROOT", 0);
  char key[8];
  uint8_t value;
  while (!feof (saved_commands))
    {
      fscanf (saved_commands, "%s %hhd", key, &value);
      bstree_add (commands, key, value);
    }

  print_tree_as_list (commands);
  return 0;
}