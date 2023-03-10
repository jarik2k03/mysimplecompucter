#include <myBigChars/myBigChars.h>
#include <mySimpleComputer/mySimpleComputer.h>
#include <myTerm/myTerm.h>
#include <stdio.h>

int
main (void)
{
  mt_clrscr ();
  bc_printA ("aqklklklqwlkjxaxaaxxa");

  bc_box (2, 12, 8, 64);
  bc_printNL ();
  int SPACE[] = { 0, 0 };
  int x = 3, y = 13;
  bc_printbigchar (NUMS[13].N, x, y, red, yellow);
  bc_printbigchar (NUMS[12].N, x, y += 8, red, yellow);
  bc_printbigchar (NUMS[15].N, x, y += 8, red, yellow);
  bc_printbigchar (SPACE, x, y += 8, red, yellow);
  bc_printbigchar (NUMS[13].N, x, y += 8, red, yellow);
  bc_printbigchar (NUMS[16].N, x, y += 8, red, yellow);
  bc_printbigchar (NUMS[16].N, x, y += 8, red, yellow);
  bc_printbigchar (NUMS[17].N, x, y += 8, red, yellow);
  bc_printNL ();
}
