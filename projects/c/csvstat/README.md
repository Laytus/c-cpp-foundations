# csvstat

A small CSV parsers + streaming statistics CLI tool written in C (C17).

## Goals
- Practice professional C project structure (headers/src separation)
- Use defensive C: sanitizers, explicit ownership, robust error handling
- Parse CSV (v1: no quoted fields) and comppute streaming stats

## Build (manual)

From `projects/c/csvstat`:

```bash
clang -std=c17 -Wall -Wextra - Werror -O0 -g -f -sanitize=address, undefined \
  -Iinclude app/main.c \
  -o build/csvstat
```

## Usage (planned)

```bash
./build/csvstat --file data.csv --col price
```