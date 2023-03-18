#include <ctype.h>
#include <fcntl.h>
#include <memory.h>
#include <myBigChars/myBigChars.h>
#include <myReadKey/myReadKey.h>
#include <mySimpleComputer/mySimpleComputer.h>
#include <myTerm/myTerm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#define TERMINAL_PATH "/dev/tty"

#define REFRESHONMOVE 1
#define REFRESHENTER 2
#define REFRESHSAVELOAD 3
#define REFRESHRESET 4
#define RAM 100
#define INPUT 0
#define MOVE 1
int current = 3;
int mode = MOVE;

void
display_bigchar (int value, int x, int shift)
{
  int bc_id;
  if (x == 2)
    (value & 0x4000) ? (bc_id = 1) : (bc_id = 0);
  else
    bc_id = 2 + ((value >> shift) & 0b1111);

  bc_printbigchar (NUMS[bc_id].N, 14, x, green, yellow);
  mt_setbgcolor (darkgrey);
}

void
draw_frame (int x, int y, int x1, int y1, char *title)
{
  bc_box (y, x, y1 - y, x1 - x);
  mt_gotoXY (y, (x1 + x) / 2 - (strlen (title) / 2));
  printf ("%s", title);
}

void
main_printCell (int address)
{
  int value, row, col, command, operand;
  char *buf = malloc (6 * sizeof (char));
  if (sc_memoryGet (address, &value) != 0
      || sc_commandDecode (value, &command, &operand) != 0)
    return;
  row = address / 10;
  col = address % 10;
  if (current == address)
    mt_setbgcolor (green);

  sc_commandDecode (value, &command, &operand);
  command >>= 1;
  snprintf (buf, 6, "%c%02X%02X", (value & 0x4000) ? '-' : '+', command,
            operand);

  mt_gotoXY (2 + row, 2 + col * 6);
  write (1, buf, 5);

  if (current == address)
    mt_setbgcolor (darkgrey);

  free (buf);
  return;
}

void
main_hints ()
{
  int x = 14, y = 48;
  mt_gotoXY (x, y);
  printf ("l\t- load");
  mt_gotoXY (x++, y);
  printf ("s\t- save");
  mt_gotoXY (x++, y);
  printf ("r\t- run");
  mt_gotoXY (x++, y);
  printf ("t\t- step");
  mt_gotoXY (x++, y);
  printf ("i\t- reset");
  mt_gotoXY (x++, y);
  printf ("F5\t- accumulator");
  mt_gotoXY (x++, y);
  printf ("F6\t- instructionCounter");
}

void
main_flags ()
{
  int value;
  char regs[] = "OEVM";
  mt_gotoXY (11, 74);

  for (u_int8_t i = 0; i < 5; i++)
    {
      sc_regGet (i + 1, &value);
      if (value == 1)
        mt_setbgcolor (red);
      else
        mt_setbgcolor (darkgrey);
      printf ("%c ", regs[i]);
    }
  mt_setbgcolor (darkgrey);
}

void
main_accumulator ()
{
  mt_gotoXY (2, 74);
  printf ("%d", current);
}

void
main_counter ()
{
  mt_gotoXY (5, 74);
  printf ("+0000");
}

void
main_operation ()
{
  mt_gotoXY (8, 74);
  printf ("+00 : 00");
}

void
main_display ()
{
  int x = 2;
  int command, operand;
  int value;
  sc_memoryGet (current, &value);
  sc_commandDecode (value, &command, &operand);
  command >>= 1;

  display_bigchar (value, x, 0);
  display_bigchar (command, x += 9, 4);
  display_bigchar (command, x += 9, 0);
  display_bigchar (operand, x += 9, 4);
  display_bigchar (operand, x += 9, 0);
}

void
main_interface ()
{
  draw_frame (1, 1, 60, 11, "Memory");
  for (int i = 0; i < 100; i++)
    main_printCell (i);
  draw_frame (63, 1, 90, 2, "accumulator");
  main_accumulator ();
  draw_frame (63, 4, 90, 5, "instructionCounter");
  main_counter ();
  draw_frame (63, 7, 90, 8, "Operation");
  main_operation ();
  draw_frame (63, 10, 90, 11, "Flags");
  main_flags ();
  draw_frame (1, 13, 45, 21, "");
  main_display ();
  draw_frame (47, 13, 90, 21, "Keys:  ");
  main_hints ();
}

void
workbykey (enum keys *k)
{
  int last = current;
  int refresh = REFRESHONMOVE;

  if (*k >= 7 && *k <= 10)
    refresh = REFRESHONMOVE;
  else if (*k == enter)
    refresh = REFRESHENTER;
  else if (*k == load || *k == save)
    refresh = REFRESHSAVELOAD;
  else if (*k == reset)
    refresh = REFRESHRESET;

  if (*k == right)
    current++;
  else if (*k == left)
    current--;
  else if (*k == up)
    current -= 10;
  else if (*k == down)
    current += 10;

  if (current < 0)
    current = 0;
  else if (current > 99)
    current = 99;

  if (refresh == REFRESHONMOVE)
    {
      if (last != current)
        {
          main_accumulator ();
          main_printCell (last);
          mt_setbgcolor (darkgrey);
          main_printCell (current);
          main_display ();
          mt_gotoXY (26, 1);
        }
    }
  else if (refresh == REFRESHSAVELOAD)
    {
      int terminal = open (TERMINAL_PATH, O_RDWR);
      char *filename = malloc (30 * sizeof (char));

      // rk_termrestore("DefaultTerm.dat");
      rk_mytermregime (1, 0, 0, 1, 1);
      mt_gotoXY (28, 1);
      if (*k == save)
        write (0, "Save to: ", 10);
      else if (*k == load)
        write (0, "Load from: ", 12);

      scanf ("%s", filename);
      if (*k == save)
        sc_memorySave (filename);
      else if (*k == load)
        {
          sc_memoryLoad (filename);
          for (int i = 0; i < RAM; i++)
            main_printCell (i);
        }
      mt_gotoXY (28, 1);
      write (0, "                                                 ", 50);
      mt_gotoXY (26, 1);
    }
  else if (refresh == REFRESHRESET)
    {
      sc_free ();
      sc_init ();
      sc_regInit ();
      for (int i = 0; i < RAM; i++)
        main_printCell (i);
      main_flags ();
      mt_gotoXY (26, 1);
    }
}

int
main (void)
{
  int terminal = open (TERMINAL_PATH, O_WRONLY);
  FILE *f = fopen ("t.txt", "r+");
  fprintf (f, "%d", terminal);
  fclose (f);
  // rk_termsave("DefaultTerm.dat");
  enum keys key = etc;
  enum keys *k = &key;
  mt_clrscr ();
  sc_init ();
  sc_memorySet (10, 0x7F7F);
  sc_memorySet (0, 1);
  sc_memorySet (5, 32767);
  sc_memorySet (50, 0777);
  sc_memorySet (99, 0xD3F1);
  main_interface ();

  while (1)
    {
      rk_readkey (k);
      workbykey (k);
    }

  return 0;
}