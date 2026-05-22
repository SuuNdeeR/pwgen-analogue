#include "output.h"
#include <stdio.h>

void output_print(const char* password)
{
    if (password == NULL) {
        return;
    }
    printf("%s\n", password);
}