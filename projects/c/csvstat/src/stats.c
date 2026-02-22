#include "stats.h"

#include <math.h>   // sqrt
// #include <float.h>  // DBL_MAX

void stats_init(Stats *s) {
    if (!s) return;
    s->n = 0;
    s->mean = 0.0;
    s->m2 = 0.0;
    s->min = 0.0;
    s->max = 0.0;
}

int stats_push(Stats *s, double x) {
    if (!s) return -1;

    // If Stats is empty
    if (s->n == 0) {
        s->n = 1;
        s->mean = x;
        s->m2 = 0.0;
        s->min = x;
        s->max = x;
        return 0;
    }

    s->n++;
    double delta = x - s->mean;
    s->mean += delta / (double)s->n;
    double delta2 = x - s->mean;
    s->m2 += delta * delta2;

    if (x < s->min) s->min = x;
    if (x > s->max) s->max = x;

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