#include "ctest.h"
#include "generator.h"
#include "random.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

static const char SPECIALS[] = "!@#$%^&*()_+-=[]{};:'\",.<>/?\\|`~";
static int is_special_char(char c)
{
    return strchr(SPECIALS, c) != NULL;
}

CTEST(generator, all_classes_standard)
{
    srand(12345); // фиксируем seed для повторяемости
    char buf[128];
    PwgenOptions opts = {0};
    opts.capitalize = true;
    opts.numerals = true;
    opts.symbols = true;

    int ret = generate_password(buf, 12, &opts);
    ASSERT_EQUAL(0, ret);
    ASSERT_EQUAL(12, (int)strlen(buf));

    int has_upper = 0, has_digit = 0, has_special = 0;
    for (int i = 0; i < 12; i++) {
        if (isupper((unsigned char)buf[i]))
            has_upper = 1;
        else if (isdigit((unsigned char)buf[i]))
            has_digit = 1;
        else if (is_special_char(buf[i]))
            has_special = 1;
    }
    ASSERT_TRUE(has_upper);
    ASSERT_TRUE(has_digit);
    ASSERT_TRUE(has_special);
}

CTEST(generator, secure_mode)
{
    // Инициализируем /dev/urandom
    ASSERT_EQUAL(0, random_init());

    char buf[128];
    PwgenOptions opts = {0};
    opts.capitalize = true;
    opts.numerals = true;
    opts.symbols = true;
    opts.secure = true;

    int ret = generate_password(buf, 10, &opts);
    ASSERT_EQUAL(0, ret);
    ASSERT_EQUAL(10, (int)strlen(buf));

    int has_upper = 0, has_digit = 0, has_special = 0;
    for (int i = 0; i < 10; i++) {
        if (isupper((unsigned char)buf[i]))
            has_upper = 1;
        else if (isdigit((unsigned char)buf[i]))
            has_digit = 1;
        else if (is_special_char(buf[i]))
            has_special = 1;
    }
    ASSERT_TRUE(has_upper);
    ASSERT_TRUE(has_digit);
    ASSERT_TRUE(has_special);

    random_cleanup();
}

CTEST(generator, invalid_inputs)
{
    PwgenOptions opts;
    set_default_options(&opts);

    // Проверяем реакцию функции на некорректные параметры (должна возвращать
    // ошибку, например -1)
    ASSERT_EQUAL(2, generate_password(NULL, 10, &opts));

    char buf[10];
    ASSERT_EQUAL(2, generate_password(buf, 0, &opts));
    ASSERT_EQUAL(2, generate_password(buf, -5, &opts));
}

CTEST(generator, only_lowercase)
{
    srand(12345);
    char buf[128];
    PwgenOptions opts = {0};

    int ret = generate_password(buf, 10, &opts);
    ASSERT_EQUAL(0, ret);
    for (int i = 0; i < 10; i++) {
        ASSERT_TRUE(buf[i] >= 'a' && buf[i] <= 'z');
    }
}

CTEST(generator, invalid_arguments)
{
    PwgenOptions opts = {0};
    char buf[10];

    int ret = generate_password(NULL, 8, &opts);
    ASSERT_EQUAL(2, ret);

    ret = generate_password(buf, 0, &opts);
    ASSERT_EQUAL(2, ret);

    ret = generate_password(buf, 8, NULL);
    ASSERT_EQUAL(2, ret);
}
