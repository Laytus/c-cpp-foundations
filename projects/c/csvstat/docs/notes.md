

| Expression | Type | Address | Evaluates To | Description |
|---------|------|---------|-----------------|-------------|
| `&x` | `const char **` | (N/A) (it's an expression, not a stored variable) | `0x3000` | Address of the variable `x` (pointer to `x`) |
| `x` |  `const char **` | `0x3000` | `0x2000` | Base address of the pointer array |
| `x[0]` | `const char *` | `0x2000` | `0x1000` | First pointer in the pointer array (points to "apple") |
| `*x` | `const char *` | `0x2000` | `0x1000` | Same as `*x` |
| `**x` | `char` | `0x1000` | `'a'` | First character of "apple" (`*x` dereferenced as a `char`) |



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
