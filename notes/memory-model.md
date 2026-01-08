# Memory Model – How to Read and Reason About Memory

This document explains how memory works at a low level, independently of C syntax.

---

## 1. Memory is byte-addressable

Modern systems use **byte-addressable memory**.

This means:
- Every address refers to **one byte**
- Addresses count bytes, not variables, not types

So memory looks like:
```
Address → Byte
-------   ----
0x1000 → 1 byte
0x1001 → 1 byte
0x1002 → 1 byte
...
```

Larger objects simply occupy **multiple consecutive bytes**.

---

## 2. Types determine how many bytes are used

Each C *type* has a *size* measured in *bytes*.

Examples (typical values, platform-dependent):
- `char` → 1 byte
- `int` → 4 byte
- `double` → 8 byte

We can always check this using:
```c
sizeof(type)
```

---

## 3. Why array addresses differ by `sizeof(type)`

Given:
```c
int array[4];
```

If:
```c
sizeof(int) == 4
```

Then memory looks like:
```
Address       Element
--------      -------
0x1000  → arr[0] (4 bytes)
0x1004  → arr[1] (4 bytes)
0x1008  → arr[2] (4 bytes)
0x100C  → arr[3] (4 bytes)
```

Each element starts **exactlu after** the previous one.

That is why:
```c
&arr[1] == &arr[0] + 4 bytes
```

---

## 4. Why addresses are written in hexadecimal

Addresses are usually printed in **hexadecimal (base 16)**.

Reasons:
- Hex maps cleanly to binary (4 bits per hex digit)
- Addresses are large numbers; hez is compact
- Debugging and systems tools use hex universally

Example:
```
Decimal:     4096
Hexadecimal: 0x1000
```

---

## 5. Why addresses sometimes "end with letters"

Hexadecimal digits are:
```
0 1 2 3 4 5 6 7 8 9 A B C D E F
```

So and address like:
```
0x16fdff4c
```

Ends with "c" because:
- `c` represents decimal `12`
- it is simply the last hex digit

---

## 6. Address alignment

Many types require **aligment**.

Example:
- An `int` often must start at an address divisable by `4`
- A `double` often must start at an address divisible by `8`

This is way:
- *array elements* appear neatly aligned
- addresses often end in `0`, `4`, `8`, `C`(hex)

This improves performance and is enforced by hardware.

---

## 7. Addresses are not values

An address is:
- not the value stored there
- not meaningful by itself
- only useful for locating memory

Two important rules:
- Equal addresses → same memory location
- Different addresses → different memory locations

We should **never** infer:
- ordering of variables
- lifetime
- ownership

from address values alone.

---

## 8. Addresses are process-local

Addresses:
- belong to a specific process
- change between executions
- are virtual, not physical

So:
- The same variable will not have the same address next run
- Address values cannot be compared across programs

---

## 9. Takeaway
- Memory is byte-addressable
- Types define how many bytes are used
- Arrays occupy contiguous memory
- Address spacing reflects object size
- Hexadecimal is just a representation
- Addresses are locations, not values

---

## 10. Core Definitions


**Byte**
: A **byte** is the smallest addressable unit of memory in a modern computer system, typically consisting of **8 bits**.

Key points:
- Memory is **byte-addressable**: each address refers to one byte.
- Larger objects (like `int`, `double`, `arrays`) occupy **multiple consecutive bytes**.
- Addresses count bytes, not variables or types.

Mental model:
> A byte is a single "slot" in memory that can hold a small piece of data.

**Lifetime**
: **Lifetime** is the period of time during program execution during shich a memory object **exists** and may be legally accessed.

Key points:
- Lifetime answers **"when does this object exist?"**
- Lifetime is determined by:
    - *stack frame* creation/destruction
    - *dynamic allocation and deallocation*
    - *program start and termination*
- An obejct outside its lifetime **must not be accessed**, even if memory still contains data.

Mental model:
> Lifetime is the interval during which memory belongs to a specific object.

**Ownership**
: **Ownership** is the responsability of managing a memory object's lifetime, including deciding **who is allowed to destroy it** and **when**.

Key points:
- Ownership answers **"who is responsible for this memory?"**
- In C:
    - ownership is **implicit**
    - enforced by convention and discipline, not by the language
- Ownership includes:
    - the obligation to **free memory** (if dynamic)
    - ensuring memory is **not freed too early**
    - ensuring memory is **freed exactly once**

Mental model:
> Ownership means being accountable for a piece of memory.

