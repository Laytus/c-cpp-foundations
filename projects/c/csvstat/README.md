# csvstat

`csvstat` is a small **CSV parsing and streaming statistics CLI tool written in C (C17)**.

The project was built as part of a **C foundations learning track** to practice professional C development patterns including memory discipline, modular design, and defensive programming.

---

# Features

- Streaming statistics computation (no full dataset in memory)
- CSV parser (v1: **no quoted fields**, comma‑separated)
- Strict numeric parsing
- Robust error handling
- Defensive C patterns (sanitizers, assertions, explicit ownership)

Statistics computed:

- `rows_seen`
- `missing_column`
- `numeric_ok`
- `numeric_bad`
- `min`
- `max`
- `mean`
- `sample standard deviation`

---

# Project Structure

```
csvstat/
│
├── app/            # CLI entry point
│   └── main.c
│
├── include/        # Public headers
│   ├── csv.h
│   ├── line_reader.h
│   ├── stats.h
│   ├── csvstat_err.h
│   └── csvstat_assert.h
│
├── src/            # Implementation files
│   ├── csv.c
│   ├── line_reader.c
│   ├── stats.c
│   └── csvstat_err.c
│
├── tests/
│   └── input/      # CSV test files
│
├── build/          # Build artifacts
│
└── Makefile
```

---

# Build

The project uses a **Makefile**.

From `projects/c/csvstat`:

```
make
```

This builds the executable:

```
build/csvstat
```

### Clean build

```
make clean
```

---

# Running the Program

Positional mode:

```
./build/csvstat data.csv price
```

Flag mode:

```
./build/csvstat --file data.csv --col price
```

Quiet mode:

```
./build/csvstat --file data.csv --col price --quiet
```

Help:

```
./build/csvstat --help
```

---

# Running Tests

The project includes smoke tests using the CSV files in `tests/input`.

Run all tests:

```
make test
```

---

# Selecting a Different Main File

The Makefile allows selecting the main file at build time.

Example:

```
make MAIN=main_v4.c
```

This is useful for testing different CLI versions while developing.

---

# Defensive C Practices Used

The project intentionally follows defensive C practices:

- AddressSanitizer / UndefinedBehaviorSanitizer
- Explicit ownership rules
- Idempotent destroy functions
- Assertions for internal invariants
- Structured error codes
- Strict numeric parsing

---

# Limitations (v1)

- No quoted CSV fields
- No escaped commas
- No multi‑line CSV cells

These can be implemented in future versions.

---

# Purpose of the Project

This project was designed to practice:

- modular C program architecture
- dynamic memory management
- CLI program design
- streaming algorithms
- professional build workflow

It serves as a **foundation project before moving to larger C/C++ systems projects**.

---

# License

Educational project.