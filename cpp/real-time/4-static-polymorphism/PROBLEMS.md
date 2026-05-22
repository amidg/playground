## Problem 4 — Virtual Dispatch vs. Static Polymorphism

### Background

You are designing the sensor abstraction layer for a robot. The system supports three sensor
types: IMU, force-torque sensor, and joint encoder. Each exposes a single `read()` method
returning a float. The control loop calls `read()` on each sensor at 1kHz.

A junior engineer on the team wrote the initial design using a virtual base class. Your job is
to implement that design correctly, then implement an alternative with no runtime dispatch, and
then explain the tradeoffs to a teammate unfamiliar with the performance implications.

### Your Task

**Part A — Runtime polymorphism**

Implement a virtual base class `Sensor` with a single pure virtual method:

```cpp
[[nodiscard]] virtual float read() const = 0;
```

Implement two concrete types: `IMUSensor` and `EncoderSensor`. Each returns a hardcoded float
for now. Use `override` on every overriding method. Ensure the class hierarchy is correct with
respect to ownership and destruction.

**Part B — Static polymorphism**

Implement the same two sensor types using CRTP (Curiously Recurring Template Pattern). The
`read()` call must resolve at compile time. The `virtual` keyword must not appear anywhere in
your Part B code.

```cpp
template <typename Derived>
class SensorBase { /* ... */ };

class IMUSensor : public SensorBase<IMUSensor> { /* ... */ };
```

**Part C — Verbal explanation (write your answers as comments or a short paragraph)**

Answer each question. Answer before reading the next one.

1. A virtual function call costs "one extra pointer dereference." That framing is incomplete —
   what else does it cost, and why does that cost matter specifically at 1kHz?

2. Where in memory is the vtable stored? Where in an object instance is the vptr stored? How
   many bytes does a vptr occupy on a 64-bit system?

3. Describe a realistic scenario in a robotics system where you would choose virtual dispatch
   despite the overhead.

4. What does marking a class `final` allow the compiler to do that it cannot do otherwise?

### Acceptance Criteria

| Item | Expected result |
|---|---|
| Part A | Compiles; `override` on all overrides; virtual destructor on base; no slicing |
| Part B | Compiles; no `virtual` anywhere; `read()` resolves at compile time |
| Part C Q1 | Mentions branch prediction failure and instruction cache miss, not just indirection |
| Part C Q2 | vtable in read-only data segment; vptr in object at offset 0; 8 bytes on 64-bit |
| Part C Q3 | A concrete scenario (e.g. plugin system, non-hot-path configuration) |
| Part C Q4 | Compiler can devirtualize — convert virtual call to direct call or inline |

### What Will Be Evaluated

The virtual destructor question is a common failure point — missing it causes undefined behavior
when a derived object is deleted through a base pointer. Be ready to explain both what goes
wrong and why the language does not make this a compile error.

### Evaluation Hints (for Claude)

Probe for:
- Missing virtual destructor in Part A — ask: "what is the behavior defined by the standard if
  I call `delete ptr` where `ptr` is a `Sensor*` pointing to an `IMUSensor` and `~Sensor` is
  not virtual?"
- CRTP that still has `virtual` somewhere — the template dispatch mechanism is the replacement,
  not a supplement
- Part C Q1 answered as just "one indirection" — push for: data-dependent branch target defeats
  the branch predictor; the target function body may not be in the instruction cache
