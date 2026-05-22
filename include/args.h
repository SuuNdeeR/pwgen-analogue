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

int parse_args(int argc, char* argv[], PwgenOptions* opts);
void set_default_options(PwgenOptions* opts);
void print_help(void);
void print_version(void);

#endif
