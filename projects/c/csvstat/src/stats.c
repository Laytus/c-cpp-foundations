#include "stats.h"
#include "csvstat_assert.h"

#include <math.h>   // sqrt

int stats_is_valid(const Stats *st) {
    if (!st) return 0;

    if (st->n == 0) {
        // when empty, min/max may be uninitialized depending on the design;
        // but M2 should be 0 and mean should be 0 (or stable).
        if (st->m2 != 0.0) return 0;
        return 1;
    }

    // when non-empty, min <= max and n >= 1, and m2 should not go negative.
    if (st->n < 1) return 0;
    if (st->min > st->max) return 0;

    // Numeric drift shouldn't make this negative; allow a tiny epsilon if wanted.
    if (st->m2 < 0.0) return 0;
    
    return 1;
}

size_t stats_count(const Stats *s) {
    return s ? s->n : 0;
}

int stats_has_data(const Stats *s) {
    return (s && s->n > 0) ? 1 : 0;
}

int stats_has_sample_variance(const Stats *s) {
    return (s && s->n >= 2) ? 1 : 0;
}

void stats_init(Stats *s) {
    if (!s) return;
    s->n = 0;
    s->mean = 0.0;
    s->m2 = 0.0;
    s->min = 0.0;
    s->max = 0.0;

    CSVSTAT_ASSERT(stats_is_valid(s));
}

int stats_push(Stats *s, double x) {
    if (!s) return -1;

    // Defensive: reject NaN/Inf so they never poison the running stats.
    if (!isfinite(x)) return -1;

    CSVSTAT_ASSERT(stats_is_valid(s));

    // First value initializes the running state.
    if (s->n == 0) {
        s->n = 1;
        s->mean = x;
        s->m2 = 0.0;
        s->min = x;
        s->max = x;

        if (!isfinite(s->mean) || !isfinite(s->m2) || !isfinite(s->min) || !isfinite(s->max)) return -1;

        CSVSTAT_ASSERT(stats_is_valid(s));
        return 0;
    }

    if (s->n == (size_t)-1) return -1; // overflow guard
    s->n++;
    double delta = x - s->mean;
    s->mean += delta / (double)s->n;
    double delta2 = x - s->mean;
    s->m2 += delta * delta2;

    // If the state becomes non-finite, signal failure.
    if (!isfinite(s->mean) || !isfinite(s->m2)) return -1;
    
    if (x < s->min) s->min = x;
    if (x > s->max) s->max = x;
    
    if (!isfinite(s->min) || !isfinite(s->max)) return -1;

    CSVSTAT_ASSERT(stats_is_valid(s));
    return 0;
}

int stats_mean(const Stats *s, double *out_mean) {
    if (!s || !out_mean) return -1;
    if (s->n == 0) { *out_mean = 0.0; return -1; }
    *out_mean = s->mean;
    return 0;
}

int stats_min(const Stats *s, double *out_min) {
    if (!s || !out_min) return -1;
    if (s->n == 0) { *out_min = 0.0; return -1; }
    *out_min = s->min;
    return 0;
}

int stats_max(const Stats *s, double *out_max) {
    if (!s || !out_max) return -1;
    if (s->n == 0) { *out_max = 0.0; return -1; }
    *out_max = s->max;
    return 0;
}

int stats_variance_sample(const Stats *s, double *out_var) {
    if (!s || !out_var) return -1;
    if (s->n < 2) { *out_var = 0.0; return -1; }
    *out_var = s->m2 / (double)(s->n - 1);
    return 0;
}

int stats_stddev_sample(const Stats *s, double *out_std) {
    if (!s || !out_std) return -1;
    double var = 0.0;
    if (stats_variance_sample(s, &var) != 0) { *out_std = 0.0; return -1; }
    *out_std = sqrt(var);
    return 0;
}