#include "random.h"
#include <fcntl.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

static int g_urandom_fd = -1;

int random_init(void)
{
    g_urandom_fd = open("/dev/urandom", O_RDONLY);
    if (g_urandom_fd < 0) {
        return 2;
    }
    return 0;
}

void random_cleanup(void)
{
    if (g_urandom_fd >= 0) {
        close(g_urandom_fd);
        g_urandom_fd = -1;
    }
}

int random_index(int max)
{
    if (max <= 0) {
        return 0;
    }
    if (g_urandom_fd < 0) {
        return 0;
    }

    const uint32_t u_max = (uint32_t)max;
    // Метод отбраковки (Rejection Sampling) для защиты от modulo bias
    const uint32_t limit = UINT32_MAX - (UINT32_MAX % u_max);
    uint32_t val = 0;

    while (1) {
        if (read(g_urandom_fd, &val, sizeof(val)) != (ssize_t)sizeof(val)) {
            return 0;
        }
        if (val < limit) {
            break;
        }
    }

    return (int)(val % u_max);
}