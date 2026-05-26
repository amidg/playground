## Problem 0 - VTables and STL

### Description
This is a written-code exercise, not a runtime program. You will write a single `.cpp` file containing short, self-contained snippets that demonstrate your understanding of how the compiler implements virtual dispatch and template instantiation. Each snippet must be accompanied by a `// EXPLANATION:` comment block answering the specific question.

### Core Objective
Prove you can reason about the machine-level consequences of C++ abstractions — the kind of reasoning that surfaces during the C++ knowledge evaluation described in your interview prep.

### Permitted Interfaces / Libraries
- C++20, no external libraries
- The file must compile: `g++ -std=c++20 -Wall -Wextra -O2 -o dissection dissection.cpp`
- Optionally inspect output with `objdump -d dissection | grep -A20 '<vtable'` and paste findings as comments

### Requirements
Write one labeled section per question below. Each section: compilable snippet + `// EXPLANATION:` block of 3–6 sentences.

**Q1 — vtable layout:**
Define a base class `Base` with two virtual methods and one non-virtual method. Define `Derived` overriding one virtual method and adding a new virtual. Draw (in comments) the vtable layout for both classes. Explain what `vptr` is and when it is written.

**Q2 — virtual call cost:**
Write a micro-benchmark (use `clock_gettime`) comparing 10 million calls through a base-class pointer (virtual) vs. 10 million direct calls on a concrete type. Print the timing delta. In `// EXPLANATION:` describe why the virtual version may be slower and under what conditions the compiler can deviate (devirtualization).

**Q3 — template instantiation bloat:**
Define `template<typename T> T square(T x) { return x * x; }`. Call it with `int`, `double`, and `float`. In `// EXPLANATION:` describe how many binary instantiations exist, where they live, and what the ODR says about them across translation units.

**Q4 — SFINAE / concept guard:**
Write a function template `print_size` that is only enabled for types with a `.size()` method (use either `requires` or `std::enable_if`). Show it compiling for `std::string` and being rejected for `int` (use a `static_assert` or a comment showing the error). Explain in `// EXPLANATION:` why the substitution failure is not an error.

**Q5 — `override` safety:**
Write a base class with `virtual void tick(int ms);`. Write a derived class that accidentally writes `virtual void tick(float ms);` (signature mismatch). Then write a second derived class using `override` on the same mismatch. Show (via comment) that the second version produces a compile error. Explain why `override` is non-negotiable in production robotics code.

### Acceptance Criteria
- [ ] File compiles cleanly with `-Wall -Wextra -O2`.
- [ ] All five sections present with compilable snippets.
- [ ] Each `// EXPLANATION:` block is 3–6 sentences minimum, technically precise.
- [ ] Q2 timing benchmark actually runs and prints two numbers.
- [ ] Q4 demonstrates the concept/SFINAE guard with a commented-out negative example showing the error message.
- [ ] Q5 `override` mismatch is shown in a `#if 0` block with the expected compiler error quoted.

### Evaluation Hints (Claude Code only)
- Compile the file; zero errors and zero warnings required.
- Run the binary; Q2 must print two timing values to stdout.
- Check Q1 vtable comments contain both `Base` and `Derived` layouts with correct slot numbering.
- Check Q5 contains a `#if 0` block with the `override` mismatch and the expected error message.
- Check Q4: `print_size` is called with `std::string` in compilable code and the `int` call is in a commented or `#if 0` block with the error quoted.
