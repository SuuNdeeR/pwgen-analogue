#include "args.h"
#include "charset.h"
#include "ctest.h"
#include <string.h>

CTEST(charset, default_alphabet)
{
    char buf[256];
    PwgenOptions opts = {0};
    int len = charset_build(&opts, buf, sizeof(buf));
    ASSERT_EQUAL(26, len);
    ASSERT_STR("abcdefghijklmnopqrstuvwxyz", buf);
}

CTEST(charset, with_capitalize)
{
    char buf[256];
    PwgenOptions opts = {0};
    opts.capitalize = true;
    int len = charset_build(&opts, buf, sizeof(buf));
    ASSERT_EQUAL(52, len);
    // проверяем наличие заглавных
    ASSERT_TRUE(strchr(buf, 'A') != NULL);
}

CTEST(charset, with_numerals)
{
    char buf[256];
    PwgenOptions opts = {0};
    opts.numerals = true;
    int len = charset_build(&opts, buf, sizeof(buf));
    ASSERT_EQUAL(36, len);
    ASSERT_TRUE(strchr(buf, '0') != NULL);
}

CTEST(charset, with_symbols)
{
    char buf[256];
    PwgenOptions opts = {0};
    opts.symbols = true;
    int len = charset_build(&opts, buf, sizeof(buf));
    ASSERT_TRUE(len > 26);
    ASSERT_TRUE(strchr(buf, '!') != NULL);
}

CTEST(charset, no_ambiguous_only_lowercase)
{
    char buf[256];
    PwgenOptions opts = {0};
    opts.no_ambiguous = true;
    int len = charset_build(&opts, buf, sizeof(buf));
    ASSERT_EQUAL(25, len);
    ASSERT_TRUE(strchr(buf, 'l') == NULL);
}

CTEST(charset, no_ambiguous_all_classes)
{
    char buf[256];
    PwgenOptions opts = {0};
    opts.capitalize = true;
    opts.numerals = true;
    opts.symbols = true;
    opts.no_ambiguous = true;
    int len = charset_build(&opts, buf, sizeof(buf));
    ASSERT_TRUE(len > 0);
    const char* ambiguous = "l1O0I";
    for (const char* p = ambiguous; *p; ++p) {
        ASSERT_TRUE(strchr(buf, *p) == NULL);
    }
}

CTEST(charset, conflict_capitalize_and_no_capitalize)
{
    char buf[256];
    PwgenOptions opts = {0};
    opts.capitalize = true;
    opts.no_capitalize = true; // no_capitalize имеет приоритет (так должно быть
                               // после parse_args)
    int len = charset_build(&opts, buf, sizeof(buf));
    ASSERT_EQUAL(26, len); // только строчные
    ASSERT_TRUE(strchr(buf, 'A') == NULL);
}

CTEST(charset, conflict_numerals_and_no_numerals)
{
    char buf[256];
    PwgenOptions opts = {0};
    opts.numerals = true;
    opts.no_numerals = true;
    int len = charset_build(&opts, buf, sizeof(buf));
    ASSERT_EQUAL(26, len); // только строчные
    ASSERT_TRUE(strchr(buf, '0') == NULL);
}
