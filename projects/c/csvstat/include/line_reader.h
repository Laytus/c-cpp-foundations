#ifndef LINE_READER_H
#define LINE_READER_H

/*
LineReader: a small utility module to read text lines of arbitrary length.

Why this exists
---------------
C does not provide a standard "read a whole line with dynamic growth" function
that is portable and safe. `fgets()` requires a fixed buffer size, and `getline()`
is POSIC (not ISO C). We want:

- No fixed maximum line length
- A clear ownership/lifetime model
- A small API that higher-level modules (CSV parses) can depend on

Ownership model
---------------
- LineReader owns an internal char buffer (heap allocation).
- `line_reader_next()` returns a borrowed pointer to that buffer via `out_line`.
- The returned pointer remains valid until:
    - the next call to `line_reader_next()`, or
    - `line_reader_destroy()`

Return code convention
----------------------
- 0  : succes, a line was read (may be empty)
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
    int     saw_oef;  // sticky EOF: once EOF is seen, further reads return EOF
} LineReader;

/*
Initialize a LineReader bound to an existing FILE*.

- `fp` must be a valid open stream.
- The reader does NOT take ownership of `fp`; caller must `fclose()`.

Returns:
- 0 on success
- -1 on error (allocation failure or invalid inputs)
*/
int line_reader_init(LineReader *lr, FILE *fp);

/*
Destroy the LineReader and release owned resources.

Safe to call multiple times on the same object.
Does NOT `fclose()` the FILE* (not owned).
*/
void line_reader_destroy(LineReader *lr);

/*
Read the next line from the stream.

Inputs:
- `*lr`: LineReader passed by pointer
- `**out_line`: the address of a (char *) pointer
- `*out_len`: size_t passed by pointer

Notes:
- `line` lives in caller
- `out_lines` is a pointer to it
- `*out_line` modifies caller's variable

Outputs:
- `out_line` receives a borrowed pointer to the internal NUL-terminated buffer.
- `out_len` (optional) receives the length in bytes (excluding the NUL terminator).

Return values:
- 0  : success, line read (out_line points to valid data)
- 1  : EOF, no line read (out_line set to NULL)
- -1 : error (I/O error or allocation failure)

Notes:
- The returned pointer becomes invalid after the next call to `line_reader_next()``
  or after `line_reader_destroy()`.
*/
int line_reader_next(LineReader *lr, const char **out_line, size_t *out_len);

#endif