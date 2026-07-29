// THIS IS EXAMPLE CODE FROM GEMINI

// The idea is to create a superclass for apps so anything (like the terminal screen) will create a subclass from this.
// maybe rename to Window or whatever

#pragma once
#include "app.h"
#include <string>
#include <vector>


class Terminal : public App {
public:
    std::string appName;
    Terminal(int x, int y);
    ~Terminal() override;

    void update(float deltaTime) override;
    void render() override;
    void handleInput() override;

private:
    // Terminal specific variables (cursor blink timer, text buffer, etc.)
    float cursorTimer;
    float inputTimer = 0;

    int lineCount = 0;
    int startX = 5;
    int startY = 5;

    std::vector<std::string> consoleText;
    int cursorX = startX, cursorY = startY;
    bool showCursor = true;
};