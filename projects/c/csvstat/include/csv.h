#ifndef CSV_H
#define CSV_H

/*
CSV parser (v1) rules
---------------------
- Delimiter: ','
- No quoted fields / no escaping
- Leading/trailing whitespace is trimmed (spaces + tabs)
- Empty fields are preserved (e.g. a,,b gives an empty middle field)
- Empty lines may be skipped by the caller, depending on caller policy
- Parsing is done in-place: delimiter and trailing-whitespace bytes may be
  replaced with '\0' terminators

Ownership / Lifetime
--------------------
- `csv_split()` modifies the provided `line` buffer in-place.
- `CsvParser` owns an internal reusable scratch array of field pointers.
- `CsvRowView` does not own field storage.
- `CsvRowView.fields` points into parser-owned scratch storage.
- Each field pointer points into the caller-provided `line` buffer.
- A row view becomes invalid after:
  - the next `csv_split()` call using the same `CsvParser`, or
  - the underlying line buffer is overwritten or destroyed
*/

#include <stddef.h>

/*
A borrowed view of one parsed CSV row.

This structure does not own field strings.
Field pointers refer to the current mutable line buffer passed to `csv_split()`.
*/
typedef struct {
    const char **fields;  // owned scratch array (reused) – field pointers
    size_t nfields;       // number of fields in the current row
} CsvRowView;

/*
CSV parser state.

Owns a reusable scratch array used to store field pointers for the current row.
*/
typedef struct {
  const char **scratch; // owned scratch array storage
  size_t cap;           // capacity (#pointers)
} CsvParser;

/*
Return 1 if the CsvParser satisfies its internal invariants, else 0.

This is mainly intended for internal/debug validation.
*/
int csv_parser_is_valid(const CsvParser *p);


/*
Initialize parser scratch storage.

If `initial_capacity` is 0, a default capacity may be chosen internally.

Returns:
- 0 on success
- -1 on allocation failure or invalid input
*/
int csv_parser_init(CsvParser *p, size_t initial_capacity);


/*
Destroy parser scratch storage.

Safe to call multiple times on the same object.
*/
void csv_parser_destroy(CsvParser *p);

/*
Split `line` into fields in-place and return a row view.

Requirements:
- `p` must be a valid initialized parser
- `line` must be a mutable NUL-terminated buffer
- `out` must be a valid output pointer

Effects:
- Modifies `line` by inserting '\0' terminators
- Writes field pointers into parser-owned scratch storage
- Sets `out->fields` and `out->nfields`

Returns:
- 0 on success
- -1 on failure (allocation failure or invalid input)
*/
int csv_split(CsvParser *p, char *line, CsvRowView *out);

/*
Find a column index in a parsed header row.

Returns:
- 0 on success and writes the index to `out_index`
- -1 if not found or if inputs are invalid
*/
int csv_find_column(const CsvRowView *header, const char *name, size_t *out_index);

#endif