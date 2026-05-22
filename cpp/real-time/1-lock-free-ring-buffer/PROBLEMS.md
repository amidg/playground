## Problem 1 — SPSC Lock-Free Ring Buffer

### Background

You are building the inter-thread transport layer for a robot's control stack. A hardware
acquisition thread reads joint states from a servo drive over EtherCAT at 1kHz and needs to
hand those readings to a planning thread running at ~100Hz. The two threads run on different
CPU cores. The acquisition thread has hard real-time constraints and must never block, sleep, or
allocate memory at runtime. The planning thread can tolerate the occasional dropped sample —
what it cannot tolerate is reading a partially-written or corrupted joint state.

### Your Task

Implement a fixed-capacity, lock-free, single-producer single-consumer (SPSC) ring buffer as a
C++ class template.

### Interface

```cpp
template <typename T, std::size_t Capacity>
class SPSCQueue {
public:
    // Returns true if the item was enqueued.
    // Returns false if the buffer is full — never blocks, never allocates.
    bool push(const T& item) noexcept;

    // Returns the oldest item if one is available, std::nullopt otherwise.
    [[nodiscard]] std::optional<T> pop() noexcept;
};
```

### Requirements

- `Capacity` is a non-type template parameter — not a constructor argument. The internal storage
  must be `std::array<T, Capacity>`, not `std::vector`.
- `push` and `pop` contain no mutexes, no condition variables, no system calls.
- Zero heap allocation after the object is constructed.
- Safe to call `push` from one thread and `pop` from a different thread simultaneously — no
  other concurrent access pattern is supported or required.
- You must explicitly handle the full/empty ambiguity. State your approach in a comment.
- The write index and read index must not share a cache line.

### Acceptance Criteria

| Scenario | Expected result |
|---|---|
| Push 5 items into a Capacity=8 queue | All return `true` |
| Push until full, then push one more | Last `push` returns `false`; no crash; no UB |
| `pop` from empty queue | Returns `std::nullopt` |
| Push N items then pop N items | Items returned in FIFO order, values exact |
| Concurrent push and pop under load | No torn values; no lost index updates |

### What Will Be Evaluated

Beyond correctness, be prepared to explain:
- Why `memory_order_acquire` / `memory_order_release` are sufficient here and `seq_cst` is not
  required (or justify if you used `seq_cst`)
- What happens in the cache coherence protocol if both indices share a cache line
- Why `Capacity` must be a power of two if you use bitmask wraparound, and what breaks if it is
  not
- How you resolved the full/empty ambiguity and what the tradeoffs of other approaches are

### Evaluation Hints (for Claude)

Probe for:
- `seq_cst` used without justification — ask what ordering is actually required at each
  operation and why
- `write_idx` and `read_idx` declared adjacent without `alignas(64)` — this is false sharing;
  ask the candidate to describe what happens in the MESI protocol
- Full/empty ambiguity unaddressed — `write == read` means both empty and full in a naive
  implementation; the candidate must state which technique they used
- Modulo for wraparound (`% Capacity`) — valid but slower; if bitmask is used, ask what
  constraint on `Capacity` makes it correct
- `std::mutex` anywhere — ask what happens if the mutex is held exactly when the RT deadline
  fires
