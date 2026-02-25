#include "line_reader.h"
#include "csv.h"
#include "stats.h"
#include "csvstat_err.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

typedef struct {
    const char *file_path;
    const char *col_name;
    int quiet;
} CliOptions;

// Print usage to stderr
static void usage(FILE *out, const char *prog) {
    /*
    FILE *out:
    - stdout: for --help
    - stderr: for argument errors
    */
    fprintf(out, 
        "csvstat – compute streaming stats for a numeric CSV column (v1)\n\n"
        "Usage:\n"
        "  %s <csv-file> <column-name>\n"
        "  %s --file <csv-file> --col <column-name> [--quiet]\n"
        "  %s --help\n\n"
        "Options:\n"
        "  --file <path>     Input CSV file\n"
        "  --col  <name>     Column name (must exist in header row)\n"
        "  --quiet           Suppress non-fatal warnings\n"
        "  --help            Show this help\n",
        prog, prog, prog
    );
}

static int parse_cli(int argc, char **argv, CliOptions *opt) {
    /*
    int argc: argument count
    char **argv: array of argument strings
    CliOptions *opt: pointer to struct where parsed values will be stored
    */

    if (!opt) return -1;

    // Initialize defaults
    opt->file_path = NULL;
    opt->col_name = NULL;
    opt->quiet = 0;

    if (argc == 2 && strcmp(argv[1], "--help") == 0) {
        usage(stdout, argv[0]);
        return 1; // Help was printed; main() should exit normally.
    }

    // Backward-compatible positional form: csvstat file col
    if (argc == 3 && argv[1][0] != '-') {
        opt->file_path = argv[1];
        opt->col_name = argv[2];
        return 0;
    }

    // Flag-based form
    for (int i = 1; i < argc; i++) {
        const char *a = argv[i];

        if (strcmp(a, "--help") == 0) {
            usage(stdout, argv[0]);
            return 1;
        } else if (strcmp(a, "--quiet") == 0) {
            opt->quiet = 1;
        } else if (strcmp(a, "--file") == 0) {
            if (i + 1 >= argc) {
                // fprintf(stderr, "Missing value for --file\n");
                return -1;
            }
            opt->file_path = argv[++i];
        } else if (strcmp(a, "--col") == 0) {
            if (i + 1 >= argc) {
                // fprintf(stderr, "Missing value for --col\n");
                return -1;
            }
            opt->col_name = argv[++i];
        } else {
            // fprintf(stderr, "Unknown argument: %s\n", a);
            return -1;
        }
    }

    if (!opt->file_path || !opt->col_name) {
        // fprintf(stderr, "Missing required arguments.\n");
        // usage(stderr, argv[0]);
        return -1;
    }

    return 0;
}

static int die(CsvStatErr code, const char *context) {
    if (context && context[0]) {
        fprintf(stderr, "csvstat: %s: %s\n", context, csvstat_err_str(code));
    } else {
        fprintf(stderr, "csvstat: %s\n", csvstat_err_str(code));
    }
    return (int)code;
}

static int die_errno(CsvStatErr code, const char *context, int errnum) {
    const char *detail = (errnum != 0) ? strerror(errnum) : NULL;

    if (context && context[0]) {
        if (detail) {
            fprintf(stderr, "csvstat: %s: %s (%s)\n", context, csvstat_err_str(code), detail);
        } else {
            fprintf(stderr, "csvstat: %s: %s\n", context, csvstat_err_str(code));
        }
    } else {
        if (detail) {
            fprintf(stderr, "csvstat: %s (%s)\n", csvstat_err_str(code), detail);
        } else {
            fprintf(stderr, "csvstat: %s\n", csvstat_err_str(code));
        }
    }

    return (int)code;
}

/*
Empty-line policy:
Skip empty or whitespace-only lines.
We treat spaces and tabs as whitespace for this project version.
*/
static int is_empty_line(const char *s) {
    // "empty line" policy: skip empty or whitespace-only
    while (*s) {
        if (*s != ' ' && *s != '\t') return 0;
        s++;
    }
    
    return 1;
}

/*
Strict numeric parsing:
- rejects empty strings
- uses strtod
- allows trailing spaces/tabs only
- rejects junk suffix (e.g., "12abc")
Returns:
- 0 on success and writes to *out
- -1 on failure
*/
static int parse_double_strict(const char *s, double *out) {
    // Null checks
    if (!s || !out) return -1;

    // Reject empty string
    if (s[0] == '\0') return -1;

    char *end = NULL;
    errno = 0;
    double v = strtod(s, &end);

    // No conversion performed
    if (end == s) return -1;

    // Allow trailing spaces/tabs only
    while (*end == ' ' || *end == '\t') end++;

    // Any remaining characters => invalid numeric token
    if (*end != '\0') return -1;

    // Treat ERANGE as invalid (overflow/underflow) - Number too large
    if (errno == ERANGE) return -1;

    // Reject NaN / +/-Inf
    if (!isfinite(v)) return -1;

    *out = v;
    return 0;
}

int main(int argc, char **argv) {
    CliOptions opt;
    int prc = parse_cli(argc, argv, &opt);
    if (prc == 1) return 0;  // help shown
    if (prc != 0) {
        int code = die(CSVSTAT_EARG, "cli");
        usage(stderr, argv[0]);
        return code;
    }

    const char *path = opt.file_path;
    const char *col_name = opt.col_name;

    CsvStatErr err = CSVSTAT_OK;
    int fp_open = 0;
    int lr_init = 0;
    int parser_init = 0;
    int saved_errno = 0;

    FILE *fp = fopen(path, "rb");
    if (!fp) {
        // perror("fopen");
        // return (int)CSVSTAT_EIO;
        err = CSVSTAT_EIO;
        saved_errno = errno;
        goto cleanup;
    }
    fp_open = 1;

    LineReader lr;
    if (line_reader_init(&lr, fp) != 0) {
        // fprintf(stderr, "line_reader_init failed\n");
        // fclose(fp);
        err = CSVSTAT_EIO;
        if (saved_errno == 0) saved_errno = errno;
        goto cleanup;
    }
    lr_init = 1;

    CsvParser parser;
    if (csv_parser_init(&parser, 16) != 0) {
        // fprintf(stderr, "csv_parser_init failed\n");
        // line_reader_destroy(&lr);
        // fclose(fp);
        // return 4;
        err = CSVSTAT_ENOMEM;
        goto cleanup;
    }
    parser_init = 1;

    const char *line = NULL;
    size_t len = 0;

    // ---- Read header (skip empty lines) ----
    CsvRowView header = (CsvRowView){0};
    size_t col_index = 0;
    // int got_header = 0;

    for (;;) {
        int rc = line_reader_next(&lr, &line, &len);
        if (rc == 1) {
            // fprintf(stderr, "Empty file: no header row\n");
            // csv_parser_destroy(&parser);
            // line_reader_destroy(&lr);
            // fclose(fp);
            // return 5;
            err = CSVSTAT_EFORMAT;
            goto cleanup;
        }
        if (rc != 0) {
            // fprintf(stderr, "Error reading header line\n");
            // csv_parser_destroy(&parser);
            // line_reader_destroy(&lr);
            // fclose(fp);
            // return 6;
            err = CSVSTAT_EIO;
            if (saved_errno == 0) saved_errno = errno;
            goto cleanup;
        }

        if (is_empty_line(line)) {
            continue; // skip empty/whitespace-only lines
        }

        // `csv_split` modifies the line buffer, so we must cast away `const`.
        // This is safe because the underlying buffer is owned by LineReader and mutable.
        if (csv_split(&parser, (char *)line, &header) != 0) {
            // fprintf(stderr, "csv_split failed on header\n");
            // csv_parser_destroy(&parser);
            // line_reader_destroy(&lr);
            // fclose(fp);
            // return 7;
            err = CSVSTAT_EFORMAT;
            goto cleanup;
        }
        
        if (csv_find_column(&header, col_name, &col_index) != 0) {
            // fprintf(stderr, "Column not found in header: %s\n", col_name);
            // csv_parser_destroy(&parser);
            // line_reader_destroy(&lr);
            // fclose(fp);
            // return 8;
            err = CSVSTAT_ENOCOL;
            goto cleanup;
        }

        // got_header = 1;
        break;
    }

    // if (!got_header) {
    //     fprintf(stderr, "Failed to read header\n");
    //     csv_parser_destroy(&parser);
    //     line_reader_destroy(&lr);
    //     fclose(fp);
    //     return 9;
    // }

    // ---- Stream rows and accumulate stats ----
    Stats st;
    stats_init(&st);

    size_t rows_seen = 0;    // rows we attempted to process (non-empty)
    size_t numeric_ok = 0;   // successfully parsed numbers
    size_t numeric_bad = 0;  // missing/invalid numbers
    size_t missing_col = 0;  // rows with fewer fields than header

    CsvRowView row = (CsvRowView){0};
    size_t row_no = 0;

    for (;;) {
        int rc = line_reader_next(&lr, &line, &len);
        if (rc == 1) break; // EOF
        if (rc != 0) {
            // fprintf(stderr, "Error reading row\n");
            // csv_parser_destroy(&parser);
            // line_reader_destroy(&lr);
            // fclose(fp);
            // return 10;
            err = CSVSTAT_EIO;
            if (saved_errno == 0) saved_errno = errno;
            goto cleanup;
        }

        if (is_empty_line(line)) {
            continue; // skip empty/whitespace-only lines
        }

        if (csv_split(&parser, (char *)line, &row) != 0) {
            // fprintf(stderr, "csv_split failed on row %zu\n", row_no);
            // csv_parser_destroy(&parser);
            // line_reader_destroy(&lr);
            // fclose(fp);
            // return 11;
            err = CSVSTAT_EFORMAT;
            goto cleanup;
        }

        rows_seen++;

        if (col_index >= row.nfields) {
            // Row has fewer fields than the header (v1 behavior: skip; optionally warn).
            missing_col++;
            // fprintf(stderr, "Row %zu: missing column %s\n", row_no, col_name);
            if (!opt.quiet) {
                fprintf(stderr, "Row %zu: missing column %s\n", row_no, col_name);
            }
            row_no++;
            continue;
        }

        const char *cell = row.fields[col_index];
        double x = 0.0;

        if (parse_double_strict(cell, &x) != 0) {
            numeric_bad++;
            if (!opt.quiet) {
                fprintf(stderr, "Row %zu: invalid number '%s'\n", row_no, cell);
            }
            row_no++;
            continue;
        }

        if (stats_push(&st, x) != 0) {
            // fprintf(stderr, "stats_push failed (unexpected)\n");
            // csv_parser_destroy(&parser);
            // line_reader_destroy(&lr);
            // fclose(fp);
            // return 12;
            err = CSVSTAT_EINTERNAL;
            goto cleanup;
        }
        
        // printf("%s\n", row.fields[col_index]);
        numeric_ok++;
        row_no++;
    }

    // ---- Print summary ----
    printf("file: %s\n", path);
    printf("column: %s\n", col_name);
    printf("rows_seen: %zu\n", rows_seen);
    printf("missing_column: %zu\n", missing_col);
    printf("numeric_ok: %zu\n", numeric_ok);
    printf("numeric_bad: %zu\n", numeric_bad);

    double v = 0.0;
    size_t n = stats_count(&st);

    // if (stats_min(&st, &v) == 0) {
    //     printf("min: %.17g\n", v);
    // } else {
    //     printf("min: n/a\n");
    // }
    if (!stats_has_data(&st)) {
        printf("min: n/a\n");
        printf("max: n/a\n");
        printf("mean: n/a\n");
        printf("stddev_sample: n/a\n");
    } else {
        // n >= 1
        // There should succeed when n > 0; if they fail, treat as internal error.
        if (stats_min(&st, &v) != 0) { 
            err = CSVSTAT_EINTERNAL;
            goto cleanup;
        }
        printf("min: %.17g\n", v);

        if (stats_max(&st, &v) != 0) {
            err = CSVSTAT_EINTERNAL;
            goto cleanup;
        }
        printf("max: %.17g\n", v);
        
        if (stats_mean(&st, &v) != 0) {
            err = CSVSTAT_EINTERNAL;
            goto cleanup;
        }
        printf("mean: %.17g\n", v);
        
        if (!stats_has_sample_variance(&st)) {
            // Sample standard deviation is undefined for n < 2.
            (void)n; // n kept for readibility / potential future printing
            printf("stddev_sample: n/a\n");
        } else {
            if (stats_stddev_sample(&st, &v) != 0) {
                err = CSVSTAT_EINTERNAL;
                goto cleanup;
            }
            printf("stddev_sample: %.17g\n", v);
        }
    }

    // if (stats_max(&st, &v) == 0) {
    //     printf("max: %.17g\n", v);
    // } else {
    //     printf("max: n/a\n");
    // }

    // if (stats_mean(&st, &v) == 0) {
    //     printf("mean: %.17g\n", v);
    // } else {
    //     printf("mean: n/a\n");
    // }

    // if (stats_stddev_sample(&st, &v) == 0) {
    //     printf("stddev_sample: %.17g\n", v);
    // } else {
    //     printf("stddev_sample: n/a\n");
    // }

    // csv_parser_destroy(&parser);
    // line_reader_destroy(&lr);
    // fclose(fp);
    err = CSVSTAT_OK;

cleanup:
    if (parser_init) {
        csv_parser_destroy(&parser);
    }
    if (lr_init) {
        line_reader_destroy(&lr);
    }
    if (fp_open) {
        fclose(fp);
    }

    // Use context strings that help we locate where the failure happened.
    if (err != CSVSTAT_OK) {
        switch (err) {
            case CSVSTAT_EARG:      return die(err, "cli");
            case CSVSTAT_EIO:       return die_errno(err, "io", saved_errno);
            case CSVSTAT_EFORMAT:   return die(err, "format");
            case CSVSTAT_ENOCOL:    return die(err, "header");
            case CSVSTAT_ENOMEM:    return die(err, "memory");
            default:                return die(err, "internal");
        }
    }

    return 0;
}