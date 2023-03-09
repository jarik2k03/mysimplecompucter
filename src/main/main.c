#include <stdio.h>
#include <myTerm/myTerm.h>
#include <mySimpleComputer/mySimpleComputer.h>

int
main (void)
{
  int row, col;
  mt_clrscr ();

  mt_setfgcolor (green);
  mt_setbgcolor (red);
  mt_getscreensize (&row, &col);
  printf ("Размер окна: %d %d\n", row, col);
  mt_setbgcolor (white);

  mt_gotoXY (10, 10);
}
