# Console (Home-Screen Shell) — Design

## Goal

A TI-83 home-screen-style console: a blinking cursor, live keyboard typing, Enter
to submit. This doc scopes v1, proposes an architecture, and breaks it into issues.

## Scope for v1 (decided)

- **Echo-only.** No real command dispatch. Enter clears/resets the line — it
  proves the input → buffer → render loop works, nothing more.
- **Replaces the demo.** `main.cpp`'s `drawRect`/`drawCircle`/etc. demo calls are
  deleted; the console becomes the app's only screen.
- **Single line, no scrollback.** No history buffer, no shifting old lines up.
  Just a prompt line and a cursor.
- **`printChar`/`printString` bugs are out of scope here.** They're real
  (`CLAUDE.md` Known Issues) and a typed console will exercise them, but
  fixing them is being handled separately, not as part of this plan.

## Design options

Three ways to hold the console's state (buffer, cursor position, blink timer):

### A. Free functions over hidden module-local state
Mirrors `hal.h`: a `console` namespace, state as anonymous-namespace statics in
`console.cpp`, functions like `console::handleInput(...)`. Consistent with the
one precedent already in the codebase for a "there's only ever one of these"
subsystem.
- Cost: hidden global mutable state. Can't have two consoles, can't reset state
  between test runs without a `console::reset()` escape hatch.

### B. A `Console` class with instance state
`class Console { char buffer_[N]; int length_; ... public: void handleInput(...); };`
Conventional encapsulation; `main.cpp` owns a `Console console;` value.
- Cost: for something that will only ever have one instance on this target, the
  class adds ceremony (constructor, member-access boilerplate) without buying
  real flexibility — same shape as the `virtual Hal` alternative already
  rejected in the HAL ADR, for the same reason.

### C. Explicit state struct, passed by reference — **recommended**
```cpp
struct ConsoleState {
    char buffer[MAX_LINE + 1];
    int  length;
    bool cursorVisible;
    int  blinkCounter;
};

namespace console {
    void handleInput(ConsoleState&, const hal::KeyEvent&);
    void tick(ConsoleState&);
    void render(const ConsoleState&);
}
```
`main.cpp` owns one `ConsoleState` on the stack and passes it in explicitly.
No hidden globals, no class machinery, no heap: `buffer` is a fixed-size array
sized for one home-screen line.
- Why this one: it's the most honest option for what the state actually is — a
  small, fixed-size, single-owner value. Explicit passing makes ownership
  visible at every call site, which is exactly the muscle worth building given
  the project's memory/ownership focus. It's also trivially testable — no
  global to reset between calls.
- Tradeoff vs. A: an extra parameter at every call site. Worth it here.

## Open decision this touches: HAL boundary (CLAUDE.md #4)

A blinking cursor needs a time source. `hal.h` currently has none — only
`init`/`shutdown`/`present`/`quitRequested`/`pollKey`. Two ways to get blink
timing:

1. **Frame-counted** — `console::tick()` increments a counter once per main-loop
   iteration; toggle visibility every *N* ticks. No HAL change. Couples blink
   rate to render rate (correct only as long as the loop runs at a roughly
   fixed frequency).
2. **Wall-clock** — add `hal::getTicksMs()` (or similar) to the HAL surface,
   toggle based on real elapsed time. Correct regardless of frame rate, but
   grows the HAL boundary — which CLAUDE.md flags as the single biggest open
   design question in the project — and deserves its own short ADR when it
   happens, not a side effect of a console feature.

**Recommendation for v1: frame-counted (option 1).** It's honest about being a
stopgap — call it out in code with a comment — and defers the HAL-surface
decision instead of bundling it into this feature by accident. Revisit if/when
frame rate becomes variable (e.g. vsync off, or real hardware without a fixed
refresh loop).

## Other open decisions — not forced

- **Heap policy (#3):** untouched either way; `ConsoleState` as designed here
  has no heap allocation regardless of which option above is picked.
- **Target hardware / display size / exceptions (#1, #2, #5):** irrelevant to
  this feature.

## Issue decomposition

Suggested order; each is one branch, one PR.

1. **Add the console module skeleton**
   `console.h` + fill in `console.cpp` (currently an empty stub) with
   `ConsoleState` and `console::init(ConsoleState&)`. Add to
   `LILOS_SOURCES` in `CMakeLists.txt`.
   *Done when:* builds clean under `-Wall -Wextra -Wpedantic`, `init` zeroes
   the struct, no behavior yet.
   *Teaches:* struct design for embedded state — fixed-size array vs.
   `std::string`, and why the choice matters here.

2. **Handle character and backspace input**
   `console::handleInput` appends `Key::Char` to `buffer` (bounds-checked
   against `MAX_LINE`) and `Key::Backspace` removes the last character.
   *Done when:* a sequence of key events produces the correct buffer contents,
   including at the buffer-full boundary (extra chars are dropped, not
   overflowed).
   *Teaches:* off-by-one and overflow bounds-checking — directly the
   project's priority weak spot, and the same bug class as the existing
   `printChar` overflow issue, just on a buffer instead of a framebuffer.

3. **Handle Enter to reset the line**
   `Key::Enter` clears `buffer`/`length` (echo-only v1 — no dispatch, no
   history).
   *Done when:* pressing Enter clears the visible line and typing resumes from
   empty.
   *Teaches:* designing the seam where real command dispatch will plug in
   later, without building it now (YAGNI) — split from issue 2 because
   "append characters" and "submit the line" are different responsibilities.

4. **Render the line and a blinking cursor**
   `console::tick` advances `blinkCounter`/`cursorVisible`; `console::render`
   draws `buffer` via `printString` and a cursor glyph after the last
   character when `cursorVisible`.
   *Done when:* typing appears live on screen and the cursor blinks at a
   steady, visible rate.
   *Teaches:* frame-based vs. wall-clock timing, and the portability gotcha in
   coupling visual timing to render rate (see HAL section above).

5. **Wire the console into `main.cpp`, replacing the demo**
   Loop becomes: poll keys → `console::handleInput` per event →
   `console::tick` → `console::render` → `hal::present`. Delete the
   `drawRect`/`drawCircle`/`printString` demo calls.
   *Done when:* the console is the only thing on screen; Esc still quits.
   *Teaches:* integrating a subsystem into an existing loop, and deciding what
   state `main.cpp` owns vs. delegates.

Dependencies: 1 → 2 → 3, 1 → 4, then 5 needs 2/3/4 all merged.

## Explicitly deferred (not in this plan)

- Scrollback / line history.
- Real command dispatch or expression evaluation.
- `hal::getTicksMs()` / wall-clock timing.
- `printChar`/`printString` bounds and font-parameter bugs (tracked
  separately).
