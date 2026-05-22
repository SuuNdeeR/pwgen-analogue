#include "generator.h"
#include "charset.h"
#include "random.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

static int is_upper(char c)
{
    return c >= 'A' && c <= 'Z';
}
static int is_digit(char c)
{
    return c >= '0' && c <= '9';
}

static int class_present(const char* pwd, int len, int (*check)(char))
{
    for (int i = 0; i < len; i++)
        if (check(pwd[i]))
            return 1;
    return 0;
}

static char
random_class_char(int upper, int digit, int special, const PwgenOptions* opts)
{
    char pool[128];
    int n = 0;
    if (upper) {
        for (char c = 'A'; c <= 'Z'; c++) {
            if (opts->no_ambiguous && (c == 'I' || c == 'O'))
                continue;
            pool[n++] = c;
        }
    } else if (digit) {
        for (char c = '0'; c <= '9'; c++) {
            if (opts->no_ambiguous && (c == '0' || c == '1'))
                continue;
            pool[n++] = c;
        }
    } else if (special) {
        for (int i = 0; SPECIAL_CHARS[i]; i++) {
            char c = SPECIAL_CHARS[i];
            if (opts->no_ambiguous
                && (c == 'l' || c == '1' || c == 'O' || c == '0' || c == 'I'))
                continue;
            pool[n++] = c;
        }
    }
    if (n == 0)
        return 'a'; // fallback (не должно происходить)
    int idx = opts->secure ? random_index(n) : (rand() % n);
    return pool[idx];
}

static void ensure_requirements(char* pwd, int len, const PwgenOptions* opts)
{
    const int MAX_ITER = 10;
    for (int a = 0; a < MAX_ITER; a++) {
        int need_upper = opts->capitalize && !class_present(pwd, len, is_upper);
        int need_digit = opts->numerals && !class_present(pwd, len, is_digit);
        int need_special = opts->symbols
                && !class_present(pwd, len, charset_is_special_char);
        if (!need_upper && !need_digit && !need_special)
            break;
        int pos = rand() % len;
        if (need_upper)
            pwd[pos] = random_class_char(1, 0, 0, opts);
        else if (need_digit)
            pwd[pos] = random_class_char(0, 1, 0, opts);
        else if (need_special)
            pwd[pos] = random_class_char(0, 0, 1, opts);
    }
}

int generate_password(char* buf, int length, const PwgenOptions* opts)
{
    if (!buf || length <= 0 || !opts)
        return 2;

    // Однократная инициализация rand() для стандартного режима
    static int rand_initialized = 0;
    if (!opts->secure && !rand_initialized) {
        srand((unsigned int)time(NULL));
        rand_initialized = 1;
    }

    char alphabet[256];
    int alen = charset_build(opts, alphabet, sizeof(alphabet));
    if (alen <= 0)
        return 2;

    for (int i = 0; i < length; i++) {
        int idx = opts->secure ? random_index(alen) : (rand() % alen);
        buf[i] = alphabet[idx];
    }
    buf[length] = '\0';

    ensure_requirements(buf, length, opts);
    return 0;
}
