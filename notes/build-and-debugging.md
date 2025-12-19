# Build and Debugging Model

This document explains the build and debugging principles used in this repository.
It focuses on **concepts**, not editor-specific tooling.

---

## Where build outputs go

When compiling a source file, the resulting binary is written to the **build tree**, not next to the source.

The output directory mirrors the source directory structure.

Example:

Source file:
```
c/fundamentals/hello.c
```

Compiled binary:
```
build/c/fundamentals/hello
```

### Rule
> The **source tree** contains only human-written code.
> The **build tree** contains only generated artifacts.

This separation keeps the repository clean, avoids collisions, and allows safe deletion of all build outputs at any time.

---

## Summary 

- Source code and build artifacts are strictly separated
- Debug builds prioritize clarity over performance
- Sanitizers catch classes of bugs the compiler cannot
- These practices scale directly to large, professional C/C++ projects
