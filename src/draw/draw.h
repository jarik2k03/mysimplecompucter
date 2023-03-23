#pragma once
#include <inttypes.h>

void display_bigchar(int value, int x, int shift);
void draw_frame(int x, int y, int x1, int y1, char* title);
void print_regCell(int address, uint8_t value);
void main_accumulator(int32_t accumulator);
void main_counter();
void main_operation(int command, int operand);
void main_display(int value, int command, int operand);
void main_interface();
void main_printCell(int address, int value, int command, int operand);
void erropenfile();
void mainpos_cursor();
void input_eraser(int length);


