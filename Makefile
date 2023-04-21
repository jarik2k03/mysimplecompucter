MAIN = main
TERM = myTerm
COMP = mySimpleComputer
CHAR = myBigChars
KEYS = myReadKey
DRAW = draw
EVNT = events
DVCS = devices
SATR = retranslators/SAtranslator
SBTR = retranslators/SBtranslator

TEST = test
TESTCOMP = testmySimpleComputer
TESTTERM = testmyTerm
TESTCHAR = testmyBigChars
TESTKEYS = testmyReadKey
DEBUG = -g3 -O0
MD = mkdir
LIBS = -l$(COMP) -l$(TERM) -l$(CHAR) -l$(KEYS) -l$(DRAW) -l$(EVNT) -l$(DVCS)

# CFLAGS =  -Wall -Wextra -Werror -Wcomments -Wdeprecated -Wno-pragmas -Wstrict-overflow=5 -Wchar-subscripts -Wmultistatement-macros -Wparentheses -Warray-bounds=2 -fdiagnostics-show-option
CPPFLAGS = -I src -MP -MMD
GCC = gcc
BIN = bin
OBJ = obj
SRC = src
C = c

# main
MAIN_EXE = $(BIN)/$(MAIN).exe
# статические либы.
TERM_LIB = $(OBJ)/$(SRC)/lib$(TERM).a
COMP_LIB = $(OBJ)/$(SRC)/lib$(COMP).a
CHAR_LIB = $(OBJ)/$(SRC)/lib$(CHAR).a
KEYS_LIB = $(OBJ)/$(SRC)/lib$(KEYS).a
DRAW_LIB = $(OBJ)/$(SRC)/lib$(DRAW).a
EVNT_LIB = $(OBJ)/$(SRC)/lib$(EVNT).a
DVCS_LIB = $(OBJ)/$(SRC)/lib$(DVCS).a
# тесты.
TESTCOMP_EXE = $(BIN)/$(TESTCOMP).exe
TESTTERM_EXE = $(BIN)/$(TESTTERM).exe
TESTCHAR_EXE = $(BIN)/$(TESTCHAR).exe
TESTKEYS_EXE = $(BIN)/$(TESTKEYS).exe

MAIN_SOURCES = $(shell find $(SRC)/$(MAIN) -name '*.$(C)')
MAIN_OBJECTS = $(MAIN_SOURCES:$(SRC)/%.$(C)=$(OBJ)/$(SRC)/%.o)

TERM_SOURCES = $(shell find $(SRC)/$(TERM) -name '*.$(C)')
TERM_OBJECTS = $(TERM_SOURCES:$(SRC)/%.$(C)=$(OBJ)/$(SRC)/%.o)

COMP_SOURCES = $(shell find $(SRC)/$(COMP) -name '*.$(C)')
COMP_OBJECTS = $(COMP_SOURCES:$(SRC)/%.$(C)=$(OBJ)/$(SRC)/%.o)

CHAR_SOURCES = $(shell find $(SRC)/$(CHAR) -name '*.$(C)')
CHAR_OBJECTS = $(CHAR_SOURCES:$(SRC)/%.$(C)=$(OBJ)/$(SRC)/%.o)

KEYS_SOURCES = $(shell find $(SRC)/$(KEYS) -name '*.$(C)')
KEYS_OBJECTS = $(KEYS_SOURCES:$(SRC)/%.$(C)=$(OBJ)/$(SRC)/%.o)

DRAW_SOURCES = $(shell find $(SRC)/$(DRAW) -name '*.$(C)')
DRAW_OBJECTS = $(DRAW_SOURCES:$(SRC)/%.$(C)=$(OBJ)/$(SRC)/%.o)

EVNT_SOURCES = $(shell find $(SRC)/$(EVNT) -name '*.$(C)')
EVNT_OBJECTS = $(EVNT_SOURCES:$(SRC)/%.$(C)=$(OBJ)/$(SRC)/%.o)

DVCS_SOURCES = $(shell find $(SRC)/$(DVCS) -name '*.$(C)')
DVCS_OBJECTS = $(DVCS_SOURCES:$(SRC)/%.$(C)=$(OBJ)/$(SRC)/%.o)

TESTCOMP_SOURCES = $(TEST)/$(MAIN).$(C) $(TEST)/$(COMP).$(C)
TESTCOMP_OBJECTS = $(TESTCOMP_SOURCES:$(TEST)/%.c=$(OBJ)/$(TEST)/%.o)

TESTTERM_SOURCES = $(TEST)/$(MAIN).$(C) $(TEST)/$(TERM).$(C)
TESTTERM_OBJECTS = $(TESTTERM_SOURCES:$(TEST)/%.c=$(OBJ)/$(TEST)/%.o)

TESTCHAR_SOURCES = $(TEST)/$(MAIN).$(C) $(TEST)/$(CHAR).$(C)
TESTCHAR_OBJECTS = $(TESTCHAR_SOURCES:$(TEST)/%.c=$(OBJ)/$(TEST)/%.o)

TESTKEYS_SOURCES = $(TEST)/$(MAIN).$(C) $(TEST)/$(KEYS).$(C)
TESTKEYS_OBJECTS = $(TESTKEYS_SOURCES:$(TEST)/%.c=$(OBJ)/$(TEST)/%.o)

# создание объектных файлов.
$(OBJ)/%.o: %.c
	@echo "\r\033[0;36m\r"
	$(GCC) $(CFLAGS) $(DEBUG) $(CPPFLAGS) -c -o $@ $<

# make (Первая зависимость - выполняет всё.)
.PHONY: $(MAIN) 
$(MAIN): $(MD) $(MAIN_EXE) tests

# make all - форматирование, компиляция и запуск в одной команде!
.PHONY: all
all: format $(MAIN) run

# добавление папок для бинарников (bin & obj)
.PHONY: $(MD)
mkdir:
	@echo "\033[1;35m----СОЗДАНИЕ ПАПОК----"
	mkdir -p $(BIN)
	mkdir -p $(OBJ)/$(TEST)
	mkdir -p $(OBJ)/$(SRC)/$(TERM)
	mkdir -p $(OBJ)/$(SRC)/$(CHAR)
	mkdir -p $(OBJ)/$(SRC)/$(COMP)
	mkdir -p $(OBJ)/$(SRC)/$(KEYS)
	mkdir -p $(OBJ)/$(SRC)/$(MAIN)
	mkdir -p $(OBJ)/$(SRC)/$(DRAW)
	mkdir -p $(OBJ)/$(SRC)/$(EVNT)
	mkdir -p $(OBJ)/$(SRC)/$(DVCS)
	mkdir -p $(OBJ)/$(SRC)/$(SATR)
	mkdir -p $(OBJ)/$(SRC)/$(SBTR)
	
# сборка программы со всеми библиотеками.
$(MAIN_EXE): $(MAIN_OBJECTS) $(COMP_LIB) $(TERM_LIB) $(CHAR_LIB) $(KEYS_LIB) $(DRAW_LIB) $(EVNT_LIB) $(DVCS_LIB)
	@echo "\033[1;31m----СБОРКА КОМПЬЮТЕРА----"
	$(GCC) $(CFLAGS) $(DEBUG) $(CPPFLAGS) -o $@ $<  obj/src/libdraw.a -L$(OBJ)/$(SRC) $(LIBS)

# создание статических библиотек.
$(COMP_LIB): $(COMP_OBJECTS)
	@echo -n "\r\033[0;33m\r"
	ar rcs $@ $^
$(TERM_LIB): $(TERM_OBJECTS)
	@echo -n "\r\033[0;33m\r"
	ar rcs $@ $^
$(CHAR_LIB): $(CHAR_OBJECTS)
	@echo -n "\r\033[0;33m\r"
	ar rcs $@ $^	
$(KEYS_LIB): $(KEYS_OBJECTS)
	@echo -n "\r\033[0;33m\r" 
	ar rcs $@ $^	
$(DRAW_LIB): $(DRAW_OBJECTS)
	@echo -n "\r\033[0;33m\r"
	ar rcs $@ $^	
$(EVNT_LIB): $(EVNT_OBJECTS)
	@echo -n "\r\033[0;33m\r"
	ar rcs $@ $^	
$(DVCS_LIB): $(DVCS_OBJECTS)
	@echo -n "\r\033[0;33m\r"
	ar rcs $@ $^	


.PHONY: tests
tests: $(TESTCOMP) $(TESTTERM) $(TESTCHAR) $(TESTKEYS)

# сборка тестов 1лабы.
.PHONY: $(TESTCOMP)
$(TESTCOMP): $(TESTCOMP_EXE)
$(TESTCOMP_EXE): $(TESTCOMP_OBJECTS)
	@echo "\033[1;31m----СБОРКА ТЕСТОВ mySimpleComputer----"
	$(GCC) $(CFLAGS) $(DEBUG) $(CPPFLAGS) -o $@ $^ -L$(OBJ)/$(SRC) -l$(COMP) 
# сборка тестов 2лабы.
.PHONY: $(TESTTERM)
$(TESTTERM): $(TESTTERM_EXE)
$(TESTTERM_EXE): $(TESTTERM_OBJECTS)
	@echo "\033[1;31m----СБОРКА ТЕСТОВ myTerm----"
	$(GCC) $(CFLAGS) $(DEBUG) $(CPPFLAGS) -o $@ $^ -L$(OBJ)/$(SRC) -l$(TERM) 
# сборка тестов 3лабы.
.PHONY: $(TESTCHAR)
$(TESTCHAR): $(TESTCHAR_EXE)
$(TESTCHAR_EXE): $(TESTCHAR_OBJECTS)
	@echo "\033[1;31m----СБОРКА ТЕСТОВ myBigChars----"
	$(GCC) $(CFLAGS) $(DEBUG) $(CPPFLAGS) -o $@ $^ -L$(OBJ)/$(SRC) -l$(CHAR) -l$(TERM)
# сборка тестов 4лабы.
.PHONY: $(TESTKEYS)
$(TESTKEYS): $(TESTKEYS_EXE)
$(TESTKEYS_EXE): $(TESTKEYS_OBJECTS)
	@echo "\033[1;31m----СБОРКА ТЕСТОВ myReadKey----"
	$(GCC) $(CFLAGS) $(DEBUG) $(CPPFLAGS) -o $@ $^ -L$(OBJ)/$(SRC) -l$(KEYS) 

.PHONY: clean
clean:
	rm -rf $(BIN) $(OBJ)
run:
	$(MAIN_EXE)
format:
	@echo "\033[1;32m----ФОРМАТИРОВАНИЕ ИСХОДНОГО КОДА----"
	find . -type f -name '*.[ch]' -not -path './thirdparty/*' | xargs clang-format --style GNU -i --verbose
