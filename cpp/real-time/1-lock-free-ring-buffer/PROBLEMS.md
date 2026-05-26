## Problem 1 — SPSC Lock-Free Ring Buffer

### Description
The sensor fusion pipeline publishes IMU frames at 1 kHz from a real-time thread. A non-RT logging thread consumes them. A mutex is forbidden on the RT path. Implement a lock-free SPSC ring buffer that is safe without any locking primitives.

### Core Objective
Implement `SPSCRingBuffer<T, N>` using only `std::atomic` and a power-of-two-sized fixed array. The producer and consumer run on separate threads with no shared mutex.

### Permitted Interfaces / Libraries
- C++20 standard library: `<atomic>`, `<array>`, `<optional>`, `<cstddef>`
- No `std::mutex`, `std::condition_variable`, `std::lock_guard`, or any blocking primitive
- No heap allocation after construction
- Compiler: `g++ -std=c++20 -Wall -Wextra -pthread -fsanitize=thread`

### Requirements
1. Template parameters: element type `T` and capacity `N` (must be a power of two; enforce with `static_assert`).
2. `bool push(const T& item)` — called by producer only. Returns `false` (drops) if the buffer is full. Must never block.
3. `std::optional<T> pop()` — called by consumer only. Returns `std::nullopt` if empty. Must never block.
4. `size_t size() const` — approximate, non-atomic read for diagnostics only.
5. All atomic loads on the consumer side use `memory_order_acquire`; all stores on the producer side use `memory_order_release`. Document why in a code comment on each load/store.
6. The head/tail indices wrap naturally via modulo with the power-of-two mask.

### Acceptance Criteria
- [ ] Compiles cleanly with `-fsanitize=thread`, zero warnings, zero ThreadSanitizer errors.
- [ ] `static_assert` fires when N is not a power of two (e.g., N=3).
- [ ] Single-threaded: push N items, pop N items, verify FIFO order.
- [ ] Single-threaded: push N+1 items into a capacity-N buffer — the (N+1)th push returns `false`.
- [ ] Multithreaded test: producer pushes 100,000 monotonically increasing integers; consumer pops all; verifies no gaps, no duplicates, correct ordering of received items.
- [ ] A `main()` that runs both single-threaded and multithreaded tests and prints PASS/FAIL.

### Evaluation Hints (Claude Code only)
- Run the multithreaded test 10 times; any TSAN hit or ordering violation across runs is a fail.
- Check that `memory_order_relaxed` is **not** used on head/tail loads in push/pop (should be acquire/release).
- Verify `static_assert(N && (N & (N-1)) == 0)` or equivalent is present.
- Grep for `mutex`, `lock`, `condition_variable` — any hit in the implementation is a fail.
- The consumer thread must receive exactly 100,000 items in order.
