MAIN = main
TERM = myTerm
COMP = mySimpleComputer
CHAR = myBigChars
KEYS = myReadKey
DRAW = draw

TEST = test
TESTCOMP = testmySimpleComputer
TESTTERM = testmyTerm
TESTCHAR = testmyBigChars
TESTKEYS = testmyReadKey
DEBUG = -g3 -O0
MD = mkdir

CFLAGS =  -Werror -Wcomments -Wdeprecated -Wno-pragmas -Wstrict-overflow=5 -Wchar-subscripts -Wmultistatement-macros -Wparentheses -Warray-bounds=2 -fdiagnostics-show-option
CPPFLAGS = -I src -MP -MMD
GCC = gcc
BIN = bin
OBJ = obj
SRC = src
C = c

# main
MAIN_EXE = $(BIN)/$(MAIN).exe
# статические либы.
TERM_LIB = $(OBJ)/$(SRC)/$(TERM)/lib$(TERM).a
COMP_LIB = $(OBJ)/$(SRC)/$(COMP)/lib$(COMP).a
CHAR_LIB = $(OBJ)/$(SRC)/$(CHAR)/lib$(CHAR).a
KEYS_LIB = $(OBJ)/$(SRC)/$(KEYS)/lib$(KEYS).a
DRAW_LIB = $(OBJ)/$(SRC)/$(DRAW)/lib$(DRAW).a
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
	$(GCC) $(CFLAGS) $(DEBUG) $(CPPFLAGS) -c -o $@ $<

# make (Первая зависимость - выполняет всё.)
.PHONY: $(MAIN) 
$(MAIN): $(MD) $(MAIN_EXE) $(TESTCOMP) $(TESTTERM) $(TESTCHAR) $(TESTKEYS)

# make all - компиляция и запуск в одной команде!
.PHONY: all
all: $(MAIN) run


# добавление папок для бинарников (bin & obj)
.PHONY: $(MD)
mkdir:
	[ -d $(BIN) ] || mkdir $(BIN)
	[ -d $(OBJ) ] || mkdir $(OBJ)
	[ -d $(OBJ)/$(SRC) ] || mkdir $(OBJ)/$(SRC)
	[ -d $(OBJ)/$(SRC)/$(TERM) ] || mkdir $(OBJ)/$(SRC)/$(TERM)
	[ -d $(OBJ)/$(SRC)/$(CHAR) ] || mkdir $(OBJ)/$(SRC)/$(CHAR)
	[ -d $(OBJ)/$(SRC)/$(COMP) ] || mkdir $(OBJ)/$(SRC)/$(COMP)
	[ -d $(OBJ)/$(SRC)/$(KEYS) ] || mkdir $(OBJ)/$(SRC)/$(KEYS)
	[ -d $(OBJ)/$(SRC)/$(MAIN) ] || mkdir $(OBJ)/$(SRC)/$(MAIN)
	[ -d $(OBJ)/$(SRC)/$(DRAW) ] || mkdir $(OBJ)/$(SRC)/$(DRAW)
	[ -d $(OBJ)/$(TEST) ] || mkdir $(OBJ)/$(TEST)

# сборка программы со всеми библиотеками.
$(MAIN_EXE): $(MAIN_OBJECTS) $(COMP_LIB) $(CHAR_LIB) $(KEYS_LIB) $(DRAW_LIB) $(TERM_LIB)
	$(GCC) $(CFLAGS) $(DEBUG) $(CPPFLAGS) -o $@ $^ -l$(COMP) -L$(OBJ)/$(SRC)/$(COMP) -l$(TERM) -L$(OBJ)/$(SRC)/$(TERM) -l$(CHAR) -L$(OBJ)/$(SRC)/$(CHAR) -l$(KEYS) -L$(OBJ)/$(SRC)/$(KEYS) -l$(DRAW) -L$(OBJ)/$(SRC)/$(DRAW)

# создание статических библиотек.
$(COMP_LIB): $(COMP_OBJECTS)
	ar rcs $@ $^
$(TERM_LIB): $(TERM_OBJECTS)
	ar rcs $@ $^
$(CHAR_LIB): $(CHAR_OBJECTS)
	ar rcs $@ $^	
$(KEYS_LIB): $(KEYS_OBJECTS)
	ar rcs $@ $^	
$(DRAW_LIB): $(DRAW_OBJECTS)
	ar rcs $@ $^	

# сборка тестов 1лабы.
.PHONY: $(TESTCOMP)
$(TESTCOMP): $(TESTCOMP_EXE)
$(TESTCOMP_EXE): $(TESTCOMP_OBJECTS) $(COMP_LIB)
	$(GCC) $(CFLAGS) $(DEBUG) $(CPPFLAGS) -o $@ $^
# сборка тестов 2лабы.
.PHONY: $(TESTTERM)
$(TESTTERM): $(TESTTERM_EXE)
$(TESTTERM_EXE): $(TESTTERM_OBJECTS) $(TERM_LIB)
	$(GCC) $(CFLAGS) $(DEBUG) $(CPPFLAGS) -o $@ $^
# сборка тестов 3лабы.
.PHONY: $(TESTCHAR)
$(TESTCHAR): $(TESTCHAR_EXE)
$(TESTCHAR_EXE): $(TESTCHAR_OBJECTS) $(CHAR_LIB) $(TERM_LIB)
	$(GCC) $(CFLAGS) $(DEBUG) $(CPPFLAGS) -o $@ $^
# сборка тестов 4лабы.
.PHONY: $(TESTKEYS)
$(TESTKEYS): $(TESTKEYS_EXE)
$(TESTKEYS_EXE): $(TESTKEYS_OBJECTS) $(KEYS_LIB)
	$(GCC) $(CFLAGS) $(DEBUG) $(CPPFLAGS) -o $@ $^



.PHONY: clean
clean:
	rm -rf $(BIN) $(OBJ)
run:
	$(MAIN_EXE)