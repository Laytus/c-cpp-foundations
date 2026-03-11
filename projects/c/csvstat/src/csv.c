#include "csv.h"
#include "csvstat_assert.h"

#include <stdlib.h>  // malloc, realloc, free
#include <string.h>  // strcmp
#include <ctype.h>   // isspace
#include <limits.h>  // SIZE_MAX

/*
CsvParser invariants:
- If cap == 0 then scratch == NULL
- If cap > 0 then scratch != NULL
*/
int csv_parser_is_valid(const CsvParser *p) {
    if (!p) return 0;

    if (p->cap == 0 && p->scratch != NULL) return 0;
    if (p->cap > 0 && p->scratch == NULL) return 0;

    return 1;
}

static int ensure_ptr_capacity(CsvParser *p, size_t needed) {
    CSVSTAT_ASSERT(csv_parser_is_valid(p));

    if (needed <= p->cap) return 0;

    size_t new_cap = (p->cap == 0) ? 16 : p->cap;
    while (new_cap < needed) {
        if (new_cap > SIZE_MAX / 2) return -1;
        new_cap *= 2;
    } 

    const char **tmp = (const char **)realloc((void *)p->scratch, new_cap * sizeof(const char *));
    if (!tmp) return -1;

    p->scratch = tmp;
    p->cap = new_cap;

    CSVSTAT_ASSERT(csv_parser_is_valid(p));
    return 0;
}

int csv_parser_init(CsvParser *p, size_t initial_capacity) {
    if (!p) return -1;

    p->scratch = NULL;
    p->cap = 0;

    if (initial_capacity == 0) initial_capacity = 16;

    p->scratch = (const char **)malloc(initial_capacity * sizeof(const char *));
    if (!p->scratch) return -1;

    p->cap = initial_capacity;

    CSVSTAT_ASSERT(csv_parser_is_valid(p));
    return 0;
}

void csv_parser_destroy(CsvParser *p) {
    if (!p) return;
    
    free((void *)p->scratch);
    p->scratch = NULL;
    p->cap = 0;

    CSVSTAT_ASSERT(csv_parser_is_valid(p));
}

/*
Trim leading and trailing whitespace in-place by:
- advancing start pointer over leading whitespace
- writing '\0' to cut trailing whitespace
Return spointer to trimmed start.
*/
static char *trim_in_place(char *s) {
    while (*s && (*s == ' ' || *s == '\t')) {
        s++;
    }

    // Trim trailing spaces/tabs by walking from end.
    // We can safely do this because `s` points inside a NUL-terminated field.
    size_t n = 0;
    while (s[n] != '\0') n++;

    while (n > 0) {
        char c = s[n - 1];
        if (c == ' ' || c == '\t') {
            s[n - 1] = '\0';
            n--;
        } else {
            break;
        }
    }

    return s;
}

int csv_split(CsvParser *p, char *line, CsvRowView *out) {
    if (!p || !line || !out) return -1;

    CSVSTAT_ASSERT(csv_parser_is_valid(p));

    out->fields = NULL;
    out->nfields = 0;

    // Worst case: "a,b,c" has fields_count = commas + 1.
    // We'll grow as needed.
    size_t field_count = 0;

    char *s = line;
    char *field_start = s;

    for (;;) {
        char c = *s;

        if (c == ',' || c == '\0') {
            // Terminate current field in-place (if comma).
            if (c == ',') {
                *s = '\0';
            }

            // Ensure room in scratch for this field pointer
            if (ensure_ptr_capacity(p, field_count + 1) != 0) {
                return -1;
            }

            // Trim field and store pointer
            char *trimmed = trim_in_place(field_start);
            p->scratch[field_count] = trimmed;
            field_count++;

            if (c == '\0') {
                break; // end of line
            }

            // Next field starts rigth after the '\0' we just wrote
            field_start = s + 1;
        }

        if (c == '\0') break;
        s++;
    }

    out->fields = p->scratch;
    out->nfields = field_count;

    CSVSTAT_ASSERT(csv_parser_is_valid(p));
    return 0;
}

int csv_find_column(const CsvRowView *header, const char *name, size_t *out_index) {
    if (!header || !name || !out_index) return -1;

    for (size_t i = 0; i < header->nfields; i++) {
        const char *h = header->fields[i];
        if (h && strcmp(h, name) == 0) {
            *out_index = i;
            return 0;
        }
    }
    return -1; // Header not found
}