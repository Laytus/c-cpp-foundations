# Compiler and Debugging Flags

This document explains the compiler and runtime flags used in this repository.
All flags are described independently of an editor or IDE.

The goal is to make the build configuration explicit, understandable, and reproducible.

---

## Language standard

### `-std=c17``
Specifies the C language standard.

- Ensures consistent language rules across compilers
- Avoids reliance on compiler-specific extensions
- Provides a stable and well-defined baseline

Using an explicit standard prevents subtle portability issues.

---

## Warnings and diagnostics


### `-Wall`
Enables a broad set of common warnings.

These warnings highlight:
- suspicious constructs
- likely logic errors
- questionable assumptions

`-Wall` does not mean "all warnings", but it enable the most important ones.

---

### `-Wextra`
Enables additional warnings not covered by `-Wall`.

These often catch:
- unused parameters
- signed/unsigned mismatches
- edge cases in expressions

Together with `-Wall`, this significantly improves code quality.

---

### `-Werror`
Treats all warnings as errors.

- Prevents ignoring warnings
- Forces issues to be fixed immediately
- Encourages disciplined coding habits

In C/C++, warnings frequently indicate real bugs.

---

## Optimization and debug symbols

### `-O0`
Disables compiler optimizations.

- Preserves a direct correspondence between source code and generated instructions
- Makes debugging predictable
- Prevents code reordering, inlining, or removal

Optimized builds may skip lines or change execution flow, which is undesirable during learning and debugging.

---

### `-g`
Generates debug symbols.

- Allows debuggers to map machine code back to source code.
- Enables inspection of variables, stack frames, and function calls

Without debug symbols, meaningful debugging is severely limited.

---

## Runtime sanitizers

Sanitizers are runtime checks inserted by the compiler to detect errors that may not caught at compile time.

---

### `-fsanitize=address`
Enables AddressSanitizer (ASan).

Detects:
- use-after-free
- heap and stack buffer overflows
- out-of-bounds memory access
- double free
- invalid pointer use

Memory bugs often compile successfully but cause crashes or silent corruption at runtime.

---

### `-fsanitize=undefined`
Enables UndefinedBehaviorSanitizer (UBSan).

Detects undefined behavior such as:
- signed integer overflow
- null pointer dereference
- invalid bit shifts
- misaligned memory access
- invalid type punning

Undefined behavior means the C standard imposes **no guarantees** on program behavior.

---

### Why sanitizers are necessary
- The compiler assumes your code follows the language rules
- Many errors depend on runtime data
- Undefined behavior can appear or disappear depending on optimization level

Sanitizers act as a safety net during development.

---

## Summary of the default development configuration

The default build configuration prioritizes:

- correctness
- debuggability
- early error detection

Performance optimizations are intentionally deferred until correctness is guaranteed.

---

## Notes on production builds

In production or benchmarking scenarios, this configuration typically changes:

- enable optimizations (`-O2` or `-O3`)
- disable sanitizers
- keep warnings enabled
- possibly keep debug symbols (for post-mortem analysis)

These decisions are context-dependent and should be made explicitly.