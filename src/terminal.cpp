#include "terminal.h"
#include "graphics.h"
#include "text.h"
#include "app.h"
#include "fonts.h"

// Below are literally only used for printing the fps rn
#include <charconv>  // std::to_chars
#include <string_view>

#include <iostream>


Terminal::Terminal(int w, int h) : width(w), height(h){
    appName = "Terminal";
}

Terminal::~Terminal(){}

void Terminal::update(float deltaTime){
    return;
}
void Terminal::render() {
    char buffer[32] = "FPS: ";
    float dt = hal::getDeltaTime();
    float fps = 1000.0f / dt;

    // Convert int/float directly into the buffer array starting after "FPS: "
    auto [ptr, ec] = std::to_chars(buffer + 5, buffer + sizeof(buffer), static_cast<int>(fps));

    if (ec == std::errc{}) {
        // Construct a string_view using the exact length of the valid character data
        std::string_view fpsView(buffer, ptr - buffer);
        
        printString((int)fpsX, (int)fpsY, fpsView, TI83Font5x7);
    }
}

void Terminal::handleInput(){
    if (hal::isKeyPressed(hal::Key::Up)){
        fpsY -= 10 * hal::getDeltaTime();
    }  if (hal::isKeyPressed(hal::Key::Down)){
        fpsY += 10 * hal::getDeltaTime();
    }  if (hal::isKeyPressed(hal::Key::Left)){
        fpsX -= 10 * hal::getDeltaTime() ;
    }  if (hal::isKeyPressed(hal::Key::Right)){
        fpsX += 10 * hal::getDeltaTime();
    }
}