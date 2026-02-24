
# First C Project Notes

## Pointers

### General Example

| Expression | Type | Address | Evaluates To | Description |
|---------|------|---------|-----------------|-------------|
| `&x` | `const char **` | (N/A) (it's an expression, not a stored variable) | `0x3000` | Address of the variable `x` (pointer to `x`) |
| `x` |  `const char **` | `0x3000` | `0x2000` | Base address of the pointer array |
| `x[0]` | `const char *` | `0x2000` | `0x1000` | First pointer in the pointer array (points to "apple") |
| `*x` | `const char *` | `0x2000` | `0x1000` | Same as `*x` |
| `**x` | `char` | `0x1000` | `'a'` | First character of "apple" (`*x` dereferenced as a `char`) |

### Project Example

Line buffer (lr.buf) contains:
```
apple\0 1.5\0 10\0
```

- `lr.buf` base = `0x1000`
- `p->scratch` base = `0x2000`
- pointer size = `8` bytes

Contents:
- `p->scratch[0]` = `0x1000` → "apple"
- `p->scratch[1]` = `0x1006` → "1.5"
- `p->scratch[2]` = `0x100A` → "10"

#### 1 `p->scratch` (base)
- `p->scratch` = `0x2000`

#### 2 `&p->scratch[i]` (address of each element in the pointer array)

These are the locations *where the pointers are stored*.
- `&p->scratch[0]` = `0x2000`
- `&p->scratch[1]` = `0x2008`
- `&p->scratch[2]` = `0x2010`

#### 3 `p->scratch[i]` (pointer value into `lr.buf`)

These are the pointers stored in the array:
- `p->scratch[0]` = `0x1000`
- `p->scratch[1]` = `0x1006`
- `p->scratch[2]` = `0x100A`

#### 4 `*(p->scratch[i])` (first char of each field)

Now we *dereference each field pointer once* (getting a `char`):
- `*(p->scratch[0])` = `'a'`
- `*(p->scratch[1])` = `'1'`
- `*(p->scratch[2])` = `'1'`

If we went one step further and printed each field as a string (`%s`), we'd get:
- `p->scratch[0] as string` = `"apple"`
- `p->scratch[1] as string` = `"1.5"`
- `p->scratch[2] as string` = `"10"`

---

## General Functions

### What is `strcmp`?

`strcmp` means:
> **string compare**

It is declared in:
```c
#include <string.h>
```

Prototype:
```c
int strcmp(const char *a, const char *b);
```

**What it does**

It compares two C strings **character by character**.

It returns:

| Return Value | Meaning |
|--------------|---------|
| 0 | String are equal |
| <0 | `a` is lexicographically smaller |
| >0 | `a` is lexicographically greater |

**Example**
```c
strcmp("price", "price") → 0
strcmp("apple", "banana") → negative
strcmp("z", "a") → positive
```

### What are `stdout` and `stderr`>

They are **standard output streams**.

Declared in:
```c
#include <stdio.h>
```

They are `FILE *` objects.

**There are three standard streams in C**:

| Stream | Purpose |
|--------|---------|
| `stdin` | Standard input (keyboard) |
| `stdout` | Standard output (normal program output) |
| `stderr` | Standard error (error, warnings) |

**Why separate `stdout` and `stderr`?**

Because in UNIX systems, they can be redirected separately.

Example:
```bash
./csvstat file.csv price > output.txt
```

This redirects **`stdout` only**.

Errors appear on screen because they go to `stderr`.

We can also redirect errors:
```bash
./csvstat file.csv price 2> errors.txt
```

- `1>` → `stdout`
- `2>` → `stderr`

### What are `argc` and `argv`?

**Signature of `main`:**
```c
int main(int argc, char **argv)
```

#### `argc`

*Argument count*
Number of arguments passed to the program

#### `argv`

*Argument vector*
Array of strings (each argument is a string)

**Example**

If we type in terminal:
```bash
./csvstat --file example.csv --col price --quiet
```

Then:

**`argc` = 6**

Because there are 6 tokens:
```
[0] ./csvstat
[1] --file
[2] example.csv
[3] --col
[4] price
[5] --quiet
```

**Memory Representation**

`argv` is basically:
```
argv
 ↓
+------------------+
| pointer to "./csvstat"
| pointer to "--file"
| pointer to "example.csv"
| pointer to "--col"
| pointer to "price"
| pointer to "--quiet"
| NULL
+------------------+
```

### What is `strtod`?

`strtod` means:
> **string to double**

Declared in:
```c
#include <stdlib.h>
```

**Prototype**
```c
double strtod(const char *nptr, char **endptr)
```

**What it does**

It converts a `string` into a `double`.

But it does **not require the whole string to be a numeric**.

It:
1. Skips leading whitespace
2. Parses as much numeric text as possible
3. Stops at the first invalid character
4. Returns:
    - the parsed number
    - and tells where it stopped

**Example Walkthrough**

```c
const char *s = "12.5xyz";
char *end;

double v = strtod(s, &end);
```

Memory view:
```
s →  '1' '2' '.' '5' 'x' 'y' 'z' '\0'
```

After parsing:
```
v = 12.5
end → 'x' 
```