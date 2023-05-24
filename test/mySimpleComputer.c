#include <Computer/mySimpleComputer/mySimpleComputer.h>
#include <stdio.h>

#include "../thirdparty/ctest.h"
#define SUCCESS 0
#define FAIL -1

CTEST(memory, init) {
  int expCode = SUCCESS;
  int curCode = sc_init();
  ASSERT_EQUAL(expCode, curCode);
}

CTEST(memory, get) {
  int val = 24;

  int expCode = SUCCESS;
  int curCode = sc_memoryGet(50, &val);
  ASSERT_EQUAL(expCode, curCode);

  expCode = FAIL;
  curCode = sc_memoryGet(110, &val);  // попыткаобращения
  ASSERT_EQUAL(expCode, curCode);
}

CTEST(reg, get) {
  int val = 1;
  int expCode = SUCCESS;
  int curCode = sc_regGet(1, &val);
  ASSERT_EQUAL(expCode, curCode);

  expCode = FAIL;
  curCode = sc_regGet(1, NULL);  // защита "от дурака".
  ASSERT_EQUAL(expCode, curCode);

  val = 0;
  expCode = FAIL;
  curCode = sc_regGet(7, &val);  // диапазон разрядов от 0 до 5.
  ASSERT_EQUAL(expCode, curCode);
}

CTEST(command, encode) {
  int command = 64, operand = 99;
  int value = 0;
  int expValue = 8291;

  int expCode = SUCCESS;
  int curCode = sc_commandEncode(command, operand, &value);
  ASSERT_EQUAL(expCode, curCode);
  ASSERT_EQUAL(expValue, value);  // закодированная команда.

  command = 0x27;
  expCode = FAIL;
  curCode = sc_commandEncode(command, operand,
                             &value);  // не существует такой команды.
  ASSERT_EQUAL(expCode, curCode);

  command = 0x20;
  operand = 0x8f;
  expCode = FAIL;
  curCode =
      sc_commandEncode(command, operand, &value);  // операнд превышает 0x7f.
  ASSERT_EQUAL(expCode, curCode);
}

CTEST(command, decode) {
  int expCommand = 64, expOperand = 99;
  int command = 0, operand = 0;
  int value = 8291;

  int expCode = SUCCESS;
  int curCode = sc_commandDecode(value, &command, &operand);
  // printf("%x :: %d\n", command, operand);
  ASSERT_EQUAL(expCode, curCode);
  ASSERT_EQUAL(expCommand, command);  // раскодированная команда.
  ASSERT_EQUAL(expOperand, operand);  // полученный операнд.

  command = 27;
  expCode = FAIL;
  curCode = sc_commandDecode(value, NULL, &operand);  // защита от "дурака".
  ASSERT_EQUAL(expCode, curCode);
}
