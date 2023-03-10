#pragma once

#include <myTerm/myTerm.h>

typedef struct NUM
{
  int N[2];
} NUM;

static NUM NUMS[] = { /* + */ { .N[0] = 0xFF181818, .N[1] = 0x181818FF },
                      /* - */ { .N[0] = 0xFF000000, .N[1] = 0x000000FF },
                      /* 0 */ { .N[0] = 0x8181817e, .N[1] = 0x7e818181 },
                      /* 1 */ { .N[0] = 0x8890A0C0, .N[1] = 0x80808080 },
                      /* 2 */ { .N[0] = 0x2040827C, .N[1] = 0xFE040810 },
                      /* 3 */ { .N[0] = 0x6080817E, .N[1] = 0x7E818060 },
                      /* 4 */ { .N[0] = 0xFF818181, .N[1] = 0x80808080 },
                      /* 5 */ { .N[0] = 0x7F0101FF, .N[1] = 0x7F808080 },
                      /* 6 */ { .N[0] = 0x0101817E, .N[1] = 0x7E81817F },
                      /* 7 */ { .N[0] = 0x204080FE, .N[1] = 0x02040810 },
                      /* 8 */ { .N[0] = 0x7E81817E, .N[1] = 0x7E818181 },
                      /* 9 */ { .N[0] = 0x7E81817E, .N[1] = 0x7E808080 },
                      /* A */ { .N[0] = 0x7E42423C, .N[1] = 0x42424242 },
                      /* B */ { .N[0] = 0x3E42423E, .N[1] = 0x3E424242 },
                      /* C */ { .N[0] = 0x0101017E, .N[1] = 0x7E010101 },
                      /* D */ { .N[0] = 0x4242221E, .N[1] = 0x1E224242 },
                      /* E */ { .N[0] = 0x7E02027E, .N[1] = 0x7E020202 },
                      /* F */ { .N[0] = 0x7E02027E, .N[1] = 0x02020202 } };

int bc_printA (char *str);
int bc_box (int x1, int y1, int x2, int y2);
int bc_printNL ();
int bc_printUB (int len);
int bc_printLB (int len);
int bc_printES (int len);
int bc_printbigchar (int a[2], int x, int y, enum colors front,
                     enum colors back);
int bc_setbigcharpos (int *big, int x, int y, int value);
int bc_getbigcharpos (int *big, int x, int y, int *value);
int bc_bigcharwrite (int fd, int *big, int count);
int bc_bigcharread (int fd, int *big, int need_count, int *count);
