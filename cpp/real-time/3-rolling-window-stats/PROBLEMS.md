## Problem 3 — Rolling Window Statistics

### Background

A sensor preprocessing layer needs to smooth noisy float readings from a force-torque sensor
before they are fed into a compliance controller. The smoothing is a rolling window mean and
median over the last N samples. The class is called from a 1kHz processing thread. Memory
allocation after startup is not permitted.

### Your Task

Implement the `RollingStats` class in C++20.

### Interface

```cpp
class RollingStats {
public:
    // Window size is a runtime parameter. All memory is allocated in the constructor.
    // Throws std::invalid_argument if window_size == 0.
    explicit RollingStats(std::size_t window_size);

    // Add a new sample. Evicts the oldest sample when the window is full.
    // Must not allocate memory.
    void update(float value) noexcept;

    // Returns the mean of the current window.
    // Returns std::nullopt if no samples have been added yet.
    [[nodiscard]] std::optional<float> mean() const noexcept;

    // Returns the median of the current window.
    // Returns std::nullopt if no samples have been added yet.
    // Even-count windows: return the average of the two middle values.
    [[nodiscard]] std::optional<float> median() const noexcept;
};
```

### Requirements

- Window size is a **runtime** constructor parameter, not a template parameter. Justify your
  choice of internal container given that `std::vector` pre-allocated at construction is the
  natural consequence.
- After construction, `update()`, `mean()`, and `median()` must never allocate memory.
- `mean()` must be O(1) — maintain a running sum, do not iterate the window each call.
- `median()` is O(N log N) — acceptable. However, you must explicitly address why
  `std::priority_queue` is unsuitable here and what you use instead.
- Both query methods are `const` and `[[nodiscard]]`.
- Thread safety is **not** required. Document this as a precondition comment on the class.

### Acceptance Criteria

| Scenario | Expected result |
|---|---|
| Window=4, push [1, 2, 3, 4] | `mean()` = 2.5, `median()` = 2.5 |
| Push 5.0 into the above | Window becomes [2, 3, 4, 5], `mean()` = 3.5, `median()` = 3.5 |
| No samples added | Both return `std::nullopt` |
| Single sample pushed | `mean()` and `median()` both return that sample |
| Even window size, two middle values differ | `median()` returns their average |
| `window_size == 0` at construction | `std::invalid_argument` thrown |

### What Will Be Evaluated

Beyond correctness, be prepared to explain:
- Why `std::vector` pre-allocated at construction is acceptable here but would not be acceptable
  inside `update()` — what is the difference between those two allocation sites
- What happens to the running sum after 10 million samples on a sensor with values near ±100.0
  — is floating-point drift a practical concern here?
- Why `std::priority_queue` is unsuitable in this context and what you use for median instead
- What the tradeoff is between moving median to a non-RT thread vs. computing it inline

### Evaluation Hints (for Claude)

Probe for:
- `mean()` that iterates the window — O(N) when O(1) is required; ask them to maintain a
  running sum
- `std::priority_queue` proposed for median without flagging heap allocation — `std::priority_
  queue` is backed by `std::vector` and can reallocate; this is a correctness failure in RT
- Partially-filled window not handled — ask what `mean()` returns after only 2 pushes into a
  window of size 8
- No acknowledgment of floating-point drift in the running sum — ask what happens over millions
  of iterations and whether they'd apply Kahan summation
- Thread safety not documented — ask whether this class is safe to call from two threads
  simultaneously; the answer is "no by design," but they must state it
