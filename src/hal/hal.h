#pragma once

#include <cstdint>

namespace hal {
    void init(int width, int height);
    void shutdown();
    void present(const uint32_t* framebuffer);
    bool quitRequested();

    enum class Key { None, Char, Enter, Backspace, Escape, Tab, Left, Right, Up, Down };
    struct KeyEvent { Key key; char ch; };
    bool pollKey(KeyEvent& out);
    float getDeltaTime();
}