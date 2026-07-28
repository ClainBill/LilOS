// THIS IS EXAMPLE CODE FROM GEMINI

// The idea is to create a superclass for apps so anything (like the terminal screen) will create a subclass from this.
// maybe rename to Window or whatever

#pragma once
#include "app.h"
#include <string>

class Terminal : public App {
public:
    std::string appName;
    Terminal(int w, int h);
    ~Terminal() override;

    void update(float deltaTime) override;
    void render() override;
    void handleInput() override;

private:
    // Terminal specific variables (cursor blink timer, text buffer, etc.)
    float cursorTimer;
    int width;
    int height;

    float fpsX = 200;
    float fpsY = 220;
};