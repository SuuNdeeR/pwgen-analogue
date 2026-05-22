#include "ctest.h"
#include "random.h"

CTEST(random_module, init_and_cleanup)
{
    const int init_result = random_init();
    ASSERT_EQUAL(0, init_result);
    random_cleanup();
}

CTEST(random_module, index_bounds)
{
    random_init();

    const int max_bound = 15;
    const int iterations = 500;

    for (int i = 0; i < iterations; i++) {
        const int idx = random_index(max_bound);
        ASSERT_TRUE(idx >= 0 && idx < max_bound);
    }

    random_cleanup();
}

CTEST(random_module, invalid_max)
{
    random_init();

    ASSERT_EQUAL(0, random_index(0));
    ASSERT_EQUAL(0, random_index(-10));

    random_cleanup();
}

CTEST(random_module, index_without_init_returns_zero)
{
    // g_urandom_fd == -1
    ASSERT_EQUAL(0, random_index(10));
}

CTEST(random_module, cleanup_without_init_safe)
{
    // Не должно упасть, если cleanup вызван без init
    random_cleanup();
    random_cleanup(); // двойной cleanup
}
