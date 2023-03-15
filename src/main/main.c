#include <myBigChars/myBigChars.h>
#include <mySimpleComputer/mySimpleComputer.h>
#include <myTerm/myTerm.h>
#include <myReadKey/myReadKey.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <memory.h>

#define TERMINAL_PATH "/dev/tty"

int current = 3;

void display_bigchar(int value, int x, int shift)
{
	int bc_id;
	if (x == 2)
		(value & 0x4000) ? (bc_id = 1) : (bc_id = 0);
	else
		bc_id = 2 + ((value >> shift) & 0b1111);
	
	bc_printbigchar (NUMS[bc_id].N, 14, x, green, yellow);
	mt_setbgcolor(darkgrey);
	
}

void draw_frame(int x, int y, int x1, int y1, char* title)
{
  bc_box(y, x, y1 - y, x1 - x);
  mt_gotoXY(y, (x1 + x) / 2 - (strlen(title) / 2));
  printf("%s", title);

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
	snprintf(buf, 6, "%c%02X%02X", (value & 0x4000) ? '-' : '+', command, operand);
	
	mt_gotoXY(2 + row, 2 + col * 6);
	write(1, buf, 5);
	
	if (current == address)
		mt_setbgcolor(darkgrey);
	
	free(buf);
	return;
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
	printf("i\t- reset");
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
	printf("%d", current);
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
	int x = 2;
	int command, operand;
	int value;
	sc_memoryGet(current, &value);
	sc_commandDecode(value, &command, &operand);
	command >>= 1;
	
	display_bigchar(value, x, 0);
	display_bigchar(command, x+=9, 4);
	display_bigchar(command, x+=9, 0);
	display_bigchar(operand, x+=9, 4);
	display_bigchar(operand, x+=9, 0);
}



void main_interface()
{
	draw_frame(1, 1, 60, 11, "Memory");
	for (int i = 0; i < 100; i++)
  		main_printCell(i);
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
}

void workbykey(enum keys* k)
{
	int last = current;
	int refresh = 0;
	
	if (*k == right) {
		current++;
		refresh = 1;
	}
	else if (*k == left) {
		current--;
		refresh = 1;
	} 
	else if (*k == up) {
		current -= 10;
		refresh = 1;
	}
	else if (*k == down) {
		current += 10;
		refresh = 1;
	}
	if (refresh == 1)
	{
		
		main_accumulator();
		main_printCell(last);
		mt_setbgcolor(darkgrey);
		main_printCell(current);
		main_display();
		mt_gotoXY(26, 1);
	}


	// int terminal = open(TERMINAL_PATH, O_WRONLY);
	// write(0, MSG_INPUT )
	// setvbuf(stdout, NULL, _IONBF, 0);
}

int
main (void)
{
	enum keys key = etc;
	enum keys* k = &key;
	mt_clrscr ();
	sc_init();	
	sc_memorySet(10, 0x7F7F);
	sc_memorySet(0, 1);
	sc_memorySet(5, 32767);
	sc_memorySet(50, 0777);
	sc_memorySet(99, 0xD3F1);
	main_interface();

	while (1)
	{
		//main_interface();
		rk_readkey(k);
		workbykey(k);
		
		
		
		
		mt_gotoXY(26, 1);
		//printf("%d", *k);

	}

	
  	return 0;
}
