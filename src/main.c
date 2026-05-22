#include "args.h"
#include "charset.h"
#include "generator.h"
#include "output.h"
#include "random.h"
#include <stdio.h>
#include <stdlib.h>

static const int EXIT_RUNTIME_ERROR = 2;

int main(int argc, char* argv[])
{
    PwgenOptions opts;
    set_default_options(&opts);

    const ParseResult result = parse_args(argc, argv, &opts);
    if (result == PARSE_HELP) {
        print_help();
        return EXIT_SUCCESS;
    }
    if (result == PARSE_VERSION) {
        print_version();
        return EXIT_SUCCESS;
    }
    if (result == PARSE_ERROR) {
        return EXIT_FAILURE;
    }

    char alphabet[256];
    const int alen = charset_build(&opts, alphabet, sizeof(alphabet));
    if (alen <= 0) {
        fprintf(stderr, "Пустой алфавит\n");
        return EXIT_RUNTIME_ERROR;
    }

    if (opts.secure) {
        if (random_init() != 0) {
            fprintf(stderr, "Ошибка: не удалось открыть /dev/urandom\n");
            return EXIT_RUNTIME_ERROR;
        }
    }

    char* pwd = malloc((size_t)opts.length + 1U);
    if (pwd == NULL) {
        fprintf(stderr, "Ошибка: не удалось выделить память\n");
        random_cleanup();
        return EXIT_RUNTIME_ERROR;
    }

    for (int i = 0; i < opts.count; i++) {
        if (generate_password(pwd, opts.length, &opts) != 0) {
            free(pwd);
            random_cleanup();
            return EXIT_RUNTIME_ERROR;
        }
        output_print(pwd);
    }

    free(pwd);
    random_cleanup();
    return EXIT_SUCCESS;
}
