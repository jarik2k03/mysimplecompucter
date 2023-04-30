#pragma once
#include <inttypes.h>

int8_t sb_read_program (char *in);
int8_t sb_check_filename (char *in);
int8_t sb_string_check (char *str);
int8_t sb_check_filename (char *in);
int8_t sb_command_check (char *str);
int8_t sb_cell_check (int16_t num, int16_t lastnum);

int8_t sb_write (int16_t num, char *command, char *args);
int8_t sb_write_program (char *in);
int8_t alloc (char var);
int8_t INPUT (char *args);
int8_t PRINT (char *args);
int8_t LET (char *args);
char preCalcProcessing (char *args);
// int8_t calc (char *args, int8_t i);