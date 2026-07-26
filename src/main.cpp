#include "main.h"
#include "graphics.h"
#include "fonts.h"
#include "text.h"

#include <SDL2/SDL.h>
#include <cstring>
#include <cstdint>

// Constants
const int WIDTH = 320;
const int HEIGHT = 240;

// Declarations
static int initialiseWindow();

// Initialise global variables
SDL_Window* window;
SDL_Renderer* renderer;
SDL_Texture* texture;
uint32_t* framebuffer = new uint32_t[WIDTH * HEIGHT];

// Main function
int main() {
    // Initialise the window
    if (initialiseWindow())
        return 1;

    // Setup clear framebuffer
    std::memset(framebuffer, 0, WIDTH * HEIGHT * sizeof(uint32_t));

    // Draw some stuff to screen
    drawRect(0, 0, WIDTH - 1, HEIGHT - 1, 0xAAFFFFFF, false);
    drawRect(100, 50, 120, 70, 0xAA03fc45);
    drawCircle(200, 50, 30, 0xAA0377fc, true);
    drawCircle(190, 100, 35, 0xAAFFFFFF);

    printChar(10,10, 'i', TI83Font5x7);
    printChar(16,10, '"', TI83Font5x7);

    drawLine(10,26,150,26, 0xAA0377fc);
    drawLine(10,35,150,35, 0xAA0377fc);

    debugDrawCharSheet(10, 10);

    printString(10, 108, "HELLO WORLD", TI83Font5x7);
    printString(10, 116, "Hello world", TI83Font5x7);
    
    printString(300, 124, "WILL THIS MOVE TO THE NEXT LINE??", TI83Font5x7);

    drawLine(0,9,319,9, 0xAA0377fc);
    drawLine(0,16,319,16, 0xAA0377fc);
    
    // main app loop
    bool running = true;
    SDL_Event event;

    while (running){
        // Check for inputs to escape
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
            if (event.type == SDL_KEYDOWN &&
                event.key.keysym.sym == SDLK_ESCAPE) running = false;;
        }

        // TODO make this use faster update method. SDL_UpdateTexture is apparently slow for updates every frame
        SDL_UpdateTexture(texture, nullptr, framebuffer, WIDTH * sizeof(uint32_t)); 

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
    }

    // Cleanup
    delete[] framebuffer;
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

// Initialise the window and SDL objects
int initialiseWindow() {
    // Check if could initialise a video
    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }

    // Initialise window
    window = SDL_CreateWindow(
        "Main Window",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WIDTH, HEIGHT,
        SDL_WINDOW_SHOWN
    );
    if (!window) {
        SDL_Log("CreateWindow failed: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Initialise renderer
    renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED
    );
    if (!renderer) {
        SDL_Log("CreateRenderer failed: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Initialise texture
    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        WIDTH, HEIGHT
    );
    return 0;
}