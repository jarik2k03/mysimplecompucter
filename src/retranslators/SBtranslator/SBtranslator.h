#pragma once
#include <inttypes.h>

int8_t sb_read_program(char *in);
int8_t sb_check_filename(char *in);
int8_t sb_string_check(char *str);
int8_t sb_check_filename(char *in);
int8_t sb_command_check(char *str);
int8_t sb_cell_check(int16_t num);
void sb_reset();

int8_t sb_write(char *num, char *command, char *args);
int8_t sb_write_program(char *in);
int8_t alloc(char *var);
int8_t alloc_memstack(int num);
int8_t free_memstack();
void calc_rpn(char *rpn_expr);
char *infix_to_prefix(char *expr);

int8_t INPUT(char *args);
int8_t PRINT(char *args);
int8_t LET(char *args);
int8_t IF(char *args);
int8_t GOTO(char *args);
int8_t END(char *args);
