#include "line_reader.h"

#include <stdlib.h>  // malloc, realloc, free
#include <string.h>  // memset
#include <errno.h>   // errno
#include <stdio.h>   // ferror, feof, fgetc

/*
Implementation notes
--------------------
We implement line reading using `fgetc()` in a loop. This is simple and portable.

Trade-offs:
- `fgetc()` is not the fastest possible approach (we can optimize later by 
  buffering reads), but it is correct and easy to reason about.
- Correctness and clarity matter most at this stage.

Growth strategy:
- The internal buffer grows by doubling.
- This gives amortized O(n) total work as lines grow.
- Doubling avoids frequent reallocations.

Safety:
- We always ensure space for the next character plus the final '\0'.
- We guard against integer overflow when computing new capacity.
*/

/*
typedef struct LineReader {
    FILE    *fp;      // input stream (NOT owned; caller manages fopen/fclose)
    char    *buf;     // owned internal buffer
    size_t  len;      // current line length (after stripping newline/CR)
    size_t  cap;      // buffer capacity in bytes
    int     saw_oef;  // sticky EOF: once EOF is seen, further reads return EOF
} LineReader;
*/

/*
Ensure the internal buffer has at least `needed` bytes capacity.

`needed` includes room for everything we want to store, including the terminating '\0'.
Returns 0 on success, -1 on allocation failure or overflow.
*/
static int ensure_capacity(LineReader *lr, size_t needed) {
    if (needed <= lr->cap) return 0;

    // Start with a small initial capacity to avoid tiny allocations. 
    // Initial allocation = 128 bytes
    size_t new_cap = (lr->cap == 0) ? 128 : lr->cap;

    // Grow by doubling until we satisfy `needed`
    while (new_cap < needed) {
        // Growth strategy: double
        size_t next = new_cap * 2;
        if (next < new_cap) return -1; // Overflow guard: if doubling wraps around, it becomes smaller
        new_cap = next;
    }

    // Realloc for new capacity
    char *tmp = (char *)realloc(lr->buf, new_cap);
    if (!tmp) return -1;

    lr->buf = tmp;
    lr->cap = new_cap;
    return 0;
}

int line_reader_init(LineReader *lr, FILE *fp) {
    if (!lr || !fp) return -1;

    // Initialize to a known state so destroy() is always safe.
    lr->fp = fp;
    lr->buf = NULL;
    lr->len = 0;
    lr->cap = 0;
    lr->saw_oef = 0;

    // Allocate an initial buffer once; avoid first-call realloc churn.
    if (ensure_capacity(lr, 128) != 0) {
        line_reader_destroy(lr);
        return -1;
    }

    // Initialize buffer with '\0'
    lr->buf[0] = '\0';
    return 0;
}

void line_reader_destroy(LineReader *lr) {
    if (!lr) return;

    // Freeing NULL is safe; keeping the function idempotent is useful.
    free(lr->buf);
    lr->buf = NULL;
    lr->len = 0;
    lr->cap = 0;

    // We doo not own `fp`, se we do not `fclose()`.
    lr->fp = NULL;
    lr->saw_oef = 0;
}

int line_reader_next(LineReader *lr, const char **out_line, size_t *out_len) {
    if (!lr || !out_line) return -1;

    // Default outputs for non-success cases.
    *out_line = NULL;
    if (out_len) *out_len = 0;

    // If we have already seen EOF previously, behave consistently.
    if (lr->saw_oef) {
        return 1; // already at EOF
    }

    // Reset current line length.
    lr->len = 0;

    // Ensure we have at least a minimal buffer of 128 bytes.
    if (lr->cap == 0 || lr->buf == NULL) {
        if (ensure_capacity(lr, 128) != 0) return -1;
    }

    /*
    Read characters until:
    - '\n' is encountered (end of line), OR
    - EOF is encountered.

    Important behavior:
    - If EOF occurs AFTER reading some characters, we return the last line.
      This supports files that do not end with a trailing newline.
    - If EOF occurs with NO characters read, that's a normal EOF return.
    */
    for (;;) { // Infinite loop
        int ch = fgetc(lr->fp);

        if (ch == EOF) {
            // EOF can mean either "end of file" OR an I/O error.
            if (ferror(lr->fp)) {
                return -1; // I/O error
            }

            lr->saw_oef = 1;

            if (lr->len == 0) {
                // True EOF with no buffered characters => no more lines.
                return 1;
            }

            // EOF but we have characters buffered => return final line.
            break;
        }

        // Ensure space for this chararcter + terminating '\0' 
        if (ensure_capacity(lr, lr->len + 2) != 0) {
            return -1;
        }

        if (ch == '\n') {
            // End of line marker; we do not store '\n'
            break;
        }

        // Store character and continue.
        lr->buf[lr->len++] = (char)ch;
    }

    // Strip Windows-style CRLF: if line ends with '\r', remove it.
    if (lr->len > 0 && lr->buf[lr->len - 1] == '\r') {
        lr->len--;
    }

    // Always NUL-terminate so the retourned pointer is a valid C string.
    lr->buf[lr->len] = '\0';

    // Return borrowed view into internal buffer.
    *out_line = lr->buf;
    if (out_len) *out_len = lr->len;

    return 0;
}

/*
Note:
line        → points to string (address)
*line       → first character
&line       → pointer to the pointer (address of address)
*/