#ifndef CHARSET_H
#define CHARSET_H

#include "args.h"   /* для PwgenOptions */
#include <stddef.h> /* для size_t */

int charset_build(const PwgenOptions* opts, char* out, size_t out_size);

#endif
