#pragma once
#include <inttypes.h>

int read_program (char *in);
int8_t check_filename (char *in);
int8_t string_check (char *str, int16_t *num, char *command, int16_t *operand);
int8_t comment_check (char *com);
int8_t command_check (char *command);
int8_t init_commands ();