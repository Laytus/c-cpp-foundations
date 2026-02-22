#include "line_reader.h"

#include <stdio.h>
#include <stdlib.h>

/*
Phase 2 test harness

Why this file exists
--------------------
Before we build CSV parsing and stats, we want to validate that the foundational
I/O primitive (reading lines) is correct and sanitizer-clean.

This `main` is intentionally simple:
- open file
- read file
- print line number and content
- exit
*/

static void usage(const char *prog) {
    fprintf(stderr, "Usage: %s <file>\n", prog);
}

/*
- argc = argument count
- argv = argument vector (array of strings)
*/
int main(int argc, char **argv) {
    if (argc != 2) {
        usage(argv[0]);
        return 1;
    }

    /*
    Open in binary mode ("rb") so we do not rely on any text-mode newline
    translations. We handle '\n' and optional '\r' ourselves.
    */
    FILE *fp = fopen(argv[1], "rb");
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

    const char *line = NULL;
    size_t len = 0;
    size_t line_no = 0;

    for (;;) {
        /*
        `line` is a borrowed pointer:
        - valid until next call to `line_reader_next()`
        - do not free it
        - copy it if we need it later
        */
        int rc = line_reader_next(&lr, &line, &len);

        if (rc == 0) {
            printf("%zu: (%zu) %s\n", line_no, len, line);
            line_no++;
            continue;
        }

        if (rc == 1) {
            // Normal EOF.
            break;
        }

        // rc == -1
        fprintf(stderr, "line_reader_next error\n");
        line_reader_destroy(&lr);
        fclose(fp);
        return 4;
    }

    line_reader_destroy(&lr);
    fclose(fp);
    return 0;
}