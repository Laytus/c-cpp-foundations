#include "line_reader.h"
#include "csv.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void usage(const char *prog) {
    fprintf(stderr, "Usage: %s <csv-file> <column-name>\n", prog);
}

static int is_empty_line(const char *s) {
    // "empty line" policy: skip empty or whitespace-only
    while (*s) {
        if (*s != ' ' && *s != '\t') return 0;
        s++;
    }
    
    return 1;
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
    CsvRowView header = {0};
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

    // ---- Process rows ----
    CsvRowView row = {0};
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

        if (col_index >= row.nfields) {
            // Row has fewer fields than header. For v1: skip quietly or warn.
            // We'll warn and continue.
            fprintf(stderr, "Row %zu: missing column %s\n", row_no, col_name);
            row_no++;
            continue;
        }
        
        printf("%s\n", row.fields[col_index]);
        row_no++;
    }

    csv_parser_destroy(&parser);
    line_reader_destroy(&lr);
    fclose(fp);
    return 0;
}