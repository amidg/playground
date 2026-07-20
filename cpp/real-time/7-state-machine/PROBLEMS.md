## Problem 7 - State Machine

### Description
The Foundations team needs a lightweight component that aggregates heterogeneous hardware signals (joint temperatures, bus voltages, loop timing), compares each against a threshold, and maintains a simple FSM with three states: `NOMINAL`, `WARNING`, and `FAULT`. This maps directly to the "State Monitor Design" scenario described in your interview prep.

### Core Objective
Design and implement a template-based `SignalMonitor` that is zero-allocation after construction, uses virtual dispatch for the threshold check strategy, and drives a state machine.

### Permitted Interfaces / Libraries
- C++20 standard library: `<array>`, `<string_view>`, `<variant>`, `<functional>`, `<cstdint>`
- No heap allocation after construction in the hot path (`update()`)
- Compiler: `g++ -std=c++20 -Wall -Wextra -fsanitize=address,undefined`

### Requirements
1. Define `enum class MonitorState : uint8_t { NOMINAL, WARNING, FAULT }`.
2. Define an abstract `IThresholdCheck` interface with `virtual MonitorState check(double value) const noexcept = 0` and a virtual destructor.
3. Provide two concrete implementations:
   - `BandCheck`: `NOMINAL` if `low <= value <= high`, `WARNING` if within 10% outside, `FAULT` otherwise.
   - `MaxCheck`: `NOMINAL` if `value <= nominal_max`, `WARNING` if `value <= fault_max`, `FAULT` otherwise.
4. `SignalMonitor<N>` holds up to N signals. Each signal has a `std::string_view` name, a reference to an `IThresholdCheck`, and the last seen state.
5. `MonitorState update(std::string_view name, double value)` — updates the named signal's state and returns it. If a signal transitions into `FAULT`, the entire monitor's state latches to `FAULT` until `reset()` is called.
6. `MonitorState overall_state() const noexcept` — returns the worst state across all signals.
7. `void reset() noexcept` — clears the fault latch and resets all signals to `NOMINAL`.

### Acceptance Criteria
- [ ] Compiles cleanly, zero warnings, zero sanitizer errors.
- [ ] `BandCheck` and `MaxCheck` return correct states for boundary values (test the exact boundary, one ULP inside, and one ULP outside).
- [ ] `update()` on an unknown signal name returns `FAULT` and latches the monitor.
- [ ] After a `FAULT` latch, calling `update()` with a `NOMINAL` value does not clear the latch.
- [ ] `reset()` correctly clears the latch and allows subsequent nominal updates to return `NOMINAL`.
- [ ] A `main()` with named test cases covering all transitions and boundary conditions.

### Evaluation Hints (Claude Code only)
- Verify latch behavior: push a signal to FAULT, then push a NOMINAL value to a different signal — `overall_state()` must still return `FAULT`.
- Test `reset()` followed by a NOMINAL update — `overall_state()` must return `NOMINAL`.
- Unknown signal name: must return `FAULT` immediately.
- Grep for `new`/`malloc` inside `update()` — prohibited.
- Boundary: for `MaxCheck(nominal_max=100, fault_max=120)`, value=100.0 → NOMINAL, value=100.001 → WARNING, value=120.001 → FAULT.
