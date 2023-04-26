#pragma once
#include <inttypes.h>

int8_t read_program (char *in);
int8_t write_program (char *in);
int8_t check_filename (char *in);
int8_t write_to_memory (int16_t *num, char *command, int16_t *operand);
int8_t save_memory (char *in);
int8_t string_check (char *str, int16_t *num, char *command, int16_t *operand);
int8_t comment_check (char *com);
int8_t command_check (char *command);
int8_t operand_check (int16_t operand, char *command);
int8_t cell_check (int16_t num);
int8_t init_commands ();