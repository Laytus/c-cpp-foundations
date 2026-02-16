# Inspect Object Files with `nm`

This document describes the workflow for analyzing object files and symbol tables using `nm`.

---

## Step 1 – Compile without Linking

Generate `.o` files using the `-c` flag:

```bash
clang -std=c17 -Wall -Wextra -Werror -O0 -g -fsanitize=address,undefined -c int_vector_main.c -o build/main.o
clang -std=c17 -Wall -Wextra -Werror -O0 -g -fsanitize=address,undefined -c int_vector.c -o build/int_vector.o
```

The `-c` flag tells the compiler:
> Compile only. Do not link.

Each `.c` file becomes on object file.

---

## Step 2 – List All Symbols

```bash
nm build/int_vector.o
```

Shows:
- Defined symbols
- Undefined symbols
- Local/internal symbols
- Compiler-generated symbols

Example:
```bash
                 U ___asan_init
                 U ___asan_report_load8
                 U ___asan_report_store4
                 U ___asan_report_store8
                 U ___asan_shadow_memory_dynamic_address
                 U ___asan_version_mismatch_check_apple_clang_1700
                 U ___ubsan_handle_pointer_overflow
                 U ___ubsan_handle_type_mismatch_v1
0000000000001520 t _asan.module_ctor
                 U _free
                 U _malloc
                 U _realloc
000000000000083c T _vec_destroy
0000000000000000 T _vec_init
00000000000004d4 t _vec_is_valid
0000000000000ef0 T _vec_push
0000000000000b40 T _vec_reserve
0000000000001538 s l_.src
0000000000001560 d l___unnamed_1
0000000000001680 d l___unnamed_10
00000000000016a0 d l___unnamed_11
00000000000016c0 d l___unnamed_12
0000000000001a70 d l___unnamed_13
0000000000001a90 d l___unnamed_14
0000000000001ab0 d l___unnamed_15
0000000000001ad0 d l___unnamed_16
0000000000001af0 d l___unnamed_17
0000000000001b10 d l___unnamed_18
0000000000001b30 d l___unnamed_19
0000000000001580 d l___unnamed_2
0000000000001b50 d l___unnamed_20
00000000000016e0 d l___unnamed_21
0000000000001700 d l___unnamed_22
0000000000001720 d l___unnamed_23
0000000000001740 d l___unnamed_24
0000000000001760 d l___unnamed_25
0000000000001780 d l___unnamed_26
00000000000017a0 d l___unnamed_27
00000000000017c0 d l___unnamed_28
00000000000017e0 d l___unnamed_29
00000000000015a0 d l___unnamed_3
0000000000001800 d l___unnamed_30
0000000000001820 d l___unnamed_31
0000000000001840 d l___unnamed_32
0000000000001860 d l___unnamed_33
0000000000001880 d l___unnamed_34
00000000000018a0 d l___unnamed_35
00000000000018c0 d l___unnamed_36
00000000000018e0 d l___unnamed_37
0000000000001900 d l___unnamed_38
0000000000001920 d l___unnamed_39
00000000000015c0 d l___unnamed_4
0000000000001940 d l___unnamed_40
0000000000001960 d l___unnamed_41
0000000000001980 d l___unnamed_42
00000000000019a0 d l___unnamed_43
00000000000019c0 d l___unnamed_44
00000000000019e0 d l___unnamed_45
0000000000001a00 d l___unnamed_46
0000000000001a20 d l___unnamed_47
0000000000001a40 d l___unnamed_48
0000000000001a50 d l___unnamed_49
00000000000015e0 d l___unnamed_5
0000000000001548 s l___unnamed_50
0000000000001b70 s l___unnamed_51
0000000000001b80 s l___unnamed_52
0000000000001ba8 s l___unnamed_53
0000000000001bc0 s l___unnamed_54
0000000000001be0 s l___unnamed_55
0000000000001c00 s l___unnamed_56
0000000000001600 d l___unnamed_6
0000000000001620 d l___unnamed_7
0000000000001640 d l___unnamed_8
0000000000001660 d l___unnamed_9
0000000000000000 t ltmp0
0000000000001538 s ltmp1
0000000000001548 s ltmp2
0000000000001560 d ltmp3
0000000000001b70 s ltmp4
0000000000001c30 s ltmp5
0000000000002180 s ltmp6
0000000000002240 s ltmp7
```

**How to read this**

The important columns are:
- **Address** (left): where the symbol lives *within that object file section* (often 0-based-ish in `.o`)
- **Letter** (middle): symbol type
- **Name** (right): the symbol name (macOS often prefixes with `_`)

---

## Step 3 – Show Only Global (Exported) Symbols

```bash
nm -g build/int_vector.o
```

Use this to inspect:
- Public API functions
- What the module exports

If we see a function here, it is externally visible.

Example:
```bash
                 U ___asan_init
                 U ___asan_report_load8
                 U ___asan_report_store4
                 U ___asan_report_store8
                 U ___asan_shadow_memory_dynamic_address
                 U ___asan_version_mismatch_check_apple_clang_1700
                 U ___ubsan_handle_pointer_overflow
                 U ___ubsan_handle_type_mismatch_v1
                 U _free
                 U _malloc
                 U _realloc
000000000000083c T _vec_destroy
0000000000000000 T _vec_init
0000000000000ef0 T _vec_push
0000000000000b40 T _vec_reserve
```

---

## Step 4 – Show only Undefined Symbols (Dependencies)

```bash
nm -u build/int_vector.o
```

This shows:
- What this object file depends on
- Which functions must be resolved by the linker

Example:
```bash
___asan_init
___asan_report_load8
___asan_report_store4
___asan_report_store8
___asan_shadow_memory_dynamic_address
___asan_version_mismatch_check_apple_clang_1700
___ubsan_handle_pointer_overflow
___ubsan_handle_type_mismatch_v1
_free
_malloc
_realloc
```

---

## Step 4b – Sort by Address (often easier to scan)

```bash
nm -n build/int_vector.o
```

Example:
```bash
___asan_init
___asan_report_load8
___asan_report_store4
___asan_report_store8
___asan_shadow_memory_dynamic_address
___asan_version_mismatch_check_apple_clang_1700
___ubsan_handle_pointer_overflow
___ubsan_handle_type_mismatch_v1
_free
_malloc
_realloc
(base) heber@Hebers-MacBook-Pro fundamentals % nm -n build/int_vector.o
                 U ___asan_init
                 U ___asan_report_load8
                 U ___asan_report_store4
                 U ___asan_report_store8
                 U ___asan_shadow_memory_dynamic_address
                 U ___asan_version_mismatch_check_apple_clang_1700
                 U ___ubsan_handle_pointer_overflow
                 U ___ubsan_handle_type_mismatch_v1
                 U _free
                 U _malloc
                 U _realloc
0000000000000000 T _vec_init
0000000000000000 t ltmp0
00000000000004d4 t _vec_is_valid
000000000000083c T _vec_destroy
0000000000000b40 T _vec_reserve
0000000000000ef0 T _vec_push
0000000000001520 t _asan.module_ctor
0000000000001538 s l_.src
0000000000001538 s ltmp1
0000000000001548 s l___unnamed_50
0000000000001548 s ltmp2
0000000000001560 d l___unnamed_1
0000000000001560 d ltmp3
0000000000001580 d l___unnamed_2
00000000000015a0 d l___unnamed_3
00000000000015c0 d l___unnamed_4
00000000000015e0 d l___unnamed_5
0000000000001600 d l___unnamed_6
0000000000001620 d l___unnamed_7
0000000000001640 d l___unnamed_8
0000000000001660 d l___unnamed_9
0000000000001680 d l___unnamed_10
00000000000016a0 d l___unnamed_11
00000000000016c0 d l___unnamed_12
00000000000016e0 d l___unnamed_21
0000000000001700 d l___unnamed_22
0000000000001720 d l___unnamed_23
0000000000001740 d l___unnamed_24
0000000000001760 d l___unnamed_25
0000000000001780 d l___unnamed_26
00000000000017a0 d l___unnamed_27
00000000000017c0 d l___unnamed_28
00000000000017e0 d l___unnamed_29
0000000000001800 d l___unnamed_30
0000000000001820 d l___unnamed_31
0000000000001840 d l___unnamed_32
0000000000001860 d l___unnamed_33
0000000000001880 d l___unnamed_34
00000000000018a0 d l___unnamed_35
00000000000018c0 d l___unnamed_36
00000000000018e0 d l___unnamed_37
0000000000001900 d l___unnamed_38
0000000000001920 d l___unnamed_39
0000000000001940 d l___unnamed_40
0000000000001960 d l___unnamed_41
0000000000001980 d l___unnamed_42
00000000000019a0 d l___unnamed_43
00000000000019c0 d l___unnamed_44
00000000000019e0 d l___unnamed_45
0000000000001a00 d l___unnamed_46
0000000000001a20 d l___unnamed_47
0000000000001a40 d l___unnamed_48
0000000000001a50 d l___unnamed_49
0000000000001a70 d l___unnamed_13
0000000000001a90 d l___unnamed_14
0000000000001ab0 d l___unnamed_15
0000000000001ad0 d l___unnamed_16
0000000000001af0 d l___unnamed_17
0000000000001b10 d l___unnamed_18
0000000000001b30 d l___unnamed_19
0000000000001b50 d l___unnamed_20
0000000000001b70 s l___unnamed_51
0000000000001b70 s ltmp4
0000000000001b80 s l___unnamed_52
0000000000001ba8 s l___unnamed_53
0000000000001bc0 s l___unnamed_54
0000000000001be0 s l___unnamed_55
0000000000001c00 s l___unnamed_56
0000000000001c30 s ltmp5
0000000000002180 s ltmp6
0000000000002240 s ltmp7
```

---

## Step 5 – Inspect Consumer Dependencies

```bash
nm -u build/int_vector_main.o
```

This shows:
- What `int_vector_main.c` expects the linker to provide
- Which module APIs it uses

---

## Step 6 – Verify `static` Behavior

Remove `static` from a helper function.

Recompile.

Run:
```bash
nm -g build/int_vector.o
```

If the function appears as `T`, it is now globally visible.

Re-add `static` to restore encapsulation.

---

## Step 7 – Use `nm -m` for more detailed output (macOS)

```bash
nm -m build/int_vector.o
```

Provides Mach-O specific information, including:
- Section names
- Symbol attributes

---

## Step 8 – Understand Linking

Final linking step:
```bash
clang build/main.o build/int_vector.o -o app
```

The linker:
- Matches `U` symbols with `T` symbols
- Produces executable
- Fails if:
    - Undefined reference
    - Multiple definition

---

## Mental Model

```
.c file
   ↓
Preprocessing:
   - expands #include
   - expands system headers
   ↓
Translation Unit (expanded source)
   ↓
Compilation
   ↓
.o file (machine code + symbol table)
   ↓
Linker
   ↓
Executable
```

---

## Practical Debug Strategy

If we get:

**"Undefined reference"**

→ Run `nm -u` on the failing object file.

**"Multiple definition"**

→ Run `nm -g` on all object files and find duplicate `T` symbols.

---

## Key Insight

The object file is:
- **Machine code**
- Plus a **symbol table**

`nm` is the window into that symbol table.