# Modules General Structure

## Phase 1 – C Foundations

### Module A - Execution Model, Memory, Pointers (C Core)

**Goal:** Build a correct mental of how C programs run and access memory.

**Estimated time:** **1-2 weeks**

**Lessons**
- [x] 1. Program execution & stack
- [x] 2. Stack frames & function calls
- [x] 3. Lifetime vs scope
- [x] 4. Addresses
- [x] 5. Pointers (conceptual)
- [x] 6. Dereferencing pointers
- [x] 7. Pointer validity & first real UB
- [x] 8. Passing pointers to functions

---

### Module B - Arrays, Pointers Semantics, and C Idioms

**Goals:** Understand how C represents collections and memory layouts.

**Estimated time:** **4-5 days**

**Lessons**
- [x] 1. Arrays as contiguous memory
- [x] 2. Arrays vs pointers (why they're related but not identical)
- [x] 3. Passing arrays to functions
- [x] 4. Off-by-one errors and bounds
- [x] 5. Strings as arrays (`char[]`)
- [ ] 6. Common array bugs (and how to debug them)
- [ ] 7. Const correctness (intro)

---

### Module C – Heap & Ownership

**Goals:** Learn dynamic memory safely and deliberately.

**Estimated time:** **4-5 days**

**Lessons**
- [ ] 1. Stack vs heap (conceptual)
- [ ] 2. `malloc`/`free``
- [ ] 3. Ownership rules
- [ ] 4. Memory leaks
- [ ] 5. Double-free and use-after-free
- [ ] 6. Designing safe APIs in C
- [ ] 7. Debugging heap issues with sanitizers

---

### Module D – Structs, APIs, and Multi-file C

**Goals:** Build real C programs, not just examples.

**Estimated time:** **4-6 days**

**Lessons**
- [ ] 1. `struct` as data layout
- [ ] 2. Passing structs (by value vs pointer)
- [ ] 3. Headers (.h) and implementation (.c)
- [ ] 4. Compilation units
- [ ] 5. Include guards
- [ ] 6. Project structure
- [ ] 7. Error handling patterns

---

### Module E – Tooling, Discipline, and Defensive C

**Goals:** Write C like a professional.

**Estimated time:** **3-4 days**

**Lessons**
- [ ] 1. Compiler warnings as a design tool
- [ ] 2. UB patterns and how to avoid them
- [ ] 3. Assertions and contracts
- [ ] 4. Debug vs release builds
- [ ] 5. Performance awareness (intro)
- [ ] 6. Reading other people's C code

---

## Phase 2 - Modern C++

### Module F – Transition to Modern C++

**Goals:** Move from C to *safe, modern* C++.

**Estimated time:** **5–6 weeks**

**Lessons**
- C++ as "better C"
- References
- RAII
- `std::vector`, `std::string`
- Value semantics
- Ownership via types
- Move semantics (conceptual)
- CMake & real C++ projects

---

| Phase | Time |
| ----------------- | ----------------- |
| C Foundations (A–E) | ~3 weeks |
| Modern C++ | ~5-6 weeks |
| Total core program | ~2-2.5 weeks |