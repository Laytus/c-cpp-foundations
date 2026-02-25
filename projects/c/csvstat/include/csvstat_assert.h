#ifndef CSVSTAT_ASSERT_H
#define CSVSTAT_ASSERT_H

#include <stdio.h>
#include <stdlib.h>

#ifdef CSVSTAT_DEBUG
#define CSVSTAT_ASSERT(expr)                                                      \
    do {                                                                          \
        if (!(expr)) {                                                            \
            fprintf(stderr, "csvstat: ASSERT failed: %s (%s:%d)\n",               \
                    #expr, __FILE__, __LINE__);                                   \
            abort();                                                              \
        }                                                                         \
    } while (0)
#else
#define CSVSTAT_ASSERT(expr) do { (void)sizeof(expr); } while (0)
#endif

#endif