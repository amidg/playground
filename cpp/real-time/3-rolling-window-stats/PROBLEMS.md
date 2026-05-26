## Problem 3 — Rolling Window Statistics

### Description
You are writing a diagnostic utility for a 1 kHz IMU stream. Downstream components need a running median over the last N samples to filter impulse noise. The utility must work correctly on a live stream where samples arrive one at a time.

### Core Objective
Implement a `RollingMedian<T, N>` class that maintains a sliding window of the last N values and returns the median in O(log N) time per insertion.

### Permitted Interfaces / Libraries
- C++20 standard library only (`<array>`, `<algorithm>`, `<optional>`, `<stdexcept>`)
- No heap allocation after construction (no `std::vector`, `std::deque`, `std::map`, `std::set`)
- Compiler: `g++ -std=c++20 -Wall -Wextra -fsanitize=address,undefined`

### Requirements
1. `RollingMedian<double, 64>` must be stack-allocatable with zero dynamic allocation after construction.
2. `push(T value)` adds a sample, evicting the oldest when the window is full.
3. `median()` returns `std::optional<T>`: `std::nullopt` when the window is empty, the median value otherwise.
4. For even window sizes, return the lower of the two middle elements (avoids floating-point averaging on integer types).
5. The class must be non-copyable (deleting copy constructor/assignment is sufficient).
6. All public methods must be marked `noexcept` where provably safe.

### Acceptance Criteria
- [ ] Compiles cleanly with `-fsanitize=address,undefined`, zero warnings.
- [ ] Correct median for a window of 1 element.
- [ ] Correct median for odd and even window fill levels.
- [ ] Eviction of oldest element is verified: after N+1 pushes, the first element is no longer reflected in the median.
- [ ] `median()` returns `std::nullopt` on an empty instance.
- [ ] No `new`, `malloc`, `std::vector`, or `std::deque` anywhere in the implementation.
- [ ] A self-contained `main()` with at least 5 deterministic test cases that print PASS/FAIL to stdout.

### Evaluation Hints (Claude Code only)
- Grep source for `new`, `malloc`, `vector`, `deque` — any hit in the implementation (not tests) is a fail.
- Run with `-fsanitize=address,undefined`; any sanitizer error is a fail.
- Check: push exactly N+1 distinct values where value[0] is an outlier; median must not reflect value[0].
- Check: window size 1 — median equals the single pushed value.
- Check: window size 4, push [1,3,2,4] → median must be 2 (lower-middle rule).
