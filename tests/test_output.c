#include "ctest.h"
#include "output.h"
#include <stddef.h>

CTEST(output_module, print_valid_string)
{
    // Проверяем базовую работоспособность функции без падений
    output_print("ValidPassword123!");
}

CTEST(output_module, print_null_pointer)
{
    // Защита от передачи NULL-указателя должна корректно срабатывать
    output_print(NULL);
}