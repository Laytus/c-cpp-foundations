#ifndef CSV_H
#define CSV_H

/*
CSV parser (v1) rules
---------------------
- Delimiter: ','
- No quoted fields / no escaping
- Leading/trailing whitespace is trimmed (spaces + tabs)
- Empty fields are skipped by the caller (recommended policy)
- Parsing is done in-place: we write '\0' terminators into the input line
  buffer and return field pointers (views) into that buffer.

Ownership / Lifetime
--------------------
- `csv_split()` modifies the provided `line` buffer in-place.
- CsvParser owns an internal reusable array of field pointers (scratch).
- The field pointers in CsvRowView are valid until the next `csv_split()`
  call using the same CsvParser (because the scratch array can be reused)
  AND until the underlying line buffer is overwritten (LineReader `next()`).
*/

#include <stddef.h>

// Represents: A view of a parsed CSV row.
// It only references memory to:
// - the scratch pointer array inside CsvParser
// - the line buffer inside LineReader
typedef struct {
    const char **fields;  // owned scratch array (reused) – field pointers
    size_t nfields;       // number of fields in the current row
} CsvRowView;

// fields → array of pointers → each pointer points to a field string
// fields (array)
//     |
//     +-- fields[0] → "apple"
//     +-- fields[1] → "1.5"
//     +-- fields[2] → "10"

typedef struct {
  const char **scratch; // owned scratch array storage
  size_t cap;           // capacity (#pointers)
} CsvParser;

int csv_parser_is_valid(const CsvParser *p);

/*
Initialize parser scratch storage.

Returns:
- 0 on success
- -1 on allocation failure / invalid input
*/
int csv_parser_init(CsvParser *p, size_t initial_capacity);

/*
Destroy parser scratch storage.
Safe to call multiple times.
*/
void csv_parser_destroy(CsvParser *p);

/*
Split `line` into fields (in-place) and return a row view.

Parameters:
- p: parser (owns scratch array)
- line: mutable NUL-terminated line buffer (will be modified)
- out: output row view (points into `p->scratch` + line)

Returns:
- 0 on success
- -1 on failure (allocation failure or invalid inputs)
*/
int csv_split(CsvParser *p, char *line, CsvRowView *out);

/*
Find a column index in a header row.

Returns:
- 0 on success and writes index to `out_index`
- -1 if not found or invalid inputs
*/
int csv_find_column(const CsvRowView *header, const char *name, size_t *out_index);

#endif