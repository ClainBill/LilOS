#pragma once

#include <cstdint>

namespace hal {
    void init(int width, int height);
    void shutdown();
    void present(const uint32_t* framebuffer);
    bool quitRequested();

    enum class Key {
        Undefined, Escape, Tab, Enter, Backspace, Left, Right, Up, Down,
        Shift, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
        Comma, Period
    };
    struct KeyEvent { Key key; char ch; };

    void updateInput();
    bool isKeyPressed(Key key);

    // bool pollKey(KeyEvent& out);
    struct CharEvent { char ch; };
    bool pollCharEvent(CharEvent& out);  // For text editors
    float getDeltaTime();
}