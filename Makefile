# Имена исполняемых файлов
TARGET = bin/pwgen
TEST_TARGET = bin/test_pwgen

# Компилятор и базовые флаги
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -Iinclude -Ithirdparty
TEST_CFLAGS = -Wall -Wextra -std=c11 -Iinclude -isystem thirdparty

# Директории проекта
SRC_DIR = src
TEST_DIR = tests
OBJ_DIR = obj
BIN_DIR = bin

# Списки исходных и объектных файлов для основного приложения
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))

# Списки файлов для тестов (исключаем main.c из исходников приложения)
TEST_FILES = $(wildcard $(TEST_DIR)/*.c)
TEST_OBJ_FILES = $(patsubst $(TEST_DIR)/%.c, $(OBJ_DIR)/%.o, $(TEST_FILES))
SRC_OBJ_FOR_TEST = $(filter-out $(OBJ_DIR)/main.o, $(OBJ_FILES))

# Главная цель по умолчанию
.PHONY: all clean test check-style coverage

all: $(BIN_DIR) $(OBJ_DIR) $(TARGET)

# Создание необходимых директорий
$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

# Линковка основного приложения
$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^

# Компиляция объектных файлов основного приложения
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

# Сборка и запуск unit-тестов
test: $(BIN_DIR) $(OBJ_DIR) $(TEST_TARGET)
	@echo "=== Запуск unit-тестов ==="
	./$(TEST_TARGET)

# Линковка исполняемого файла тестов (CFLAGS здесь обязателен для работы gcov)
$(TEST_TARGET): $(TEST_OBJ_FILES) $(SRC_OBJ_FOR_TEST)
	$(CC) $(CFLAGS) -o $@ $^

# Компиляция объектных файлов тестов
$(OBJ_DIR)/%.o: $(TEST_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(TEST_CFLAGS) -c -o $@ $<

# Проверка стиля кода через clang-format
check-style:
	@echo "=== Проверка стиля кода ==="
	clang-format -n $(SRC_DIR)/*.c include/*.h $(TEST_DIR)/*.c

# =========================================================================
# Сбор покрытия кода СТРОГО через gcov (без lcov/genhtml)
# =========================================================================
coverage:
	@echo "=== Полная очистка перед замером покрытия ==="
	$(MAKE) clean
	@echo "=== Сборка проекта с флагами gcov ==="
	$(MAKE) all CFLAGS="$(CFLAGS) --coverage"
	$(MAKE) test CFLAGS="$(CFLAGS) --coverage" TEST_CFLAGS="$(TEST_CFLAGS) --coverage"
	@echo "=== Вывод статистики покрытия через gcov ==="
	@echo "---------------------------------------------------------"
	@# Вызываем gcov, передавая путь к объектным файлам
	@gcov -b --object-directory=$(OBJ_DIR) $(SRC_DIR)/args.c
	@gcov -b --object-directory=$(OBJ_DIR) $(SRC_DIR)/charset.c
	@gcov -b --object-directory=$(OBJ_DIR) $(SRC_DIR)/generator.c
	@gcov -b --object-directory=$(OBJ_DIR) $(SRC_DIR)/output.c
	@gcov -b --object-directory=$(OBJ_DIR) $(SRC_DIR)/random.c
	@echo "---------------------------------------------------------"
	@echo "Текстовые отчеты *.gcov сохранены в корне проекта."

# Полная очистка проекта
clean:
	rm -rf $(OBJ_DIR)/*.o $(TARGET) $(TEST_PWGEN)
	rm -f *.gcov *.gcda *.gcno