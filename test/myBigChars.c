#include "../thirdparty/ctest.h"
#include <myBigChars/myBigChars.h>
#include <myTerm/myTerm.h>
#define SUCCESS 0
#define FAIL -1

CTEST (chars, printA)
{
  int expCode = SUCCESS;
  int curCode = bc_printA ("klqw");
  ASSERT_EQUAL (expCode, curCode);
}

CTEST (chars, bc_box)
{
  int expCode = SUCCESS;
  int curCode = bc_box(0, 0, 12, 45);
  ASSERT_EQUAL (expCode, curCode);

  expCode = FAIL;
  curCode = bc_box(-1, 0, -12, 45); // только положительные координаты.
  ASSERT_EQUAL (expCode, curCode);

}

CTEST (chars, bc_setbigcharpos)
{
  int expCode = SUCCESS;
  int curCode = bc_setbigcharpos(NUMS[10].N, 6, 4, 0);
  ASSERT_EQUAL (expCode, curCode);

  expCode = FAIL;
  curCode = bc_setbigcharpos(NUMS[12].N, 10, -1, 1); // диапазон бит в пределах 8.
  ASSERT_EQUAL (expCode, curCode);

  expCode = FAIL;
  curCode = bc_setbigcharpos(NUMS[12].N, 2, 2, 6); // значение от 0 до 1
  ASSERT_EQUAL (expCode, curCode);

  expCode = FAIL;
  curCode = bc_setbigcharpos(NULL, 2, 2, 6); // необходим символ.
  ASSERT_EQUAL (expCode, curCode);
}

CTEST (chars, bc_getbigcharpos)
{
  int value; 

  int expCode = SUCCESS;
  int curCode = bc_getbigcharpos(NUMS[5].N, 6, 4, &value);
  ASSERT_EQUAL (expCode, curCode);

  expCode = FAIL;
  curCode = bc_getbigcharpos(NUMS[5].N, 6, 4, NULL); // нулль нельзя передавать.
  ASSERT_EQUAL (expCode, curCode);

  expCode = FAIL;
  curCode = bc_getbigcharpos(NUMS[5].N, 20, 10, &value); // диапазон бит до 8.
  ASSERT_EQUAL (expCode, curCode);

  expCode = FAIL;
  curCode = bc_getbigcharpos(NULL, 4, 3, &value); // большой символ требуется.
  ASSERT_EQUAL (expCode, curCode);
}
