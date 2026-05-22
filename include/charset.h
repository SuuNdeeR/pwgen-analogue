#ifndef CHARSET_H
#define CHARSET_H

#include "args.h"
#include <stddef.h>

#define SPECIAL_CHARS "!@#$%^&*()_+-=[]{}|;:,.<>?/~`"

int charset_build(const PwgenOptions* opts, char* out, size_t out_size);
int charset_is_special_char(char c);

#endif
