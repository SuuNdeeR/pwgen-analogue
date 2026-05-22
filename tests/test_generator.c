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

CTEST(generator, trigger_ensure_requirements_standard)
{
    /* Первый «холостой» вызов, чтобы rand_initialized стал 1
       и generate_password больше не трогал srand(time(NULL)) */
    char buf[128];
    PwgenOptions opts = {0};
    generate_password(buf, 8, &opts);

    opts.capitalize = true;
    opts.numerals = true;
    opts.symbols = true;

    /* Длина 4 и три класса — вероятность, что случайно выпадут
       все классы сразу, мала. За 200 итераций с разными seed'ами
       ensure_requirements и random_class_char гарантированно
       отработают по всем веткам (upper, digit, special). */
    for (int seed = 1; seed <= 200; seed++) {
        srand((unsigned int)seed);
        generate_password(buf, 4, &opts);
        ASSERT_EQUAL(4, (int)strlen(buf));
    }
}

CTEST(generator, trigger_ensure_requirements_secure)
{
    ASSERT_EQUAL(0, random_init());

    char buf[128];
    PwgenOptions opts = {0};
    opts.capitalize = true;
    opts.numerals = true;
    opts.symbols = true;
    opts.secure = true;

    for (int i = 0; i < 50; i++) {
        generate_password(buf, 4, &opts);
        ASSERT_EQUAL(4, (int)strlen(buf));
    }

    random_cleanup();
}

CTEST(generator, no_ambiguous_in_ensure_requirements)
{
    char buf[128];
    PwgenOptions opts = {0};
    generate_password(buf, 8, &opts); /* init rand_initialized */

    opts.capitalize = true;
    opts.numerals = true;
    opts.symbols = true;
    opts.no_ambiguous = true;

    for (int seed = 1; seed <= 200; seed++) {
        srand((unsigned int)seed);
        generate_password(buf, 4, &opts);
        ASSERT_EQUAL(4, (int)strlen(buf));
    }
}
