#include <ctype.h>
#include <draw/draw.h>
#include <memory.h>
#include <myBigChars/myBigChars.h>
#include <myReadKey/myReadKey.h>
#include <mySimpleComputer/mySimpleComputer.h>
#include <myTerm/myTerm.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/time.h>

#define RAM 100

int current = 0;
int accumulator = 0x0000;
int registr = 0;
int counter = 0x0000;

struct itimerval nval = {.it_interval.tv_sec = 2,
                           .it_interval.tv_usec = 0,
                           .it_value.tv_sec = 1,
                           .it_value.tv_usec = 0},
                 oval = {.it_interval.tv_sec = 0,
                           .it_interval.tv_usec = 0,
                           .it_value.tv_sec = 0,
                           .it_value.tv_usec = 0};

size_t rdwrreturn;

void ALU(int command, int operand);
void event_listener (enum keys *k);
void run_event (enum keys* k);

void
decode_and_print (int address)
{
  int value;
  int command, operand;
  sc_memoryGet (address, &value);

  sc_commandDecode (value, &command, &operand);
  command >>= 1;

  if (address == current)
    mt_setbgcolor (green);

  print_cell (address, value, command, operand);
  mt_setbgcolor (darkgrey);
}

void
decode_and_display_bc_ (int lastaddress, int address)
{
  int value, lastvalue;
  int command, operand;
  sc_memoryGet (address, &value);
  sc_memoryGet (lastaddress, &lastvalue);
  if (lastvalue == value)
    return;
  sc_commandDecode (value, &command, &operand);
  command >>= 1;
  print_display (value, command, operand);
}

void
decode_and_display_bc (int address)
{
  int value;
  int command, operand;
  sc_memoryGet (address, &value);

  sc_commandDecode (value, &command, &operand);
  command >>= 1;
  print_display (value, command, operand);
}

void
operation_event (int address)
{
  int value;
  int command, operand;
  sc_memoryGet (address, &value);
  sc_commandDecode (value, &command, &operand);
  command >>= 1;
  char sign = (value & 0x4000) ? '-' : '+';
  print_operation (sign, command, operand);
}

void
register_event (int address)
{
  int value;
  sc_regGet (address, &value);
  print_regCell (address, value);
}

void
counter_event()
{
  int buffer;
  rk_mytermregime (0, 0, 1, 1, 1);
  mt_gotoXY (28, 1);
  rdwrreturn = write (0, "Set counter: ", 14);

  rdwrreturn = scanf ("%x", &buffer);

  if (buffer < 0xffff && buffer >= 0)
    {
      counter = buffer;
      print_counter (counter);
    }
  input_eraser (50);
  mainpos_cursor ();

}

void
enter_event ()
{
  char *request = malloc (30 * sizeof (char));
  // int address, value;
  // char operation;
  rk_mytermregime (1, 0, 1, 1, 1);
  mt_gotoXY (28, 1);
  rdwrreturn = write (0, "Set cell: ", 11);
  rdwrreturn = read (0, request, 30);

  mt_gotoXY (29, 1);
  // printf("%s %c %s", caddress, operation, cvalue);
  input_eraser (50);
  mainpos_cursor ();
}

void
move_event (enum keys *k)
{
  int last = current;
  switch (*k)
    {
    case right:
      current++;
      break;
    case left:
      current--;
      break;
    case up:
      current -= 10;
      break;
    case down:
      current += 10;
      break;
    default:
      break;
    }

  if (current < 0)
    current = 0;
  else if (current > 99)
    current = 99;

  if (last != current)
    {
      decode_and_print (last);
      decode_and_print (current);
      decode_and_display_bc_ (last, current);
      operation_event (current);
      mainpos_cursor ();
    }
}

void
saveload_event (enum keys *k)
{
  char *filename = malloc (30 * sizeof (char));
  rk_mytermregime (1, 0, 1, 1, 1);
  mt_gotoXY (28, 1);
  if (*k == save)
    rdwrreturn = write (0, "Save to: ", 10);
  else if (*k == load)
    rdwrreturn = write (0, "Load from: ", 12);
  rdwrreturn = read (0, filename, 30);
  uint8_t endstr = strlen (filename) - 1;
  ;
  filename[endstr] = '\0';

  if (*k == save)
    {
      if (sc_memorySave (filename) == -1)
        erropenfile ();
    }
  else if (*k == load)
    {
      if (sc_memoryLoad (filename) == -1)
        erropenfile ();
      for (int i = 0; i < RAM; i++)
        decode_and_print (i);
      decode_and_display_bc (current);
    }
  free (filename);
  input_eraser (50);
  mainpos_cursor ();
}

void
accumulator_event ()
{
  int buffer;
  rk_mytermregime (0, 0, 1, 1, 1);
  mt_gotoXY (28, 1);
  rdwrreturn = write (0, "Set accumulator: ", 18);

  rdwrreturn = scanf ("%x", &buffer);

  if (buffer < 0xffff && buffer >= 0)
    {
      accumulator = buffer;
      print_accumulator (accumulator);
    }
  input_eraser (50);
  mainpos_cursor ();
}

void
reset_event ()
{
  sc_free ();
  sc_init ();
  sc_regInit ();
  accumulator = 0;
  current = 0;
  registr = 0;
  counter = 0;
  print_accumulator (accumulator);
  operation_event (0);
  print_counter(counter);
  for (int i = 0; i < 4; i++)
    register_event (i);
  for (int i = 0; i < RAM; i++)
    decode_and_print (i);
  decode_and_display_bc (current);
  mainpos_cursor ();
}

void
all_events ()
{
  print_interface ();
  for (uint8_t i = 0; i < RAM; i++)
    decode_and_print (i);
  for (int i = 0; i < 4; i++)
    register_event (i);

  print_counter ();
  print_accumulator (accumulator);
  decode_and_display_bc (current);
  operation_event (current);
  print_counter(counter);
}


void CU (int signum)
{
  enum keys next = right; // шажок вправо.
  int value, command, operand;
  sc_memoryGet(counter + 1, &value);
  if (sc_commandDecode(value, &command, &operand) == 0 || (value & 0x4000) == 0) {
     if (command >= 30 && command <= 33)
     	ALU(command, operand);
     move_event(&next);
     counter++;
     print_counter(counter);  
  
  }
  else {
    sc_regSet(3, 1); // unknown command
    sc_regSet(4, 1); // ignore n
    register_event(3);
    register_event(4);
    return ;
  }
  return ;
  
  
}

void
run_event (enum keys* k)
{
  
  int value;
  int ignore;
  int oldcell = current;
  current = counter;
  
  sc_regGet(3, &ignore); // 3 - ignore N
  signal(SIGALRM, *CU);
  setitimer(ITIMER_REAL, &nval, &oval); // start.
  decode_and_print(oldcell);
    for (;;) 
      {
        sc_memoryGet(counter, &value);
        if (value & 0x4000) {
          alarm(0);
	  break;	        
        }
        pause();
        setitimer(ITIMER_REAL, &nval, &oval); // start.
      }
    if (counter >= RAM) {
      sc_regSet(4, 1); // out from mem.
      register_event(4);
    }

    
} 

void
event_listener (enum keys *k)
{
  if (*k >= 7 && *k <= 10)
    move_event (k);
  else if (*k == f6)
    counter_event ();
  else if (*k == load || *k == save)
    saveload_event (k);
  else if (*k == reset)
    reset_event ();
  else if (*k == f5)
    accumulator_event ();
  else if (*k == run)
    run_event (k);
}

void ALU(int command, int operand) 
{
  int cell;
  sc_memoryGet(operand, &cell);
  
  switch (command) {
    case 0x30: // ADD
      accumulator += cell & 0x3fff;
      break;
    case 0x31: // SUB
      accumulator -= cell & 0x3fff;
      break;
    case 0x32: // DIVIDE
      accumulator /= cell & 0x3fff;
      break;
    case 0x33: // SUB
      accumulator *= cell & 0x3fff;
      break;
    default:
      break;
  }
  print_accumulator(accumulator);
  print_counter(counter);
  mainpos_cursor ();
}





int
main (void)
{
  enum keys k;
  int cols, rows;
  u_int8_t wasSmall = 1;
  mt_clrscr ();
  sc_init ();
  // sc_memoryRand();
  while (1)
    {
      mt_getscreensize (&rows, &cols);
      if (rows >= 32 && cols >= 96)
        {
          if (wasSmall == 1)
            {
              all_events ();
              mainpos_cursor ();
              wasSmall = 0;
            }
          rk_readkey (&k);
          event_listener (&k);
        }
      else
        {
          wasSmall = 1;
          mt_clrscr ();
          printf ("РАЗВЕРНИТЕ ПРИЛОЖЕНИЕ НА ПОЛНЫЙ ЭКРАН!");
          getchar ();
        }
    }
  sc_free ();
  return 0;
}
