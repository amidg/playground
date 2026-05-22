# PROBLEMS.md — C++ Real-Time Systems Practice Problems

This file set of rules for the problems for this study session. It is read by Claude Code alongside your solution to evaluate correctness, real-time safety, API design, and verbal reasoning.

**Problem Definition:**
- Problems are located in the respective folder in the format <problem number>-<problem name>. Example, when user wants to work on a problem 1, claude will find problem folder that starts with 1

**How to use:**
- Tell Claude: `"I want to work on Problem N"` — Claude will present the task, then wait
- When done: `"I'm done"` or `"Evaluate this"` — Claude runs the full evaluation protocol
- Ask for a hint if stuck: Claude may give a method signature or concept name, not the answer

**Standard (applies to every problem unless overridden):**
- C++17 minimum; C++20 preferred
- Compile with `-Wall -Wextra -Wno-unused-parameter -std=c++20`
- No external libraries beyond the C++ standard library
- Write your solution in a single `.cpp` or `.hpp` file per problem
