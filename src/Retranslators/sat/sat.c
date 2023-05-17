#include <Retranslators/SAtranslator/SAtranslator.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[]) {
  if (argc < 2) {
    printf("Use: ./sat <inputfile>\n");
    return 0;
  }

  char input[30];
  strcpy(input, argv[1]);
  if (sa_read_program(input) != -1) sa_write_program(input);
  return 0;
}
