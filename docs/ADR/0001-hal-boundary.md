# ADR 0001: HAL boundary for the SDL host backend

## Status
Proposed

## Context
`src/main.cpp` is the only file that includes real SDL. `main.h` currently
exports both platform-agnostic state (`WIDTH`, `HEIGHT`, `framebuffer`) and
SDL handle pointers (`SDL_Window*`, `SDL_Renderer*`, `SDL_Texture*`) that
nothing outside `main.cpp` uses — a leftover forward-declaration seam, not a
real boundary. `graphics.cpp` and `text.cpp` already depend only on the
plain framebuffer contract, so they need no changes.

## Decision
Introduce a function-based HAL (no virtual dispatch — only one backend is
ever compiled in at a time, selected by CMake, not at runtime):

    // src/hal/hal.h
    namespace hal {
        void init(int width, int height);
        void shutdown();
        void present(const uint32_t* framebuffer);

        bool quitRequested();

        enum class Key {
            None, Char, Enter, Backspace, Escape, Tab,
            Left, Right, Up, Down
        };

        struct KeyEvent {
            Key key;
            char ch;   // valid ASCII payload when key == Key::Char
        };

        bool pollKey(KeyEvent& out);
    }

- `quitRequested()` reports the window being closed (`SDL_QUIT`) only. It is
  no longer tied to a specific key — see the note on Esc below.
- `pollKey()` drains one pending key event per call and returns `false` once
  the queue is empty, mirroring `SDL_PollEvent`'s pattern so the caller's
  `while (hal::pollKey(ev)) { ... }` loop needs no heap allocation or queue
  type of its own.
- `Key` covers exactly what a line-editing shell needs (text entry, cursor
  movement, submit, delete, cancel) rather than mirroring every SDL keycode.
  A future physical-keypad backend (closer to the TI-83 target) maps its own
  buttons onto this same small enum instead of onto SDL's.
- Printable characters go through `Key::Char` + `ch` rather than getting a
  named `Key` case each — SDL supplies the already-shifted/layout-aware
  character via `SDL_TEXTINPUT` events, so `sdl_mac.cpp` reads those for
  `Key::Char` and `SDL_KEYDOWN` (filtered to the cases above) for everything
  else. This keeps `hal.h` from growing a case per key on the keyboard.
- **Esc is no longer hardcoded to quit.** With real key input, Esc is a
  meaningful shell key (e.g. cancel the current line) and must reach the
  app as `Key::Escape` like any other key, not be intercepted by the HAL.
  Quitting the host window is now `quitRequested()` (X-button/`SDL_QUIT`)
  only; if the app wants Esc-to-quit behavior too, that's a decision made in
  `main.cpp` by reacting to `Key::Escape`, not inside the HAL.
- `platform/sdl_mac.cpp` implements `hal.h` against SDL2: owns
  `SDL_Window*`/`SDL_Renderer*`/`SDL_Texture*` as file-local statics (not
  exported — nothing else needs them), does `SDL_Init`/`SDL_CreateWindow`/
  `SDL_CreateTexture` in `init()`, `SDL_UpdateTexture`+`SDL_RenderPresent` in
  `present()`, and translates the `SDL_PollEvent` loop into `quitRequested()`
  / `pollKey()` — enabling `SDL_StartTextInput()` during `init()` so
  `SDL_TEXTINPUT` events are actually delivered.
- `main.h` shrinks to just `WIDTH`, `HEIGHT`, `extern uint32_t* framebuffer`
  — the SDL forward-declarations are deleted.
- `main.cpp` keeps owning the framebuffer allocation and becomes
  platform-agnostic: `hal::init(...)` → loop of `hal::quitRequested()` +
  draining `hal::pollKey()` / draw via `graphics.h`+`text.h` /
  `hal::present(framebuffer)` → `hal::shutdown()`.
- `CMakeLists.txt` split (already sketched at the bottom of the file):
  `lilos_core` STATIC lib from `src/graphics.cpp`, `src/text.cpp`,
  `src/fonts/*.cpp` — no SDL2 include path — plus an executable from
  `src/main.cpp` + `platform/sdl_mac.cpp` linked against `lilos_core` and
  `SDL2::SDL2`. Because `lilos_core` never sees SDL2's include path, an
  accidental `#include <SDL2/SDL.h>` in core code stops compiling — the
  boundary becomes build-enforced, not just a convention.

## Rejected alternative
An abstract `class Hal` with virtual methods, one subclass per backend.
Rejected because backend selection happens once, at compile time via
CMake's source list — runtime polymorphism would add a vtable and heap
allocation for no actual variability, working against the "be conservative
with the standard library / think embedded" constraint.

## Consequences
- `graphics.cpp`/`text.cpp` are untouched.
- `main.cpp` must stop relying on Esc-quits-the-app; that behavior either
  moves into `main.cpp`'s own key handling or is dropped in favor of
  window-close only. Worth deciding explicitly, not by accident.
- The shell (the next piece of work) consumes `hal::pollKey()` directly —
  no HAL changes should be needed once line editing and command dispatch
  are built on top of `Key`/`KeyEvent`.
- Key repeat (holding a key down) is whatever SDL's OS-level repeat gives
  us for free via repeated `SDL_KEYDOWN`; the HAL does no debouncing or
  timing of its own. If that feels wrong once the shell is in hand, that's
  a follow-up decision, not part of this ADR.
- Framebuffer allocation policy (heap vs. static vs. pool — Open Decision
  #3) is untouched by this ADR; `present()` takes a pointer, so any
  allocation strategy can be swapped later without changing the HAL surface.
- Adding a second backend later (e.g. a fixed-pool embedded target) means
  adding `platform/<name>/*.cpp` implementing the same four functions and
  pointing CMake at it — no changes to `hal.h` or `src/`.

## Suggested implementation order (separate PRs, each independently attemptable)
1. Strip SDL types out of `main.h`; move them into `platform/sdl_mac.cpp` as statics. Confirms nothing outside `main.cpp` actually used them (compile-only change).
2. Write `hal.h` (`init`/`shutdown`/`present`/`quitRequested`), implement it in `platform/sdl_mac.cpp` by moving code out of `main.cpp` unchanged.
3. Rewrite `main.cpp`'s loop to call through `hal::*` instead of SDL directly.
4. Add `Key`/`KeyEvent`/`pollKey()` to `hal.h`; implement in `platform/sdl_mac.cpp` via `SDL_TEXTINPUT` + filtered `SDL_KEYDOWN`. Decide and implement `main.cpp`'s Esc behavior explicitly (see Consequences).
5. Split `CMakeLists.txt` into `lilos_core` + executable targets per the existing comment block.
