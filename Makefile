MAIN = main
SAT = sat
SBT = sbt
TERM = myTerm
COMP = mySimpleComputer
CHAR = myBigChars
KEYS = myReadKey
DRAW = draw
EVNT = events
DVCS = devices
SATR = SAtranslator
SBTR = SBtranslator
TREE = bstree
STCK = stack

TEST = test
TESTCOMP = testmySimpleComputer
TESTTERM = testmyTerm
TESTCHAR = testmyBigChars
TESTKEYS = testmyReadKey
DEBUG = -g3 -O0
MD = mkdir
LIBS = -l$(COMP) -l$(TERM) -l$(CHAR) -l$(KEYS) -l$(DRAW) -l$(EVNT) -l$(DVCS) -l$(SATR) -l$(SBTR) -l$(TREE) -l$(STCK)

#CFLAGS =  -Wall -Wextra -Werror
CPPFLAGS = -I src -MP -MMD
GCC = gcc
BIN = programs
LIB = libraries
OBJ = objects
SRC = src
C = c

# main
MAIN_EXE = $(MAIN).exe
# статические либы.
TERM_LIB = $(LIB)/lib$(TERM).a
COMP_LIB = $(LIB)/lib$(COMP).a
CHAR_LIB = $(LIB)/lib$(CHAR).a
KEYS_LIB = $(LIB)/lib$(KEYS).a
DRAW_LIB = $(LIB)/lib$(DRAW).a
EVNT_LIB = $(LIB)/lib$(EVNT).a
DVCS_LIB = $(LIB)/lib$(DVCS).a
SATR_LIB = $(LIB)/lib$(SATR).a
SBTR_LIB = $(LIB)/lib$(SBTR).a
TREE_LIB = $(LIB)/lib$(TREE).a
STCK_LIB = $(LIB)/lib$(STCK).a
# ретрансляторы
SATR_EXE = $(BIN)/$(SAT).exe
SBTR_EXE = $(BIN)/$(SBT).exe
# тесты.
TESTCOMP_EXE = $(BIN)/$(TESTCOMP).exe
TESTTERM_EXE = $(BIN)/$(TESTTERM).exe
TESTCHAR_EXE = $(BIN)/$(TESTCHAR).exe
TESTKEYS_EXE = $(BIN)/$(TESTKEYS).exe

MAIN_SOURCES = $(shell find $(SRC)/Computer/$(MAIN) -name '*.$(C)')
MAIN_OBJECTS = $(MAIN_SOURCES:$(SRC)/%.$(C)=$(OBJ)/$(SRC)/%.o)

SAT_SOURCES = $(shell find $(SRC)/Retranslators/$(SAT) -name '*.$(C)')
SAT_OBJECTS = $(SAT_SOURCES:$(SRC)/%.$(C)=$(OBJ)/$(SRC)/%.o)

SBT_SOURCES = $(shell find $(SRC)/Retranslators/$(SBT) -name '*.$(C)')
SBT_OBJECTS = $(SBT_SOURCES:$(SRC)/%.$(C)=$(OBJ)/$(SRC)/%.o)

TERM_SOURCES = $(shell find $(SRC)/Computer/$(TERM) -name '*.$(C)')
TERM_OBJECTS = $(TERM_SOURCES:$(SRC)/%.$(C)=$(OBJ)/$(SRC)/%.o)

COMP_SOURCES = $(shell find $(SRC)/Computer/$(COMP) -name '*.$(C)')
COMP_OBJECTS = $(COMP_SOURCES:$(SRC)/%.$(C)=$(OBJ)/$(SRC)/%.o)

CHAR_SOURCES = $(shell find $(SRC)/Computer/$(CHAR) -name '*.$(C)')
CHAR_OBJECTS = $(CHAR_SOURCES:$(SRC)/%.$(C)=$(OBJ)/$(SRC)/%.o)

KEYS_SOURCES = $(shell find $(SRC)/Computer/$(KEYS) -name '*.$(C)')
KEYS_OBJECTS = $(KEYS_SOURCES:$(SRC)/%.$(C)=$(OBJ)/$(SRC)/%.o)

DRAW_SOURCES = $(shell find $(SRC)/Console/$(DRAW) -name '*.$(C)')
DRAW_OBJECTS = $(DRAW_SOURCES:$(SRC)/%.$(C)=$(OBJ)/$(SRC)/%.o)

EVNT_SOURCES = $(shell find $(SRC)/Console/$(EVNT) -name '*.$(C)')
EVNT_OBJECTS = $(EVNT_SOURCES:$(SRC)/%.$(C)=$(OBJ)/$(SRC)/%.o)

DVCS_SOURCES = $(shell find $(SRC)/Computer/$(DVCS) -name '*.$(C)')
DVCS_OBJECTS = $(DVCS_SOURCES:$(SRC)/%.$(C)=$(OBJ)/$(SRC)/%.o)

TREE_SOURCES = $(shell find $(SRC)/Retranslators/$(TREE) -name '*.$(C)')
TREE_OBJECTS = $(TREE_SOURCES:$(SRC)/%.$(C)=$(OBJ)/$(SRC)/%.o)

STCK_SOURCES = $(shell find $(SRC)/Retranslators/$(STCK) -name '*.$(C)')
STCK_OBJECTS = $(STCK_SOURCES:$(SRC)/%.$(C)=$(OBJ)/$(SRC)/%.o)

SATR_SOURCES = $(shell find $(SRC)/Retranslators/$(SATR) -name '*.$(C)')
SATR_OBJECTS = $(SATR_SOURCES:$(SRC)/%.$(C)=$(OBJ)/$(SRC)/%.o)

SBTR_SOURCES = $(shell find $(SRC)/Retranslators/$(SBTR) -name '*.$(C)')
SBTR_OBJECTS = $(SBTR_SOURCES:$(SRC)/%.$(C)=$(OBJ)/$(SRC)/%.o)

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
$(MAIN): $(MD) $(MAIN_EXE) tests $(SAT) $(SBT)

# make all - форматирование, компиляция и запуск в одной команде!
.PHONY: all
all: format $(MAIN) run

# добавление папок для бинарников (bin & obj)
.PHONY: $(MD)
mkdir:
	@echo "\033[1;35m----СОЗДАНИЕ ПАПОК----"
	mkdir -p $(BIN)
	mkdir -p $(LIB)
	mkdir -p $(OBJ)/$(TEST)
	mkdir -p $(OBJ)/$(SRC)/Computer/$(TERM)
	mkdir -p $(OBJ)/$(SRC)/Computer/$(CHAR)
	mkdir -p $(OBJ)/$(SRC)/Computer/$(COMP)
	mkdir -p $(OBJ)/$(SRC)/Computer/$(KEYS)
	mkdir -p $(OBJ)/$(SRC)/Computer/$(MAIN)
	mkdir -p $(OBJ)/$(SRC)/Console/$(DRAW)
	mkdir -p $(OBJ)/$(SRC)/Console/$(EVNT)
	mkdir -p $(OBJ)/$(SRC)/Computer/$(DVCS)
	mkdir -p $(OBJ)/$(SRC)/Retranslators/$(TREE)
	mkdir -p $(OBJ)/$(SRC)/Retranslators/$(STCK)
	mkdir -p $(OBJ)/$(SRC)/Retranslators/$(SATR)
	mkdir -p $(OBJ)/$(SRC)/Retranslators/$(SBTR)
	mkdir -p $(OBJ)/$(SRC)/Retranslators/$(SAT)
	mkdir -p $(OBJ)/$(SRC)/Retranslators/$(SBT)
	
# сборка программы со всеми библиотеками.
$(MAIN_EXE): $(MAIN_OBJECTS) $(COMP_LIB) $(TERM_LIB) $(CHAR_LIB) $(KEYS_LIB) $(DRAW_LIB) $(EVNT_LIB) $(DVCS_LIB) $(SATR_LIB) $(SBTR_LIB) $(TREE_LIB) $(STCK_LIB)
	@echo "\033[1;31m----СБОРКА КОМПЬЮТЕРА----"
	$(GCC) $(CFLAGS) $(DEBUG) $(CPPFLAGS) -o $@ $< $(LIB)/libdraw.a -L$(LIB) $(LIBS)

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
$(SATR_LIB): $(SATR_OBJECTS)
	@echo -n "\r\033[0;33m\r"
	ar rcs $@ $^	
$(SBTR_LIB): $(SBTR_OBJECTS)
	@echo -n "\r\033[0;33m\r"
	ar rcs $@ $^	
$(TREE_LIB): $(TREE_OBJECTS)
	@echo -n "\r\033[0;33m\r"
	ar rcs $@ $^	
$(STCK_LIB): $(STCK_OBJECTS)
	@echo -n "\r\033[0;33m\r"
	ar rcs $@ $^	
	
# сборка ретрансляторов.
.PHONY: $(SAT)
$(SAT): $(SATR_EXE)
$(SATR_EXE): $(SAT_OBJECTS)
	@echo "\033[1;31m----СБОРКА SA РЕТРАНСЛЯТОРА----"
	$(GCC) $(CFLAGS) $(DEBUG) $(CPPFLAGS) -o $@ $^ $(LIB)/libdraw.a -L$(LIB) -l$(SATR) -l$(TREE) -l$(COMP) -l$(DRAW) -l$(TERM) -l$(CHAR)

.PHONY: $(SBT)
$(SBT): $(SBTR_EXE)
$(SBTR_EXE): $(SBT_OBJECTS)
	@echo "\033[1;31m----СБОРКА SB РЕТРАНСЛЯТОРА----"
	$(GCC) $(CFLAGS) $(DEBUG) $(CPPFLAGS) -o $@ $^ $(LIB)/libdraw.a -L$(LIB) -l$(SATR) -l$(SBTR) -l$(TREE) -l$(STCK) -l$(DRAW) -l$(TERM) -l$(CHAR) -l$(COMP)

.PHONY: tests
tests: $(TESTCOMP) $(TESTTERM) $(TESTCHAR) $(TESTKEYS)
# сборка тестов 1лабы.
.PHONY: $(TESTCOMP)
$(TESTCOMP): $(TESTCOMP_EXE)
$(TESTCOMP_EXE): $(TESTCOMP_OBJECTS)
	@echo "\033[1;31m----СБОРКА ТЕСТОВ mySimpleComputer----"
	$(GCC) $(CFLAGS) $(DEBUG) $(CPPFLAGS) -o $@ $^ -L$(LIB) -l$(COMP) 
# сборка тестов 2лабы.
.PHONY: $(TESTTERM)
$(TESTTERM): $(TESTTERM_EXE)
$(TESTTERM_EXE): $(TESTTERM_OBJECTS)
	@echo "\033[1;31m----СБОРКА ТЕСТОВ myTerm----"
	$(GCC) $(CFLAGS) $(DEBUG) $(CPPFLAGS) -o $@ $^ -L$(LIB) -l$(TERM) 
# сборка тестов 3лабы.
.PHONY: $(TESTCHAR)
$(TESTCHAR): $(TESTCHAR_EXE)
$(TESTCHAR_EXE): $(TESTCHAR_OBJECTS)
	@echo "\033[1;31m----СБОРКА ТЕСТОВ myBigChars----"
	$(GCC) $(CFLAGS) $(DEBUG) $(CPPFLAGS) -o $@ $^ -L$(LIB) -l$(CHAR) -l$(TERM)
# сборка тестов 4лабы.
.PHONY: $(TESTKEYS)
$(TESTKEYS): $(TESTKEYS_EXE)
$(TESTKEYS_EXE): $(TESTKEYS_OBJECTS)
	@echo "\033[1;31m----СБОРКА ТЕСТОВ myReadKey----"
	$(GCC) $(CFLAGS) $(DEBUG) $(CPPFLAGS) -o $@ $^ -L$(LIB) -l$(KEYS) 

.PHONY: clean
clean:
	rm -rf $(BIN) $(OBJ) $(LIB) main.exe
run:
	./$(MAIN_EXE)
format:
	@echo "\033[1;32m----ФОРМАТИРОВАНИЕ ИСХОДНОГО КОДА----"
	find . -type f -name '*.[ch]' -not -path './thirdparty/*' | xargs clang-format --style google -i --verbose

# sc_files/binary/b.o