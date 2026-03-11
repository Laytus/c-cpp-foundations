
# C Project Learning Notes

These notes document important concepts encountered while building the **csvstat** project.

The goal is to consolidate understanding of pointers, memory layout, CLI arguments, and standard C library functions.

---

## 1. Pointer Concepts

### Basic Pointer Example

Assume we have a pointer-to-pointer:

```c
const char **x;
```


Example memory layout:

| Expression | Type | Address | Value | Description |
|------------|------|--------|-------|-------------|
| `&x` | `const char ***` | (stack address) | `0x3000` | Address of variable `x` |
| `x` | `const char **` | `0x3000` | `0x2000` | Pointer to the first element of a pointer array |
| `x[0]` | `const char *` | `0x2000` | `0x1000` | Pointer to the string "apple" |
| `*x` | `const char *` | `0x2000` | `0x1000` | Dereferencing `x` gives the first pointer |
| `**x` | `char` | `0x1000` | `'a'` | First character of "apple" |

Key idea:

```
x        -> pointer to pointer
*x       -> pointer to char
**x      -> actual char value
```

---

## 2. Pointer Usage in the Project

During CSV parsing we have two main memory regions:

```
Heap (line buffer)
------------------
lr.buf

Stack (parser state)
--------------------
p->scratch
```

Example CSV line:

```
apple,1.5,10
```

After parsing, the commas are replaced with `\0`:

```
apple\0 1.5\0 10\0
```

#### Memory Example

```
lr.buf base address  = 0x1000
p->scratch base      = 0x2000
pointer size         = 8 bytes
```

Stored pointers:

| Field | Pointer stored | Points to |
|------|-----------------|-----------|
| `p->scratch[0]` | `0x1000` | "apple" |
| `p->scratch[1]` | `0x1006` | "1.5" |
| `p->scratch[2]` | `0x100A` | "10" |

#### Pointer Array Layout

Addresses where pointers are stored:

| Expression | Address |
|-----------|---------|
| `&p->scratch[0]` | `0x2000` |
| `&p->scratch[1]` | `0x2008` |
| `&p->scratch[2]` | `0x2010` |

Pointer values stored in the array:

| Expression | Value |
|-----------|------|
| `p->scratch[0]` | `0x1000` |
| `p->scratch[1]` | `0x1006` |
| `p->scratch[2]` | `0x100A` |


Dereferencing once:

```
*(p->scratch[i])
```

returns the **first character of each field**.

Example:

```
*(p->scratch[0]) -> 'a'
*(p->scratch[1]) -> '1'
*(p->scratch[2]) -> '1'
```

Printing as strings (`%s`) gives:

```
apple
1.5
10
```

---

## 3. Standard C Foundations Used

### `strcmp`

`strcmp` means **string compare**.

Header:
```c
#include <string.h>
```

Prototype:

```c
int strcmp(const char *a, const char *b);
```

#### Behavior

Compares two strings character-by-character.

| Return Value | Meaning |
|--------------|--------|
| `0` | Strings are equal |
| `< 0` | `a` is lexicographically smaller |
| `> 0` | `a` is lexicographically greater |

Example:

```
strcmp("price", "price") -> 0
strcmp("apple", "banana") -> negative
strcmp("z", "a") -> positive
```

---

## 4. Standard Streams

Declared in:

```c
#include <stdio.h>
```

C programs have three default streams:

| Stream | Purpose |
|------|---------|
| `stdin` | program input |
| `stdout` | normal output |
| `stderr` | errors and warnings |

Example:

```bash
./csvstat file.csv price > output.txt
```

Redirects only **`stdout`**.

Errors still appear on the terminal.

Redirect errors:

```bash
./csvstat file.csv price 2> errors.txt
```

```
1 -> stdout
2 -> stderr
```

---

## 5. CLI Arguments

The `main` function normally looks like this:

```c
int main(int argc, char **argv)
```

#### `argc`

Number of command line arguments.

#### `argv`

Array of argument strings.

Example command:

```
./csvstat --file example.csv --col price --quiet
```

Then:

```
argc = 6
```

Argument array:

```
argv[0] -> "./csvstat"
argv[1] -> "--file"
argv[2] -> "example.csv"
argv[3] -> "--col"
argv[4] -> "price"
argv[5] -> "--quiet"
```

Internally:

```
argv
 ↓
+-------------------+
| pointer -> "./csvstat"
| pointer -> "--file"
| pointer -> "example.csv"
| pointer -> "--col"
| pointer -> "price"
| pointer -> "--quiet"
| NULL
+-------------------+
```

---

## 6. `strtod`

`strtod` means **string to double**.

Header:

```c
#include <stdlib.h>
```

Prototype:

```c
double strtod(const char *nptr, char **endptr);
```

#### What it does

Converts a string to a floating point number.

The entire string does **not** need to be numeric.

The function:

1. Skips leading whitespace
2. Parses the numeric portion
3. Stops at the first invalid character
4. Returns both the value and the stopping location

#### Example

```c
const char *s = "12.5xyz";
char *end;

double v = strtod(s, &end);
```

Memory layout:

```
s -> '1' '2' '.' '5' 'x' 'y' 'z' '\0'
```

After parsing:

```
v = 12.5
end -> 'x'
```

So:

```
parsed portion = "12.5"
remaining text = "xyz"
```

This allows the program to detect invalid numeric tokens.

---

## 7. Key Lessons from the Project

Important systems programming ideas reinforced during the project:

### Ownership

Every piece of heap memory must have **one clear owner**.

Example:

```
LineReader owns lr.buf
CsvParser owns scratch array
```

### Idempotent Destructors

Destroy functions should be safe to call multiple times.

Example:

```
free(NULL) is safe
```

### In‑Place Parsing

CSV parsing works by modifying the buffer:

```
comma -> '\0'
```

This avoids extra allocations.

### Streaming Algorithms

Statistics are computed **without storing the entire dataset**.

This is essential for large files.

---

## 8. Memory Model of the Program

Understanding how memory is organized at runtime is critical when working in C. The `csvstat` program mainly uses **three memory regions**: the `stack`, the `heap`, and the `file input buffer`.

### High-Level Layout

```
STACK
-----------------------------------
main()

LineReader lr
CsvParser parser
Stats stats

pointers:
    lr.buf ---------> heap buffer
    parser.scratch -> heap pointer array

-----------------------------------
HEAP

lr.buf
-----------------------------------
"apple\0 1.5 \0 10\0"

parser.scratch
-----------------------------------
[ ptr -> "apple" ]
[ ptr -> "1.5"   ]
[ ptr -> "10"    ]

-----------------------------------
FILE INPUT
-----------------------------------
CSV file being read line-by-line.
```

### Stack Objects

These structures live on the **stack**:

```
LineReader lr
CsvParser parser
Stats stats
```

They store program state but **do not contain large buffers directly**.

Instead they store pointers to heap memory.

Example:

```
lr.buf -> dynamically allocated line buffer
parser.scratch -> array of pointers to fields
```

This keeps stack usage small and predictable.

### Heap Allocations

Two important allocations exist in the program.

#### Line Buffer

Owned by `LineReader`.

```
lr.buf
```

This buffer holds the entire CSV line that was just read.

Example after parsing:

```
apple\0 1.5\0 10\0
```

Commas are replaced by '\0' so each field becomes an independent C string.

#### Field Pointer Array

Owned by `CsvParser`.

```
parser.scratch
```

This array contains **pointers into the line buffer**.

Example:

```
parser.scratch[0] -> "apple"
parser.scratch[1] -> "1.5"
parser.scratch[2] -> "10"
```

No new strings are allocated; we only store pointers.

This design is **very memory efficient**.

### Why this design is good

This architecture has several advantages:

#### 1. Minimal Allocations

We only allocate:

- one line buffer
- one pointer array

The program never allocates memory for each field.

#### 2. Cache-Friendly

All parsed data stays inside the same line buffer.

This improves CPU cache locality.

#### 3. Streaming-Friendly

The program processes **one row at a time**.

Memory usage stays constant regardless of file size.

#### 4. Clear Ownership

Each heap allocation has a clear owner:

```
LineReader -> owns lr.buf
CsvParser  -> owns scratch
```

Which allows safe destruction:

```
line_reader_destroy()
csv_parser_destroy()
```
