## Problem 5 — Deadlock Prevention and Lock Ordering

### Background

You are reviewing a production incident report. The robot's control system occasionally freezes under load. Examination of a core dump reveals two threads permanently blocked:

- The **control thread** holds `joint_mutex_` and is waiting to acquire `sensor_mutex_`
- The **sensor thread** holds `sensor_mutex_` and is waiting to acquire `joint_mutex_`

Neither thread will ever make progress. The robot is locked up, holding live torque on joints.

### Your Task

**Part A — Diagnosis (written answer)**

Identify the problem by name. State the four Coffman conditions for deadlock. For each condition, confirm that it is present in this specific scenario.

**Part B — Fix using the standard library**

Fix the deadlock without introducing a single global mutex that replaces both. You may change only how the locks are acquired — not the data structures or the thread logic.

Implement the fix two ways:
1. Using `std::scoped_lock` (C++17)
2. Using `std::lock()` with `std::adopt_lock` and `std::lock_guard`

Show both as standalone code snippets with comments explaining the mechanism.

**Part C — Structural prevention**

Write a RAII wrapper that makes this class of bug impossible to introduce through normal use.

```cpp
// Acquires two mutexes in a consistent global order (lower address first).
// Releases both on destruction.
// Non-copyable. Non-moveable.
class OrderedLock {
public:
    OrderedLock(std::mutex& a, std::mutex& b);
    ~OrderedLock();

    OrderedLock(const OrderedLock&) = delete;
    OrderedLock& operator=(const OrderedLock&) = delete;
    OrderedLock(OrderedLock&&) = delete;
    OrderedLock& operator=(OrderedLock&&) = delete;
};
```

The address-ordering approach means any two threads that use `OrderedLock` on the same pair of mutexes will always acquire them in the same order, making circular wait structurally impossible.

### Acceptance Criteria

| Item | Expected result |
|---|---|
| Part A | All four Coffman conditions named and confirmed present |
| Part B approach 1 | `std::scoped_lock` acquires both atomically; no possible interleaving creates deadlock |
| Part B approach 2 | `std::lock()` + `std::adopt_lock` + `std::lock_guard`; correct ownership transfer |
| Part C | `OrderedLock` acquires in address order; non-copyable; non-moveable; releases in destructor |
| Part C edge case | `&a == &b` handled — must not double-lock the same mutex |

### What Will Be Evaluated

Beyond correctness, be prepared to explain:
- Why reversing lock order in only one thread fixes this instance but is not a scalable solution as the codebase grows
- What `std::adopt_lock` does and why it is necessary in the `std::lock()` approach
- How `OrderedLock` makes circular wait structurally impossible by construction
- The difference between deadlock and priority inversion, and how priority inheritance mutexes address the latter (bonus)

### Evaluation Hints (for Claude)

Probe for:
- Part A answer that just says "deadlock" without naming all four Coffman conditions — push for all four: mutual exclusion, hold-and-wait, no preemption, circular wait
- Part B approach 2 that forgets `std::adopt_lock` — ask what happens to the mutex if the
  `lock_guard` tries to lock it again in its constructor
- `OrderedLock` that is moveable — ask: "if this object is moved after construction, who is
  responsible for releasing the locks, and is that guaranteed?"
- `OrderedLock` that does not handle `&a == &b` — ask what happens when both references alias the same mutex and the constructor tries to lock it twice
