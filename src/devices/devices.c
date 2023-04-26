#include "devices.h"

#include <draw/draw.h>
#include <events/events.h>
#include <malloc.h>
#include <myBigChars/myBigChars.h>
#include <myReadKey/myReadKey.h>
#include <mySimpleComputer/mySimpleComputer.h>
#include <myTerm/myTerm.h>

#include <stdio.h>
#include <string.h>
#include <unistd.h>

extern int current;
extern int accumulator;
extern int registr;
extern int counter;

void
ALU (int command, int operand)
{
  int cell;
  sc_memoryGet (operand, &cell);

  switch (command)
    {
    case 30: // ADD
      accumulator += cell & 0x3fff;
      break;
    case 31: // SUB
      accumulator -= cell & 0x3fff;
      break;
    case 32: // DIVIDE
      accumulator /= cell & 0x3fff;
      break;
    case 33: // MUL
      accumulator *= cell & 0x3fff;
      break;
    default:
      break;
    }
  print_accumulator (accumulator);
  print_counter (counter);
  // read_program ();
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