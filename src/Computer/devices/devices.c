#include "devices.h"

#include <Computer/myBigChars/myBigChars.h>
#include <Computer/myReadKey/myReadKey.h>
#include <Computer/mySimpleComputer/mySimpleComputer.h>
#include <Computer/myTerm/myTerm.h>
#include <Console/draw/draw.h>
#include <Console/events/events.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern int current;
extern int accumulator;
extern int registr;
extern int counter;

int8_t historyCounter = 0;

void ALU(int command, int operand) {
  int value;
  sc_memoryGet(operand, &value);

  hist_cursor(historyCounter);

  if (command == 0x32 && value == 0) {
    sc_regSet(zero, 1);
    register_event(zero);
    return;
  }

  switch (command) {
    case 0x30:  // ADD
      printf("ADD:: добавляем в аккумулятор = (%d) число = %d.\n", accumulator,
             value);
      accumulator += value;
      break;
    case 0x31:  // SUB
      printf("SUB:: вычитаем из аккумулятора (%d) число = %d.\n", accumulator,
             value);
      accumulator -= value;
      break;
    case 0x32:  // DIVIDE
      printf("DIVIDE:: делим аккумулятор (%d) на число = %d.\n", accumulator,
             value);
      accumulator /= value;
      break;
    case 0x33:  // MUL
      printf("MUL:: умножаем аккумулятор (%d) на число = %d.\n", accumulator,
             value);
      accumulator *= value;
      break;
    default:
      break;
  }

  historyCounter++;
  if (accumulator >= -0x7fff && accumulator < 0x7fff) return;

  sc_regSet(overflow, 1);
  register_event(overflow);
  accumulator = 0x7fff;
  print_accumulator(accumulator);
}

void CU() {
  enum keys next = right;  // шажок вправо.
  int value, command, operand;

  if (sc_memoryGet(counter, &value) == -1) {
    register_event(memoryfault);
    return;
  }

  if (sc_commandDecode(value, &command, &operand) == 0) {
    if (command >= 0x30 && command <= 0x33)
      ALU(command, operand);
    else if (command == 0x0)
      VAR(operand);
    else if (command == 0x10)
      READ(operand);
    else if (command == 0x11)
      WRITE(operand);
    else if (command == 0x20)
      LOAD(operand);
    else if (command == 0x21)
      STORE(operand);
    else if (command == 0x40)
      JUMP(operand);
    else if (command == 0x41)
      JNEG(operand);
    else if (command == 0x42)
      JZ(operand);
    else if (command == 0x43)
      HALT(operand);
    else if (command == 0x53)
      OR(operand);
    else if (command == 0x54)
      XOR(operand);
    counter++;
    move_event(&next);
    rk_mytermregime(0, 1, 0, 0, 1);
    print_counter(counter);
  } else {
    sc_regSet(unknown, 1);
    sc_regSet(ticks, 1);
    register_event(ticks);
    register_event(unknown);
  }
}

void READ(int operand) {
  int buffer;
  rk_mytermregime(0, 0, 1, 1, 1);

  mainpos_cursor();
  write(0, "Input: ", 8);

  scanf("%x", &buffer);
  mainpos_eraser(50);
  if (buffer < 0 && buffer > 0x7fff) return;
  sc_memorySet(operand, buffer);

  print_ccell(operand, buffer, green);
  hist_cursor(historyCounter);
  printf("READ:: записано %d в ячейку: %d.\n", buffer, operand);
  historyCounter++;
}

void WRITE(int operand) {
  int value;
  sc_memoryGet(operand, &value);

  hist_cursor(historyCounter);
  printf("WRITE:: значение %d ячейки = %d.\n", operand, value);
  historyCounter++;
}

void LOAD(int operand) {
  int value;
  sc_memoryGet(operand, &value);
  accumulator = value;

  print_accumulator(accumulator);

  hist_cursor(historyCounter);
  printf("LOAD:: %d значение добавлено в аккумулятор из %d ячейки.\n", value,
         operand);
  historyCounter++;
}
void STORE(int operand) {
  sc_memorySet(operand, accumulator);

  uint8_t color;
  if (accumulator == 0) color = white;
  else if (accumulator == 0x7f) color = red;
  else color = yellow;

  print_ccell(operand, accumulator, color);
  hist_cursor(historyCounter);
  printf("STORE:: %d значение выгружено из аккумулятора в %d ячейку.\n",
         accumulator, operand);
  historyCounter++;
}

void JUMP(int operand) {
  int8_t lastcell = counter;
  counter = operand;
  decode_and_print(lastcell);
  decode_and_print(counter);
  print_counter();
  hist_cursor(historyCounter);
  printf("JUMP:: совершен переход в ячейку %d.\n", operand);
  historyCounter++;
}

void JNEG(int operand) {
  if (accumulator >= 0) return;
  int8_t lastcell = counter;
  counter = operand;
  decode_and_print(lastcell);
  decode_and_print(counter);
  print_counter();
  hist_cursor(historyCounter);
  printf("JNEG:: совершен переход в ячейку %d.\n", operand);
  historyCounter++;
}

void JZ(int operand) {
  if (accumulator != 0) return;
  int8_t lastcell = counter;
  counter = operand;
  decode_and_print(lastcell);
  decode_and_print(counter);
  print_counter();
  hist_cursor(historyCounter);
  printf("JZ:: совершен переход в ячейку %d.\n", operand);
  historyCounter++;
}

void HALT(int operand) {
  sc_regSet(ticks, 1);
  register_event(ticks);

  hist_cursor(historyCounter);
  printf("HALT:: программа завершилась с кодом %d.\n", operand);
  historyCounter = 0;
}

void OR(int operand) {
  int value;
  sc_memoryGet(operand, &value);
  accumulator |= value;

  print_accumulator(accumulator);

  hist_cursor(historyCounter);
  printf("OR:: делаем побитовое ИЛИ аккумулятора (%d) на число = %X.\n",
         accumulator, value);
  historyCounter++;
}

void XOR(int operand) {
  int value;
  sc_memoryGet(operand, &value);
  accumulator ^= value;

  print_accumulator(accumulator);

  hist_cursor(historyCounter);
  printf("OR:: делаем ИСКЛЮЧАЮЩЕЕ ИЛИ аккумулятора (%d) на число = %d.\n",
         accumulator, value);
  historyCounter++;
}

void VAR(int operand) {
  accumulator = operand;

  print_accumulator(accumulator);

  hist_cursor(historyCounter);
  printf("=:: добавляем в аккумулятор число = %d.\n", operand);
  historyCounter++;
}
// sc_files/binary/b.o