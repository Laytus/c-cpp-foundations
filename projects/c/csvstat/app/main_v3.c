#include "line_reader.h"
#include "csv.h"
#include "stats.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

// Print usage to stderr
static void usage(const char *prog) {
    fprintf(stderr, "Usage: %s <csv-file> <column-name>\n", prog);
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
    if (!s || !out) return -1;

    // Reject empty string
    if (s[0] == '\0') return -1;

    char *end = NULL;
    double v = strtod(s, &end);

    // No conversion performed
    if (end == s) return -1;

    // Allow trailing spaces/tabs only
    while (*end == ' ' || *end == '\t') end++;

    // Any remaining characters => invalid numeric token
    if (*end != '\0') return -1;

    // Treat ERANGE as invalid (overflow/underflow)
    if (errno == ERANGE) return -1;

    *out = v;
    return 0;
}

int main(int argc, char **argv) {
    if (argc != 3) {
        usage(argv[0]);
        return 1;
    }

    const char *path = argv[1];
    const char *col_name = argv[2];

    FILE *fp = fopen(path, "rb");
    if (!fp) {
        perror("fopen");
        return 2;
    }

    LineReader lr;
    if (line_reader_init(&lr, fp) != 0) {
        fprintf(stderr, "line_reader_init failed\n");
        fclose(fp);
        return 3;
    }

    CsvParser parser;
    if (csv_parser_init(&parser, 16) != 0) {
        fprintf(stderr, "csv_parser_init failed\n");
        line_reader_destroy(&lr);
        fclose(fp);
        return 4;
    }

    const char *line = NULL;
    size_t len = 0;

    // ---- Read header (skip empty lines) ----
    CsvRowView header = (CsvRowView){0};
    size_t col_index = 0;
    int got_header = 0;

    for (;;) {
        int rc = line_reader_next(&lr, &line, &len);
        if (rc == 1) {
            fprintf(stderr, "Empty file: no header row\n");
            csv_parser_destroy(&parser);
            line_reader_destroy(&lr);
            fclose(fp);
            return 5;
        }
        if (rc != 0) {
            fprintf(stderr, "Error reading header line\n");
            csv_parser_destroy(&parser);
            line_reader_destroy(&lr);
            fclose(fp);
            return 6;
        }

        if (is_empty_line(line)) {
            continue; // skip empty/whitespace-only lines
        }

        // `csv_split` modifies the line buffer, so we must cast away `const`.
        // This is safe because the underlying buffer is owned by LineReader and mutable.
        if (csv_split(&parser, (char *)line, &header) != 0) {
            fprintf(stderr, "csv_split failed on header\n");
            csv_parser_destroy(&parser);
            line_reader_destroy(&lr);
            fclose(fp);
            return 7;
        }
        
        if (csv_find_column(&header, col_name, &col_index) != 0) {
            fprintf(stderr, "Column not found in header: %s\n", col_name);
            csv_parser_destroy(&parser);
            line_reader_destroy(&lr);
            fclose(fp);
            return 8;
        }

        got_header = 1;
        break;
    }

    if (!got_header) {
        fprintf(stderr, "Failed to read header\n");
        csv_parser_destroy(&parser);
        line_reader_destroy(&lr);
        fclose(fp);
        return 9;
    }

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
            fprintf(stderr, "Error reading row\n");
            csv_parser_destroy(&parser);
            line_reader_destroy(&lr);
            fclose(fp);
            return 10;
        }

        if (is_empty_line(line)) {
            continue; // skip empty/whitespace-only lines
        }

        if (csv_split(&parser, (char *)line, &row) != 0) {
            fprintf(stderr, "csv_split failed on row %zu\n", row_no);
            csv_parser_destroy(&parser);
            line_reader_destroy(&lr);
            fclose(fp);
            return 11;
        }

        rows_seen++;

        if (col_index >= row.nfields) {
            // Row has fewer fields than header. For v1: skip quietly or warn.
            // We'll warn and continue.
            missing_col++;
            fprintf(stderr, "Row %zu: missing column %s\n", row_no, col_name);
            row_no++;
            continue;
        }

        const char *cell = row.fields[col_index];
        double x = 0.0;

        if (parse_double_strict(cell, &x) != 0) {
            numeric_bad++;
            row_no++;
            continue;
        }

        if (stats_push(&st, x) != 0) {
            fprintf(stderr, "stats_push failed (unexpected)\n");
            csv_parser_destroy(&parser);
            line_reader_destroy(&lr);
            fclose(fp);
            return 12;
        }
        
        printf("%s\n", row.fields[col_index]);
        numeric_ok++;
        row_no++;
    }

    // ---- Print summary ----
    printf("column: %s\n", col_name);
    printf("rows_seen: %zu\n", rows_seen);
    printf("missing_column: %zu\n", missing_col);
    printf("numeric_ok: %zu\n", numeric_ok);
    printf("numeric_bad: %zu\n", numeric_bad);

    double v = 0.0;

    if (stats_min(&st, &v) == 0) {
        printf("min: %.17g\n", v);
    } else {
        printf("min: n/a\n");
    }

    if (stats_max(&st, &v) == 0) {
        printf("max: %.17g\n", v);
    } else {
        printf("max: n/a\n");
    }

    if (stats_mean(&st, &v) == 0) {
        printf("mean: %.17g\n", v);
    } else {
        printf("mean: n/a\n");
    }

    if (stats_stddev_sample(&st, &v) == 0) {
        printf("stddev_sample: %.17g\n", v);
    } else {
        printf("stddev_sample: n/a\n");
    }

    csv_parser_destroy(&parser);
    line_reader_destroy(&lr);
    fclose(fp);
    return 0;
}