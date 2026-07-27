#include "hal/hal.h"
#include <SDL2/SDL.h>

// REMEMBER keep the HAL to things that the SDL requires specific to macos

// Initialise SDL variables
namespace {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;

    // initialise SDL event
    SDL_Event event;
    int SDLwidth;
    bool quit = false;
}


// Render the framebuffer to SDL window
void hal::present(const uint32_t* framebuffer){
        // TODO make this use faster update method. SDL_UpdateTexture is apparently slow for updates every frame
        SDL_UpdateTexture(texture, nullptr, framebuffer, SDLwidth * sizeof(uint32_t)); 
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
}


bool hal::pollKey(KeyEvent& out){
// Check for inputs to escape
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) quit = true;
        if (e.type == SDL_TEXTINPUT) {
            out = {Key::Char, e.text.text[0]};
            return true;
        }
        if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
                case SDLK_RETURN:    out = {Key::Enter, 0};     return true;
                case SDLK_BACKSPACE: out = {Key::Backspace, 0}; return true;
                case SDLK_ESCAPE:    out = {Key::Escape, 0};    return true;
                case SDLK_LEFT:      out = {Key::Left, 0};      return true;
                // ...
            }
        }
    }
    return false;
}

bool hal::quitRequested(){
    return quit;
}


// create a cleanupSDL() void()
void hal::shutdown(){
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}


// Initialise the window and SDL objects
void hal::init(int width, int height) {
    SDLwidth = width;

    // Check if could initialise a video
    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
    }

    // Initialise window
    window = SDL_CreateWindow(
        "Lil'OS",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height,
        SDL_WINDOW_SHOWN
    );
    if (!window) {
        SDL_Log("CreateWindow failed: %s", SDL_GetError());
        SDL_Quit();
    }

    // Initialise renderer
    renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED
    );
    if (!renderer) {
        SDL_Log("CreateRenderer failed: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    // Initialise texture
    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        width, height
    );

}