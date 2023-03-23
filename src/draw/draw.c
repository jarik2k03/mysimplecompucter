#include "draw.h"
#include <myTerm/myTerm.h>
#include <mySimpleComputer/mySimpleComputer.h>
#include <myBigChars/myBigChars.h>
#include <myReadKey/myReadKey.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

void display_bigchar(int value, int x, int shift)
{
    int bc_id;
    if (x == 2)
        (value & 0x4000) ? (bc_id = 1) : (bc_id = 0);
    else
        bc_id = 2 + ((value >> shift) & 0b1111);

    bc_printbigchar(NUMS[bc_id].N, 14, x, green, yellow);
    mt_setbgcolor(darkgrey);
}

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
    printf("i\t- reset");
    mt_gotoXY(x++, y);
    printf("F5\t- accumulator");
    mt_gotoXY(x++, y);
    printf("F6\t- instructionCounter");
}


void print_regCell(int address, uint8_t value)
{
  if (address < 0 || address > 3)
    return;
    
  char regs[] = "OEVM";
  mt_gotoXY(11, 74 + address);
  if (value == 1) 
    mt_setbgcolor(green);
  printf("%c", *(regs + address));
  mt_setbgcolor(darkgrey);
}


void main_accumulator(int32_t accumulator)
{
    mt_gotoXY(2, 74);
    char sign = (accumulator & 0x4000) ? '-' : '+';
    printf("%c%04X", sign, accumulator);
}

void main_counter()
{
    mt_gotoXY(5, 74);
    printf("+0000");
}

void main_operation(int command, int operand)
{
    mt_gotoXY(8, 74);
    printf("+%X : %X", command, operand);
}

void main_display(int value, int command, int operand)
{
    int x = 2;
    display_bigchar(value, x, 0);
    display_bigchar(command, x += 9, 4);
    display_bigchar(command, x += 9, 0);
    display_bigchar(operand, x += 9, 4);
    display_bigchar(operand, x += 9, 0);
}

void main_interface()
{
    draw_frame(1, 1, 60, 11, "Memory");
    draw_frame(63, 1, 90, 2, "accumulator");
    draw_frame(63, 4, 90, 5, "instructionCounter");
    draw_frame(63, 7, 90, 8, "Operation");
    draw_frame(63, 10, 90, 11, "Flags");
    draw_frame(1, 13, 45, 21, "");
    draw_frame(47, 13, 90, 21, "Keys:  ");
    main_hints();
}

void main_printCell(int address, int value ,int command, int operand)
{
    int row, col;
    char* buf = malloc(6 * sizeof(char));

    row = address / 10;
    col = address % 10;

    snprintf(buf, 6, "%c%02X%02X", (value & 0x4000) ? '-' : '+', command,
        operand);

    mt_gotoXY(2 + row, 2 + col * 6);
    write(1, buf, 5);

    free(buf);
    return;
}

void erropenfile()
{
    mt_gotoXY(28, 1);
    mt_setbgcolor(red);
    write(0, "Указанный файл не найден или поврежден!", 73);
    mt_setbgcolor(darkgrey);
    sleep(1);
    mt_gotoXY(28, 1);
}


void mainpos_cursor()
{
    mt_gotoXY(26, 1);
}

void input_eraser(int length)
{
    char* buffer = malloc(length * sizeof(char));
    for (int i = 0; i < length; i++)
      *(buffer + i) = ' ';
    
    mt_gotoXY(28, 1);
    write(0, buffer, length);
    free(buffer);
}