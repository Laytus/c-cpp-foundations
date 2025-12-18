# c-cpp-foundations

A personal repository to build strong engineering foundations in **C** and **modern C++**:
- memory model, debugging, tooling, build systems
- API design, project structure, professional workflows
- general-purpose focus

## Repository structure
- `c/`- C exercises and examples, organized by topic
- `cpp/` - modern C++ exercises and exmples, organized by topic
- `projects/` - larger, end-to-end projects (C and C++)
- `notes/` - technical notes (memory, compilation, debugging, etc.)
- `tools/` - tooling decisions and utilities

## Quick compile (C)

```bash
clang -std=c17 -Wall -Wextra -Werror -O0 -g hello.c -o hello
./hello
```

## Debug (LLDB)

```bash
lldb ./hello
(lldb) breakpoint set --name main
(lldb) run
```

## Sanitizers (macOS)

```bash
clang -std=c17 -Wall -Wextra -Werror -O0 -g \
    -fsanitize=address,undefined test.c -o test
./test
```