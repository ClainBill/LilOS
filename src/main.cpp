#include "main.h"
#include "hal/hal.h"
#include "graphics.h"
#include "fonts.h"
#include "text.h"
#include "app_manager.h"
#include "terminal.h"

#include <cstring>
#include <cstdint>

#include <iostream>


// Constants
const int WIDTH = 320;
const int HEIGHT = 240;

// Declarations


// Initialise global variables
uint32_t* framebuffer = new uint32_t[WIDTH * HEIGHT];


// Main function
int main() {

    // Initialise the window
    hal::init(WIDTH, HEIGHT);
    
    AppManager appManager;
    appManager.openApp(new Terminal(0,0));

    // Setup clear framebuffer
    std::memset(framebuffer, 0, WIDTH * HEIGHT * sizeof(uint32_t));
    
    bool running = true;
    hal::KeyEvent event;

    // Main OS loop
    while (running){
        // Handle inputs!
        hal::updateInput();

        // This is what polling for typing would loook like
        // hal::CharEvent charE;
        // while (hal::pollCharEvent(charE)){
        //     std::cout << charE.ch << std::endl;
        // }

        // Check if we want to quit and then signal app in focus to handle input
        if (hal::isKeyPressed(hal::Key::Escape) || hal::quitRequested()) running = false;
        appManager.handleInput();

        // Clear framebuffer
        std::memset(framebuffer, 0, WIDTH * HEIGHT * sizeof(uint32_t));

        appManager.update(hal::getDeltaTime());
        appManager.render();
        hal::present(framebuffer);
    }

    // Cleanup
    delete[] framebuffer;
    hal::shutdown();
    return 0;
}

// Draw some stuff to screen
// This is now just garbage test graphics for easy copying...
        // drawRect(0, 0, WIDTH - 1, HEIGHT - 1, 0xAAFFFFFF, false);
        // drawRect(100, 50, 120, 70, 0xAA03fc45);
        // drawCircle(200, 50, 30, 0xAA0377fc, true);
        // drawCircle(190, 100, 35, 0xAAFFFFFF);

        // printChar(10,10, 'i', TI83Font5x7);
        // printChar(16,10, '"', TI83Font5x7);

        // drawLine(10,26,150,26, 0xAA0377fc);
        // drawLine(10,35,150,35, 0xAA0377fc);

        // debugDrawCharSheet(10, 10);

        // printString(10, 108, "HELLO WORLD", TI83Font5x7);
        // printString(10, 116, "Hello world", TI83Font5x7);
        
        // printString(300, 124, "WILL THIS MOVE TO THE NEXT LINE??", TI83Font5x7);

        // drawLine(0,9,319,9, 0xAA0377fc);
        // drawLine(0,16,319,16, 0xAA0377fc);