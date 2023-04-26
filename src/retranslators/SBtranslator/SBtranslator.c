#include "SBtranslator.h"
#include <bstree/bstree.h>
#include <draw/draw.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>

int8_t SACounter = 0;
int8_t SBCounter = 0;
const char commandSet[][8]
    = { "REM", "INPUT", "PRINT", "GOTO", "IF", "LET", "END" };

int8_t
sb_read_program (char *in)
{
  if (check_filename (in) == -1)
    {
      erropenfile ("Принимаются только *.sb файлы!");
      return -1;
    }
  char path[] = "sc_files/basic/";
  strcat (path, in);
  FILE *SAprog = fopen (path, "r+");
  if (SAprog == NULL)
    {
      erropenfile ("Указанный файл не найден.");
      return -1;
    }

  char line[120];
  while (!feof (SAprog))
    {
      fgets (line, 120, SAprog);
      // if (string_check (line, &num, command, &operand) == -1)
      //   return -1;
    }

  return 0;
}

int8_t
check_filename (char *in)
{
  uint8_t ext = strlen (in) - 3;
  char exp_in[] = ".sb";
  for (uint8_t i = 0; i < 3; i++)
    if (in[ext + i] != exp_in[i])
      return -1;

  return 0;
}

int8_t
string_check (char *str)
{
  char command[8], args[120];
  int16_t num;
  sscanf (str, "%hd %s %[^\n]s ", &num, command, args);
}