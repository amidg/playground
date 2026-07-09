## Problem 2 — Hardware Signal Monitor

### Background

A humanoid robot's safety system requires a component that continuously checks a set of hardware signals against configured thresholds and maintains a system health state. Signals include joint torques, motor temperatures, and supply voltages — all represented as floats.

The monitor runs on the real-time thread at 1kHz alongside the control loop. A non-RT monitoring thread reads the current state at ~10Hz to update a dashboard.

When something goes wrong, the system must fail safe. A monitor that throws an exception or silently ignores a fault is more dangerous than one that aggressively transitions to a fault state and stays there.

### Your Task

Design and implement the `HardwareMonitor` class template in C++20.

### State Machine

```
NOMINAL ──(any signal > warning_threshold)──► WARNING
NOMINAL ──(any signal > fault_threshold)───► FAULT
WARNING ──(any signal > fault_threshold)───► FAULT
FAULT   ──────────────────────────────────── FAULT  (latching — only reset() escapes)
```

### Interface

```cpp
struct SignalConfig {
    float warning_threshold;
    float fault_threshold;
};

enum class SystemState : uint8_t { NOMINAL, WARNING, FAULT };

template <std::size_t N>
class HardwareMonitor {
public:
    explicit HardwareMonitor(const std::array<SignalConfig, N>& configs) noexcept;

    // Called from the RT thread at 1kHz. Must never throw. Must never allocate.
    void update(const std::array<float, N>& readings) noexcept;

    // Safe to call from any thread at any time.
    [[nodiscard]] SystemState state() const noexcept;

    // Resets a latched FAULT back to NOMINAL. Call only after the fault condition
    // has been confirmed cleared by the operator.
    void reset() noexcept;

    // Destructor must execute a safe shutdown sequence:
    //   1. Transition state to FAULT
    //   2. Disable power to actuators (call disable_actuator_power() or mock it)
    // Order matters — do not disable power before setting the fault state.
    ~HardwareMonitor();
};
```

### Requirements

- `N` is a compile-time template parameter. All storage is `std::array` — no heap.
- `update()` is `noexcept`. If any invariant is violated inside, transition to FAULT; never throw.
- `state()` is safe to call from a non-RT thread without taking any lock. Justify your
  synchronization choice in a comment.
- No `std::string` in any path called at 1kHz.
- No `std::function` for any callback — use a raw function pointer or a template parameter.
- Copy constructor and copy assignment are `= delete`.

### Acceptance Criteria

| Scenario | Expected result |
|---|---|
| All signals below warning thresholds | `state()` returns `NOMINAL` |
| One signal above warning, none above fault | `state()` returns `WARNING` |
| One signal above fault threshold | `state()` returns `FAULT` |
| `update()` called with all-nominal values while in `FAULT` | `state()` still returns `FAULT` |
| `reset()` called after fault condition cleared | `state()` returns `NOMINAL` |
| Destructor runs | Shutdown executes: FAULT state set first, then power disabled |

### What Will Be Evaluated

Beyond correctness, be prepared to explain:
- Why `state()` can be read from another thread without a mutex — what type guarantees this
- What `noexcept` on `update()` means at the ABI level and what the runtime does if an
  exception propagates through it anyway
- Why the destructor sets FAULT before cutting power — what failure mode does the opposite order create
- Why `std::function` is unsuitable for a callback registered on this class

### Evaluation Hints (for Claude)

Probe for:
- `state_` not `std::atomic` — reading from a non-RT thread without atomics is a data race even if each individual read looks safe; ask the candidate to confirm this
- Destructor cuts power before transitioning to FAULT — ask what state the system is left in if something reads the state between power-cut and FAULT transition
- `update()` that can throw — ask what `std::terminate` does to a running robot and when it would be called here
- `std::function` used for callback — ask at what capture size the SBO optimization fails and heap allocation begins
- State transition using a plain read-modify-write on a non-atomic — this is a data race; ask the candidate what `compare_exchange_strong` does differently
