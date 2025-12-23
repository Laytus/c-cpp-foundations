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

## Common

#### Exit
- Quit the debugger:
    - `quit`