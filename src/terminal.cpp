#include "terminal.h"
#include "graphics.h"
#include "text.h"
#include "app.h"
#include "fonts.h"

#include <iostream>


Terminal::Terminal(int x, int y) : startX(x), startY(y){
    appName = "Terminal";
    consoleText.push_back("");
}

Terminal::~Terminal(){}

void Terminal::update(float deltaTime){
    // Increase cursor timer and toggle show if a second has elapsed
    cursorTimer += deltaTime;
    if (cursorTimer >= 1.0f){
        showCursor = !showCursor;
        cursorTimer = 0;
    }
    
    if (inputTimer > 0.0f) inputTimer += deltaTime;
    if (inputTimer > .15f || (!hal::isKeyPressed(hal::Key::Backspace) && !hal::isKeyPressed(hal::Key::Enter))) inputTimer = 0;
}
void Terminal::render() {
    lineCount = 0;
    // For each line of text
    for (auto line : consoleText){
        if (!line.empty()){
            std::string_view strView = line;
            printString(startX, startY + (lineCount * (TI83Font5x7.height + 1)), strView, TI83Font5x7, 0x00ff2d); //TODO : Make it print the string as a string_view difficult difficult lemon difficult!
        }
        lineCount = std::max(0, lineCount + 1);
    }

    // Display the blinking cursor
    if (showCursor)
        printChar(cursorX, cursorY, 'I', TI83Font5x7);
}

void Terminal::handleInput(){
    // Polls for normal keyboard input
    hal::KeyEvent e;
    while (hal::pollCharEvent(e)){
        // If anything was pressed show the cursor
        cursorTimer = 0.0f;
        showCursor = true;

        consoleText.back() += e.ch;
        increaseCursor(cursorX, cursorY, TI83Font5x7);
    }
    
    // On backspace
    if (hal::isKeyPressed(hal::Key::Backspace)){
        if (inputTimer == 0){
            inputTimer += hal::getDeltaTime();
            showCursor = true;
            cursorTimer = .0f;
            if (consoleText.back().size() == 0){
                consoleText.pop_back();
                cursorY -= TI83Font5x7.height + 1;
                cursorX = consoleText.back().size() * (TI83Font5x7.width + 1);
                lineCount = std::max(0, lineCount - 1);
            }
            else {
                consoleText.back().pop_back();
                decreaseCursor(cursorX, cursorY, TI83Font5x7);
            }
        }
    }
    
    // On enter
    if (hal::isKeyPressed(hal::Key::Enter)){
        if (inputTimer == 0){
            inputTimer += hal::getDeltaTime();
            showCursor = true;
            cursorTimer = .0f;

            consoleText.push_back("");
            cursorX = startX;
            cursorY += TI83Font5x7.height + 1;
        }
    }
}