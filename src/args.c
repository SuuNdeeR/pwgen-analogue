#include "args.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static const int MIN_LENGTH = 4;
static const int MAX_LENGTH = 256;
static const int MIN_COUNT = 1;
static const int MAX_COUNT = 10000;
static const int DEFAULT_LENGTH = 8;
static const int DEFAULT_COUNT = 1;

void set_default_options(PwgenOptions* opts)
{
    opts->length = DEFAULT_LENGTH;
    opts->count = DEFAULT_COUNT;
    opts->capitalize = false;
    opts->no_capitalize = false;
    opts->numerals = false;
    opts->no_numerals = false;
    opts->symbols = false;
    opts->secure = false;
    opts->no_ambiguous = false;
}

static ParseResult
parse_positional(int argc, char* argv[], int optind, PwgenOptions* opts)
{
    const int remaining = argc - optind;

    if (remaining > 2) {
        fprintf(stderr, "Ошибка: слишком много аргументов\n");
        return PARSE_ERROR;
    }

    if (remaining >= 1) {
        char* endptr = NULL;
        const long length_val = strtol(argv[optind], &endptr, 10);
        if (*endptr != '\0' || length_val < MIN_LENGTH
            || length_val > MAX_LENGTH) {
            fprintf(stderr,
                    "Ошибка: длина должна быть от %d до %d\n",
                    MIN_LENGTH,
                    MAX_LENGTH);
            return PARSE_ERROR;
        }
        opts->length = (int)length_val;
    }

    if (remaining >= 2) {
        char* endptr = NULL;
        const long count_val = strtol(argv[optind + 1], &endptr, 10);
        if (*endptr != '\0' || count_val < MIN_COUNT || count_val > MAX_COUNT) {
            fprintf(stderr,
                    "Ошибка: количество должно быть от %d до %d\n",
                    MIN_COUNT,
                    MAX_COUNT);
            return PARSE_ERROR;
        }
        opts->count = (int)count_val;
    }

    return PARSE_OK;
}

ParseResult parse_args(int argc, char* argv[], PwgenOptions* opts)
{
    optind = 1;
    opterr = 0;

    const char* short_opts = "cAn0ysBh";
    struct option long_options[]
            = {{"capitalize", no_argument, NULL, 'c'},
               {"no-capitalize", no_argument, NULL, 'A'},
               {"numerals", no_argument, NULL, 'n'},
               {"no-numerals", no_argument, NULL, '0'},
               {"symbols", no_argument, NULL, 'y'},
               {"secure", no_argument, NULL, 's'},
               {"no-ambiguous", no_argument, NULL, 'B'},
               {"help", no_argument, NULL, 'h'},
               {"version", no_argument, NULL, 'v'},
               {NULL, 0, NULL, 0}};

    int opt = 0;
    while ((opt = getopt_long(argc, argv, short_opts, long_options, NULL))
           != -1) {
        if (opt == 'c') {
            opts->capitalize = true;
            opts->no_capitalize = false;
            continue;
        }
        if (opt == 'A') {
            opts->capitalize = false;
            opts->no_capitalize = true;
            continue;
        }
        if (opt == 'n') {
            opts->numerals = true;
            opts->no_numerals = false;
            continue;
        }
        if (opt == '0') {
            opts->numerals = false;
            opts->no_numerals = true;
            continue;
        }
        if (opt == 'y') {
            opts->symbols = true;
            continue;
        }
        if (opt == 's') {
            opts->secure = true;
            continue;
        }
        if (opt == 'B') {
            opts->no_ambiguous = true;
            continue;
        }
        if (opt == 'h') {
            return PARSE_HELP;
        }
        if (opt == 'v') {
            return PARSE_VERSION;
        }

        fprintf(stderr, "Ошибка: неизвестный флаг\n");
        return PARSE_ERROR;
    }

    return parse_positional(argc, argv, optind, opts);
}

void print_help(void)
{
    printf("Использование: pwgen [ОПЦИИ] [ДЛИНА [КОЛИЧЕСТВО]]\n");
    printf("Генератор случайных паролей.\n\n");
    printf("Позиционные аргументы:\n");
    printf("  ДЛИНА       Длина пароля (%d-%d, по умолчанию %d)\n",
           MIN_LENGTH,
           MAX_LENGTH,
           DEFAULT_LENGTH);
    printf("  КОЛИЧЕСТВО  Количество паролей (%d-%d, по умолчанию %d)\n",
           MIN_COUNT,
           MAX_COUNT,
           DEFAULT_COUNT);
    printf("\nФлаги:\n");
    printf("  -c, --capitalize     Включить заглавные буквы\n");
    printf("  -A, --no-capitalize  Не включать заглавные буквы\n");
    printf("  -n, --numerals       Включить цифры\n");
    printf("  -0, --no-numerals    Не включать цифры\n");
    printf("  -y, --symbols        Включить специальные символы\n");
    printf("  -s, --secure         Генерация через /dev/urandom\n");
    printf("  -B, --no-ambiguous   Исключить неоднозначные символы\n");
    printf("  -h, --help           Вывод справки\n");
    printf("      --version        Вывод версии программы\n");
}

void print_version(void)
{
    printf("pwgen 1.0\n");
}
