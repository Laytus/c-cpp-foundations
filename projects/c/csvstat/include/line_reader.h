#ifndef LINE_READER_H
#define LINE_READER_H

/*
LineReader: a small utility module to read text lines of arbitrary length.

Why this exists
---------------
C does not provide a portable standard function that reads a whole line with
dynamic growth. `fgets()` requires a fixed-size buffer, and `getline()` is
POSIX, not ISO C.

This module provides:
- no fixed maximum line length
- a clear ownership/lifetime model
- a small API that higher-level modules (such as CSV parsers) can depend on

Ownership model
---------------
- LineReader owns an internal heap-allocated character buffer.
- `line_reader_next()` returns a borrowed pointer to that buffer via `out_line`.
- The caller must not free or modify the returned pointer.
- The returned pointer remains valid until:
  - the next call to `line_reader_next()`, or
  - `line_reader_destroy()`

Return code convention
----------------------
- 0  : success, a line was read (possibly empty)
- 1  : EOF reached, no line was read
- -1 : error (I/O error or allocation failure)

Newline normalization
---------------------
`line_reader_next()` strips:
- trailing '\n' (Unix newline)
- optional trailing '\r' (Windows CRLF)
*/

#include <stdio.h>   // FILE
#include <stddef.h>  // size_t

typedef struct LineReader {
    FILE    *fp;      // input stream (NOT owned; caller manages fopen/fclose)
    char    *buf;     // owned internal buffer
    size_t  len;      // current line length (after stripping newline/CR)
    size_t  cap;      // buffer capacity in bytes
    int     saw_eof;  // sticky EOF: once EOF is seen, further reads return EOF
} LineReader;


/*
Return 1 if the LineReader satisfies its internal invariants, else 0.

This is mainly intended for internal/debug validation.
*/
int line_reader_is_valid(const LineReader *lr);

/*
Initialize a LineReader bound to an existing FILE*.

Requirements:
- `lr` must be a valid pointer.
- `fp` must be a valid open stream.
- The caller retains ownership of `fp` and must eventually call `fclose()`.

Returns:
- 0 on success
- -1 on error (invalid input or allocation failure)
*/
int line_reader_init(LineReader *lr, FILE *fp);

/*
Destroy the LineReader and release its owned resources.

Safe to call multiple times on the same object.
Does NOT call `fclose()` on `fp` because the stream is not owned.
*/
void line_reader_destroy(LineReader *lr);

/*
Read the next line from the stream.

Outputs:
- `out_line` receives a borrowed pointer to the internal NUL-terminated buffer.
- `out_len` (optional) receives the length in bytes, excluding the NUL terminator.

Return values:
- 0  : success, a line was read
- 1  : EOF reached, no line was read
- -1 : error (I/O error or allocation failure)

Lifetime:
- The returned pointer becomes invalid after the next call to
  `line_reader_next()` or after `line_reader_destroy()`.
*/
int line_reader_next(LineReader *lr, const char **out_line, size_t *out_len);

#endif