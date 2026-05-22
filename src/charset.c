#include "charset.h"
#include <string.h>

static const char AMBIGUOUS[] = "l1O0I";

static int is_ambiguous(char c)
{
    for (int i = 0; AMBIGUOUS[i]; i++)
        if (c == AMBIGUOUS[i])
            return 1;
    return 0;
}

int charset_build(const PwgenOptions* opts, char* out, size_t out_size)
{
    if (!opts || !out || out_size == 0)
        return 0;
    size_t len = 0;

    // Всегда строчные латинские буквы (база)
    for (char c = 'a'; c <= 'z'; c++) {
        if (opts->no_ambiguous && is_ambiguous(c))
            continue;
        if (len + 1 >= out_size)
            return 0;
        out[len++] = c;
    }

    // Заглавные – только если capitalize и не запрещено no_capitalize
    if (opts->capitalize && !opts->no_capitalize) {
        for (char c = 'A'; c <= 'Z'; c++) {
            if (opts->no_ambiguous && is_ambiguous(c))
                continue;
            if (len + 1 >= out_size)
                return 0;
            out[len++] = c;
        }
    }

    // Цифры – только если numerals и не запрещено no_numerals
    if (opts->numerals && !opts->no_numerals) {
        for (char c = '0'; c <= '9'; c++) {
            if (opts->no_ambiguous && is_ambiguous(c))
                continue;
            if (len + 1 >= out_size)
                return 0;
            out[len++] = c;
        }
    }

    // Специальные символы
    if (opts->symbols) {
        for (int i = 0; SPECIAL_CHARS[i]; i++) {
            char c = SPECIAL_CHARS[i];
            if (opts->no_ambiguous && is_ambiguous(c))
                continue;
            if (len + 1 >= out_size)
                return 0;
            out[len++] = c;
        }
    }

    if (len == 0)
        return 0;
    out[len] = '\0';
    return (int)len;
}

int charset_is_special_char(char c)
{
    for (int i = 0; SPECIAL_CHARS[i]; i++)
        if (c == SPECIAL_CHARS[i])
            return 1;
    return 0;
}
