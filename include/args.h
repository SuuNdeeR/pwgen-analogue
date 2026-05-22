#ifndef ARGS_H
#define ARGS_H

#include <stdbool.h>

typedef struct {
    int length;
    int count;
    bool capitalize;
    bool no_capitalize;
    bool numerals;
    bool no_numerals;
    bool symbols;
    bool secure;
    bool no_ambiguous;
} PwgenOptions;

typedef enum {
    PARSE_OK = 0,
    PARSE_ERROR = 1,
    PARSE_HELP = 2,
    PARSE_VERSION = 3
} ParseResult;

void set_default_options(PwgenOptions* opts);
ParseResult parse_args(int argc, char* argv[], PwgenOptions* opts);
void print_help(void);
void print_version(void);

#endif /* ARGS_H */
