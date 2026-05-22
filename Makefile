CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -Iinclude -Ithirdparty
# Тесты без -Werror, т.к. ctest.h несовместим с GCC 13
TEST_CFLAGS = -Wall -Wextra -std=c11 -Iinclude -isystem -Ithirdparty

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
TEST_DIR = tests

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))
TARGET = $(BIN_DIR)/pwgen

TEST_SRC = $(wildcard $(TEST_DIR)/test_*.c)
TEST_OBJ = $(patsubst $(TEST_DIR)/%.c,$(OBJ_DIR)/%.o,$(TEST_SRC))
TEST_PWGEN = $(BIN_DIR)/test_pwgen

all: dirs $(TARGET)

dirs:
	@mkdir -p $(OBJ_DIR) $(BIN_DIR)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Основной код — со всеми проверками
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Тесты — без -Werror (ctest.h несовместим с GCC 13)
$(OBJ_DIR)/%.o: $(TEST_DIR)/%.c
	$(CC) $(TEST_CFLAGS) -c -o $@ $<

test: dirs $(TEST_PWGEN)
	./$(TEST_PWGEN)

$(TEST_PWGEN): $(TEST_OBJ) $(filter-out $(OBJ_DIR)/main.o,$(OBJ))
	$(CC) $(TEST_CFLAGS) -o $@ $^

check-style:
	clang-format --dry-run --Werror $(SRC_DIR)/*.c include/*.h $(TEST_DIR)/*.c

format:
	clang-format -i $(SRC_DIR)/*.c include/*.h $(TEST_DIR)/*.c

clean:
	rm -rf $(OBJ_DIR)/*.o $(TARGET) $(TEST_PWGEN)

.PHONY: all test check-style format clean dirs