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
- [x] 6. Common array bugs (and how to debug them)
- [x] 7. Const correctness (intro)

---

### Module C – Heap & Ownership

**Goals:** Learn dynamic memory safely and deliberately.

**Estimated time:** **4-5 days**

**Lessons**
- [x] 1. Dynamic Memory Basics – The Heap
- [x] 2. `malloc` – Allocating Memory
- [x] 3. Ownership and Responsability
- [x] 4. `free` – Releasing Memory
- [x] 5. `Use-After-Free` & Dangling Pointers
- [x] 6. Memory Leaks
- [x] 7. Dynamic Arrays (Heap-Allocated Arrays)
- [x] 8. `realloc` – Resizing Allocations
- [x] 9. `calloc` – Zero-Initialized Memory
- [x] 10. Common Dynamic Memory Bugs (and How to Debug Them)
- [x] 11. Const + Dynamic Memory (Read-only Ownership)

---

### Module D – Structs, APIs, and Multi-file C

**Goals:** Build real C programs, not just examples.

**Estimated time:** **4-6 days**

**Lessons**
- [x] 1. `struct` as data layout
- [x] 2. Passing structs (by value vs pointer)
- [x] 3. Initialization and destruction patterns
- [x] 4. Headers (.h) and implementation (.c)
- [x] 5. Compilation units
- [x] 6. Include guards
- [x] 7. Project structure
- [x] 8. Error handling patterns

---

### Module E – Tooling, Discipline, and Defensive C

**Goals:** Write C like a professional.

**Estimated time:** **3-4 days**

**Lessons**
- [x] 1. Compiler warnings as a design tool
- [x] 2. UB patterns and how to avoid them
- [x] 3. Assertions and contracts
- [x] 4. Debug vs release builds
- [x] 5. Performance awareness (intro)
- [x] 6. Reading other people's C code

---

### Optional Path – C Systems Programming (Future)

**Goal:**  Deepen low-level systems skills in C. This path is optional and can be completed later once C++ development goals are underway.

**Estimated time:** **3–4 weeks** (not required before C++)

**Topics / Projects**
- Implement a hash table from scratch
- Build a custom memory allocator (arena / pool allocator)
- Implement a small HTTP server
- Write Unix-style CLI utilities
- Implement a JSON parser
- Study high-quality C codebases (`musl`, `Redis`, `SQLite`)

---

## Phase 2 - Modern C++ (Fast Transition)

### Module F – Modern C++ Core

**Goals:** Learn the minimum modern C++ features required to start building real systems and numerical projects.

**Estimated time:** **1–2 weeks**

**Lessons**
- [x] 1. C++ vs C: what changes
- [x] 2. References
- [x] 3. RAII
- [ ] 4. Constructors and destructors
- [ ] 5. `std::vector`
- [ ] 6. `std::string`
- [ ] 7. Range-based loops
- [ ] 8. `std::unique_ptr`
- [ ] 9. Basic templates (intro)

---

### Module G – C++ Project Fundations

**Goal:** Leanr the tooling and architecture used in real C++ + projects.

**Estimated time:** **1 week**

**Lessons**
- CMake fundamentals
- Project structure for C++ libraries and executables
- Separate compilation in C++
- Using third-party libraries
- Debug vs release builds in C++

---

### Module H – C++ Systems & Numerical Projects

**Goal:** Apply C++ to real computational problems relevant to quant / ML systems.

**Estimated time:** **3–4 weeks**

**Projects**
- Rewrite `csvstat` in C++ (RAII + STL)
- Build a matrix / vector numerical library
- Implement a limit-order book simulator
- Implement a small neural network from scratch

---

| Phase | Time |
| ----------------- | ----------------- |
| C Foundations (A–E) | ~6–8 weeks |
| Optional C System Path | ~3–4 weeks |
| Modern C++ Core + Projects | ~5-6 weeks |