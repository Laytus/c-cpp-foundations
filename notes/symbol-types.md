# Symbol Types in C (Object Files & Linking)

This document explains the meaning of symbol types as shown by tools like `nm` on macOS (Mach-O).

Symbols represent name entities (functions, variables) stored in object files (`.o`) and used by the linker.

---

## What is a Symbol?

A **symbol** is:
- A named entity
- Stored in an object file's symbol table
- Used by the linker to resolve references across translation units

Symbols can represent:
- Functions
- Global variables
- Static (internal) functions
- Data sections
- External dependencies

---

## Common `nm` Symbol Letters (macOS / Mach-O)

| Letter | Meaning | Visibility | Section | Notes |
|--------|----------|------------|---------|-------|
| `T` | Defined global function | External | Text | Exported function |
| `t` | Defined local function | Internal | Text | Often `static` |
| `U` | Undefined symbol | External | N/A | Must be resolved at link time |
| `D` | Initialized global variable | External | Data | Global data |
| `d` | Initialized local/static variable | Internal | Data | File-scoped static |
| `B` | Uninitialized global variable | External | BSS | Zero-initialized globals |
| `b` | Uninitialized local/static variable | Internal | BSS | Static file-scope |
| `R` | Read-only global data | External | Read-only | e.g., const globals |
| `r` | Read-only local data | Internal | Read-only | e.g., static const |
| `S` / `s` | Symbols in other sections | Varies | Section-specific | Often compiler-generated |


---

## Uppercase vs Lowecase

- Uppercase → Global (externally visible)
- Lowercase → Local (internal linkage, often `static`)

Example:
```
T_vec_init       →  Exported function
t_vec_is_valid   →  Internal static function
U_malloc         →  Dependency onexternal symbol
```

---

## What does `U` (Undefined) mean?

If a symbol appears as `U`, it means:
- This object file uses it

- It is *not defined* in this object file
- The linker must resolve it from:
    - Another `.o` file
    - A `static` library
    - A shared library (e.g., `libc`)

Example:
```
U_printf
U_malloc
```

---

## What is stored in the Symbol Table?

Each symbol entry typically contains:
- Name
- Type (text, data, undefined, etc.)
- Visibility (global vs local)
- Address/offset within section
- Section information

The symbol table is used by:
- The linker
- Debuggers
- Profilers
- Static analysis tools

---

## Why Symbol Visibility matters

- `static` functions → appear as lowercase `t`
- Non-`static` functions → appear as uppercase `T`
- Only exported symbols (`T`) are linkable from other translation units

Encapsulation in C is enforced at the symbol level via linkage.

---

## Key Concepts

- **Translation Unit** → produces one object file 
- **Object File** → contains machine code + symbols table
- **Linker** → resolves undefined symbols using defined ones
- **`static`** → gives internal linkage (local symbol)