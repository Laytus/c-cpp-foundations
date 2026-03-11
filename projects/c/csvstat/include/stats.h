#ifndef STATS_H
#define STATS_H

#include <stddef.h>

/*
Stats accumulator for streaming numeric data.

This module owns no heap memory.
`Stats` is a plain value-type object that is safe to allocate on the stack
and safe to copy by value.
*/
typedef struct {
    size_t n;     // number of valid samples
    double mean;  // running mean
    double m2;    // running sum of squares of differences from the mean (Welford)
    double min;
    double max;
} Stats;

int stats_is_valid(const Stats *st);

/*
Return 1 if the Stats object satisfies its internal invariants, else 0.

This is mainly intended for internal/debug validation.
*/
size_t stats_count(const Stats *s);
int stats_has_data(const Stats *s);
int stats_has_sample_variance(const Stats *s);

/*
Initialize a Stats accumulator.

After initialization:
- n = 0
- mean = 0
- m2 = 0
- min/max are not meaningful until the first accepted sample
*/
void stats_init(Stats *s);

/*
Add one sample to the accumulator.

Returns:
- 0 on success
- -1 on invalid input or if the update would produce an invalid state
*/
int stats_push(Stats *s, double x);

/*
Derived quantities
------------------

Requirements:
- `stats_mean`, `stats_min`, `stats_max` require `n > 0`
- `stats_variance_sample`, `stats_stddev_sample` require `n >= 2`

On failure:
- output is set to 0
- the function returns -1
*/
int stats_mean(const Stats *s, double *out_mean);
int stats_min(const Stats *s, double *out_min);
int stats_max(const Stats *s, double *out_max);
int stats_variance_sample(const Stats *s, double *out_var); // sample variance
int stats_stddev_sample(const Stats *s, double *out_std);   // sqrt(sample variance)

#endif