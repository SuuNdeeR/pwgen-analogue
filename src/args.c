#include "args.h"
#include <stdio.h>

void set_default_options(PwgenOptions* opts)
{
    opts->length = 8;
    opts->count = 1;
    opts->capitalize = false;
    opts->no_capitalize = false;
    opts->numerals = false;
    opts->no_numerals = false;
    opts->symbols = false;
    opts->secure = false;
    opts->no_ambiguous = false;
}

int parse_args(int argc, char* argv[], PwgenOptions* opts)
{
    (void)argc;
    (void)argv;
    (void)opts;
    return 0;
}

void print_help(void)
{
    printf("Использование: ./bin/pwgen [ОПЦИИ] [ДЛИНА [КОЛИЧЕСТВО]]\n");
}

void print_version(void)
{
    printf("Версия: pwgen 1.0\n");
}
