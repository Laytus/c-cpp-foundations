# C++ Fundamentals – Core Concepts for Modern Development

## 🎯 Goal

Understand the fundamental building blocks used in modern C++ projects:
- Compilation safety
- Code organization
- Type abstraction
- Domain modeling

---

# 1. Why C++17?

## ✅ Why use C++17

C++17 provides the best balance between:
- Modern features
- Stability
- Toolchain compatibility

### Key features available in C++17
- std::optional
- STL improvements
- Structured bindings
- Smart pointers (std::unique_ptr)

## ❌ Why not older standards (C++11/14)

They lack:
- std::optional
- cleaner STL usage
- modern conveniences

## ❌ Why not newer (C++20/23) yet

Although powerful, they introduce:
- Concepts
- Ranges
- Coroutines

These add complexity and are not necessary for:
- first real systems project
- learning core C++ design

## 🧠 Takeaway

Use C++17 to learn modern C++ without overwhelming complexity.

---

# 2. What is `lob`?

lob stands for Limit Order Book.

It represents the domain of the project.

## 🧠 Takeaway

`lob` defines the domain boundary of your system.

---

# 3. #pragma once

## Problem

Including the same file multiple times causes errors.

## Solution

#pragma once

This tells the compiler to include the file only once.

## 🧠 Takeaway

Prevents duplicate definitions.

---

# 4. Namespaces

## Problem

Name collisions in large codebases.

## Solution

namespace lob {
    struct Order {};
}

Usage:
lob::Order order;

## 🧠 Takeaway

Namespace = logical container for code.

---

# 5. using (Type Aliases)

## Example

using OrderId = std::int64_t;

## Benefits

- Readability
- Flexibility

## 🧠 Takeaway

Improves clarity and maintainability.

---

# 6. enum class

## Example

enum class Side {
    Buy,
    Sell
};

## Benefits

- Scoped
- Type-safe

## 🧠 Takeaway

Safe, scoped set of values.

---

# 🚀 Final Insight

Good C++ is about modeling systems clearly and safely.
