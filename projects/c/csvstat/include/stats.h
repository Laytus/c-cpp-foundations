#ifndef STATS_H
#define STATS_H

#include <stddef.h>

typedef struct {
    size_t n;     // number of valid samples
    double mean;  // running mean
    double m2;    // running sum of squares of differences from the mean (Welford)
    double min;
    double max;
} Stats;

int stats_is_valid(const Stats *st);

/*
Query helpers

These make "de we have enough data?" decisions explicit and keep `main` and tests
from duplicating `n` checks.

Return values:
- stats_count: returns 0 if s == NULL
- stats_has_data: 1 if there is at least one sample, else 0
- stats_has_sample_variance: 1 if n >= 2, else 0
*/
size_t stats_count(const Stats *s);
int stats_has_data(const Stats *s);
int stats_has_sample_variance(const Stats *s);

/*
Initialize stats accumulator.
After init: n=0, mean-0, m2=0, min/max undefined until first sample.
*/
void stats_init(Stats *s);

/*
Add a sample x to the accumulator.
Returns:
- 0 on success
- -1 on invalid input pointer
*/
int stats_push(Stats *s, double x);

/*
Derived quantities. 
If n == 0 (or n < 2 for variance), outputs are set to 0 and the function returns -1.
*/
int stats_mean(const Stats *s, double *out_mean);
int stats_min(const Stats *s, double *out_min);
int stats_max(const Stats *s, double *out_max);
int stats_variance_sample(const Stats *s, double *out_var); // sample variance
int stats_stddev_sample(const Stats *s, double *out_std);   // sqrt(sample variance)

#endif