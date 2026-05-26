## Problem 6 - RAII

### Description
You are integrating a hypothetical motor controller SDK. The SDK exposes raw C handles that must be opened, configured, enabled, and then shut down in strict reverse order. Write a C++ RAII wrapper that guarantees correct teardown even on exceptions, matching the pattern you described for the AgiBot integration.

### Core Objective
Demonstrate mastery of RAII, move semantics, and exception-safe resource management with a non-copyable, movable handle type.

### Permitted Interfaces / Libraries
- C++20 standard library only
- Simulated SDK: define a `MockMotorSDK` namespace with stub free functions (provided below in the starter scaffold) — do not alter the stub signatures
- Compiler: `g++ -std=c++20 -Wall -Wextra -fsanitize=address`

**Starter scaffold — MockMotorSDK stubs (copy verbatim into your file):**
```cpp
namespace MockMotorSDK {
    // Returns handle id >= 0 on success, -1 on failure
    int  open_controller(int axis_id);
    // Must be called before enable
    bool configure(int handle, double max_torque_nm);
    // Begins torque output
    bool enable(int handle);
    // Stops torque output — must be called before close if enabled
    void disable(int handle);
    // Releases handle — must be called before process exits
    void close_controller(int handle);
}
```
Implement the stubs with simple `printf` logging and a global `std::set<int>` tracking open handles. The destructor/test will verify the shutdown sequence via stdout output order.

### Requirements
1. Class `MotorController` wraps a single SDK handle.
2. Constructor opens and configures the controller; throws `std::runtime_error` if `open_controller` returns -1 or `configure` returns false.
3. `enable()` enables the controller; throws if already enabled or if the SDK call fails.
4. Destructor: if enabled, calls `disable()` first, then always calls `close_controller()`. Must be `noexcept`.
5. `MotorController` is non-copyable. It is movable: moved-from object must be in a valid-but-inert state (destructor must be a no-op on a moved-from instance).
6. A `bool is_enabled() const noexcept` accessor.

### Acceptance Criteria
- [ ] Compiles cleanly with `-fsanitize=address`, zero warnings.
- [ ] Normal path: construct → enable → destruct produces logs in order: `open → configure → enable → disable → close`.
- [ ] Exception during construction: no `close` or `disable` is called for the failed instance.
- [ ] Move: after `MotorController b = std::move(a)`, destructing `a` is a no-op (no double-close).
- [ ] ASAN reports zero leaks and zero errors.
- [ ] A `main()` with four named test cases (normal, throw-on-open, throw-on-enable, move semantics) each printing PASS/FAIL.

### Evaluation Hints (Claude Code only)
- Parse stdout log lines; verify `disable` always precedes `close` when enable was called.
- Verify `close` is NOT called when constructor throws before `open_controller` succeeds.
- Run move test: after move, original object's destructor must not emit any log lines.
- ASAN leak check: `ASAN_OPTIONS=detect_leaks=1` — zero leaks.
- Grep for raw `delete` or explicit `close_controller` calls outside the destructor — should not exist.
