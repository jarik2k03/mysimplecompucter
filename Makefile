APP_NAME = main
LIB_NAME = myTerm
COMP_NAME = mySimpleComputer
TEST_NAME = test
TESTCOMP_NAME = testmySimpleComputer
TESTTERM_NAME = testmyTerm

DEBUG = -g3 -O0

CFLAGS = -Wall -Wextra -Werror -Wcomments -Wdeprecated -Wformat-extra-args -Wno-pragmas -Wstrict-overflow=5 -Wpedantic
CPPFLAGS = -I src -MP -MMD
CC = gcc
BIN_DIR = bin
OBJ_DIR = obj
SRC_DIR = src

APP_PATH = $(BIN_DIR)/$(APP_NAME)
LIB_PATH = $(OBJ_DIR)/$(SRC_DIR)/$(LIB_NAME)/lib$(LIB_NAME).a
COMP_PATH = $(OBJ_DIR)/$(SRC_DIR)/$(COMP_NAME)/lib$(COMP_NAME).a
TESTCOMP_PATH = $(BIN_DIR)/$(TESTCOMP_NAME)
TESTTERM_PATH = $(BIN_DIR)/$(TESTTERM_NAME)

SRC_EXT = c

APP_SOURCES = $(shell find $(SRC_DIR)/$(APP_NAME) -name '*.$(SRC_EXT)')
APP_OBJECTS = $(APP_SOURCES:$(SRC_DIR)/%.$(SRC_EXT)=$(OBJ_DIR)/$(SRC_DIR)/%.o)

LIB_SOURCES = $(shell find $(SRC_DIR)/$(LIB_NAME) -name '*.$(SRC_EXT)')
LIB_OBJECTS = $(LIB_SOURCES:$(SRC_DIR)/%.$(SRC_EXT)=$(OBJ_DIR)/$(SRC_DIR)/%.o)

COMP_SOURCES = $(shell find $(SRC_DIR)/$(COMP_NAME) -name '*.$(SRC_EXT)')
COMP_OBJECTS = $(COMP_SOURCES:$(SRC_DIR)/%.$(SRC_EXT)=$(OBJ_DIR)/$(SRC_DIR)/%.o)

TESTCOMP_SOURCES = $(TEST_NAME)/main.c $(TEST_NAME)/$(COMP_NAME).c
TESTCOMP_OBJECTS = $(TESTCOMP_SOURCES:$(TEST_NAME)/%.c=$(OBJ_DIR)/$(TEST_NAME)/%.o)

TESTTERM_SOURCES = $(TEST_NAME)/main.c $(TEST_NAME)/$(LIB_NAME).c
TESTTERM_OBJECTS = $(TESTTERM_SOURCES:$(TEST_NAME)/%.c=$(OBJ_DIR)/$(TEST_NAME)/%.o)


DEPS = $(APP_OBJECTS:.o=.d) $(LIB_OBJECTS:.o=.d) $(COMP_OBJECTS:.o=.d)



.PHONY: $(APP_NAME)
$(APP_NAME): mkdir $(APP_PATH)

-include $(DEPS)

.PHONY: mkdir
mkdir:
	[ -d $(BIN_DIR) ] || mkdir $(BIN_DIR)
	[ -d $(OBJ_DIR) ] || mkdir $(OBJ_DIR)
	[ -d $(OBJ_DIR)/$(SRC_DIR) ] || mkdir $(OBJ_DIR)/$(SRC_DIR)
	[ -d $(OBJ_DIR)/$(SRC_DIR)/$(LIB_NAME) ] || mkdir $(OBJ_DIR)/$(SRC_DIR)/$(LIB_NAME)
	[ -d $(OBJ_DIR)/$(SRC_DIR)/$(COMP_NAME) ] || mkdir $(OBJ_DIR)/$(SRC_DIR)/$(COMP_NAME)
	[ -d $(OBJ_DIR)/$(SRC_DIR)/$(APP_NAME) ] || mkdir $(OBJ_DIR)/$(SRC_DIR)/$(APP_NAME)
	[ -d $(OBJ_DIR)/$(TEST_NAME) ] || mkdir $(OBJ_DIR)/$(TEST_NAME)

$(APP_PATH): $(APP_OBJECTS) $(LIB_PATH) $(COMP_PATH)
	
	$(CC) $(CFLAGS) $(DEBUG) $(CPPFLAGS) -o $@ $^ -l$(LIB_NAME) -Lobj/src/$(LIB_NAME)

$(LIB_PATH): $(LIB_OBJECTS)
	ar rcs $@ $^
	
$(COMP_PATH): $(COMP_OBJECTS)
	ar rcs $@ $^

$(OBJ_DIR)/%.o: %.c
	$(CC) $(CFLAGS) $(DEBUG) $(CPPFLAGS) -c -o $@ $<



.PHONY: $(TESTCOMP_NAME)
$(TESTCOMP_NAME): mkdir $(TESTCOMP_PATH)

-include $(DEPS)

$(TESTCOMP_PATH): $(TESTCOMP_OBJECTS) $(COMP_PATH)
	$(CC) $(CFLAGS) $(DEBUG) $(CPPFLAGS) -o $@ $^ -lm


.PHONY: $(TESTTERM_NAME)
$(TESTTERM_NAME): mkdir $(TESTTERM_PATH)

$(TESTTERM_PATH): $(TESTTERM_OBJECTS) $(LIB_PATH)
	$(CC) $(CFLAGS) $(DEBUG) $(CPPFLAGS) -o $@ $^ -lm


$(OBJ_DIR)/%.o: %.c
	$(CC) $(CFLAGS) $(DEBUG) $(CPPFLAGS) -c -o $@ $<





.PHONY: clean
clean:
	rm -rf bin obj

run:
	-$(APP_PATH)
test_run:
	-$(TEST_PATH) 
