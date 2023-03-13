#include <myBigChars/myBigChars.h>
#include <mySimpleComputer/mySimpleComputer.h>
#include <myTerm/myTerm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int current = 5;

void draw_frame(int x, int y, int x1, int y1, char* title)
{
  bc_box(y, x, y1 - y, x1 - x);
  mt_gotoXY(y, (x1 + x) / 2 - (strlen(title) / 2));
  printf("%s", title);

}

void main_hints()
{
	int x = 14, y = 48;
	mt_gotoXY(x, y);
	printf("l\t- load");
	mt_gotoXY(x++, y);
	printf("s\t- save");
	mt_gotoXY(x++, y);
	printf("r\t- run");
	mt_gotoXY(x++, y);
	printf("t\t- step");
	mt_gotoXY(x++, y);
	printf("r\t- reset");
	mt_gotoXY(x++, y);
	printf("F5\t- accumulator");
	mt_gotoXY(x++, y);
	printf("F6\t- instructionCounter");
}

void main_flags()
{
	mt_gotoXY(11, 74);
	printf("O E V M");
}

void main_accumulator()
{
	mt_gotoXY(2, 74);
	printf("+9999");
}

void main_counter()
{
	mt_gotoXY(5, 74);
	printf("+0000");
}

void main_operation()
{
	mt_gotoXY(8, 74);
	printf("+00 : 00");
}

void main_display()
{
	int x = 2, y = 14;
	mt_gotoXY(y, x);
	int bc_id;
	/*bc_printbigchar (NUMS[0].N, y, x, red, yellow);
	for (int i = 0; i < 4; i++) {
		bc_printbigchar (NUMS[2 + (rand() % 16)].N, y, x += 9, red, yellow);
	}*/
	int command, operand;
	int value;
	sc_memoryGet(current, &value);
	
	
	
	(value & 0x4000) ? (bc_id = 1) : (bc_id = 0);
	bc_printbigchar (NUMS[bc_id].N, y, x, green, yellow);
	int q = 30;
	mt_gotoXY(30, 10);
	printf("%x\t", value);
	sc_commandDecode(value, &command, &operand);
	command >>= 1;
	
	
	for (int shift = 4; shift != -4; shift -= 4)
	{
		bc_id = 2 + ((command >> shift) & 0b1111);
		mt_gotoXY(30, q += 5);
		printf("%d\t", bc_id);
		bc_printbigchar (NUMS[bc_id].N, y, x += 9, green, yellow);
	}
	for (int shift = 4; shift != -4; shift -= 4)
	{
		bc_id = 2 + ((operand >> shift) & 0b1111);
		mt_gotoXY(30, q += 5);
		printf("%d\t", bc_id);
		bc_printbigchar (NUMS[bc_id].N, y, x += 9, green, yellow);
	}
	
	mt_setbgcolor(darkgrey);
	/*
	int operand, command;
	sc_commandDecode(value, &command, &operand);
	
	int v;
	sc_commandEncode(command, operand, &v);
	
	mt_gotoXY(30, q += 5);
	printf("%x %x\t", command, operand);
	printf("%x", v);
	*/

	
		//bc_printbigchar (NUMS[bc_id].N, y, x += 9, red, yellow);
	
		
		
	
	
	
	
	
}

void main_interface()
{
	int x, y;
	x = 60, y = 10;
	draw_frame(1, 1, 60, 11, "Memory");
	draw_frame(63, 1, 90, 2, "accumulator");
	main_accumulator();
	draw_frame(63, 4, 90, 5, "instructionCounter");
	main_counter();
	draw_frame(63, 7, 90, 8, "Operation");
	main_operation();
	draw_frame(63, 10, 90, 11, "Flags");
	main_flags();
	draw_frame(1, 13, 45, 21, "");
	main_display();
	draw_frame(47, 13, 90, 21, "Keys:  ");
	main_hints();
	/*bc_box (1, 1, y, x);
	mt_gotoXY(1, x / 2);
	printf("Memory");*/
	
	
	
}


void main_printCell(int address)
{
	int value, row, col, command, operand;
	char* buf = malloc(6 * sizeof(char));
	if (sc_memoryGet(address, &value) != 0 || sc_commandDecode(value, &command, &operand) != 0)
		return;
	row = address / 10;
	col = address % 10;
	if (current == address)
		mt_setbgcolor(green);
		
	
	sc_commandDecode(value, &command, &operand);
	command >>= 1;
	//operand >>= 1;

	
	snprintf(buf, 6, "%c%02X%02X", (value & 0x4000) ? '-' : '+', command, operand);
	
	mt_gotoXY(2 + row, 2 + col * 6);
	write(1, buf, 5);
	
	if (current == address)
		mt_setbgcolor(darkgrey);
	
	free(buf);
	return;
}

int
main (void)
{
  mt_clrscr ();
  sc_init();
  
  
  sc_memorySet(10, 0x7F7F);
  sc_memorySet(0, 1);
  sc_memorySet(5, 32767);
  sc_memorySet(50, 0777);
  sc_memorySet(99, 0xD3F1);
  
  main_interface();
  for (int i = 0; i < 100; i++)
  	main_printCell(i);
  mt_gotoXY(26, 1);
  
  

  return 0;
  /*
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
  */
  


}
