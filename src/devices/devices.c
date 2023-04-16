#include "devices.h"

#include <ctype.h>
#include <draw/draw.h>
#include <events/events.h>
#include <myBigChars/myBigChars.h>
#include <myReadKey/myReadKey.h>
#include <mySimpleComputer/mySimpleComputer.h>
#include <myTerm/myTerm.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define RAM 100

int current = 0;
int accumulator = 0x0000;
int registr = 0;
int counter = 0x0000;

void
get_data (int *cur, int *accum, int *reg, int *countr)
{
  *cur = current;
  *accum = accumulator;
  *reg = registr;
  *countr = counter;
}

void
ALU (int command, int operand)
{
  int cell;
  sc_memoryGet (operand, &cell);

  switch (command)
    {
    case 0x30: // ADD
      accumulator += cell & 0x3fff;
      break;
    case 0x31: // SUB
      accumulator -= cell & 0x3fff;
      break;
    case 0x32: // DIVIDE
      accumulator /= cell & 0x3fff;
      break;
    case 0x33: // MUL
      accumulator *= cell & 0x3fff;
      break;
    default:
      break;
    }
  print_accumulator (accumulator);
  print_counter (counter);
  mainpos_cursor ();
}

void
CU ()
{
  enum keys next = right; // шажок вправо.
  int value, command, operand;
  counter++;
  if (sc_memoryGet (counter, &value) == -1)
    {
      register_event (memoryfault);
      counter--;
      return;
    }

  if (sc_commandDecode (value, &command, &operand) == 0)
    {
      if (command >= 30 && command <= 33)
        ALU (command, operand);
      move_event (&next);

      print_counter (counter);
    }
  else
    {
      sc_regSet (unknown, 1);
      sc_regSet (ticks, 1);
      register_event (ticks);
      register_event (unknown);
    }
}