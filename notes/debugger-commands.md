# Debugger Commands (LLDB)

This file tracks the LLDB commands used trhoughout the exercises.

---

## 1 - Single function, local variables

#### Breakpoints
- Set breakpoint by function name:
    - `breakpoint set --name main`

#### Run
- Start execution:
    - `run`

#### Step
- Step over (execute current line, don't enter function calls):
    - `next`

#### Inspect locals
- Show variables in the current frame:
    - `frame variable`

---

## 2 - Stack frames and function calls

#### Backtrace (call stack)
- Show the current call stack:
    - `bt`

#### Switch frames
- Select a frame by index (0 = current/top frame):
    - `frame select <index>`

#### Inspect locals (per frame)
- Show locals for the selected frame:
    - `frame variable`

---

## 3 - Addresses - Observing memory location

#### Print the address of a variable
- Show the memory address where a variable lives:
    - `p &variable`

#### Print a variable's value
- Show the value of a given variable (works also for pointers)
    - `p variable`

---

## 4 - Pointers and Dereferencing

#### Print a pointer's stored address
- Show the address stored inside a pointer:
    - `p pointer`

#### Dereference a pointer
- Show the value stored at the address pointed to by a pointer:
    - `p *pointer`

---

## Common

#### Exit
- Quit the debugger:
    - `quit`