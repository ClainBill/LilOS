#include "hal/hal.h"
#include <SDL2/SDL.h>

#include <unordered_map>
#include <queue>

// REMEMBER keep the HAL to things that the SDL requires specific to macos

// Initialise SDL variables
namespace {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;

    int SDLwidth;
    bool quit = false;

    // time and deltatime
    uint64_t lastFrameTime = 0;
    float deltaTime = 0.0f;

    std::unordered_map <hal::Key, bool> keyState;

    // Helper: convert SDL keysym to our Key enum
    hal::Key sdlKeyToHalKey(SDL_Keycode sym) {
        switch (sym) {
            case SDLK_ESCAPE:    return hal::Key::Escape;
            case SDLK_TAB:       return hal::Key::Tab;
            case SDLK_RETURN:    return hal::Key::Enter;
            case SDLK_BACKSPACE: return hal::Key::Backspace;
            case SDLK_LEFT:      return hal::Key::Left;
            case SDLK_RIGHT:     return hal::Key::Right;
            case SDLK_UP:        return hal::Key::Up;
            case SDLK_DOWN:      return hal::Key::Down;
            case SDLK_LSHIFT:
            case SDLK_RSHIFT:    return hal::Key::Shift;
            case SDLK_COMMA:     return hal::Key::Comma;
            case SDLK_PERIOD:    return hal::Key::Period;
            case SDLK_a:         return hal::Key::A;
            case SDLK_b:         return hal::Key::B;
            case SDLK_c:         return hal::Key::C;
            case SDLK_d:         return hal::Key::D;
            case SDLK_e:         return hal::Key::E;
            case SDLK_f:         return hal::Key::F;
            case SDLK_g:         return hal::Key::G;
            case SDLK_h:         return hal::Key::H;
            case SDLK_i:         return hal::Key::I;
            case SDLK_j:         return hal::Key::J;
            case SDLK_k:         return hal::Key::K;
            case SDLK_l:         return hal::Key::L;
            case SDLK_m:         return hal::Key::M;
            case SDLK_n:         return hal::Key::N;
            case SDLK_o:         return hal::Key::O;
            case SDLK_p:         return hal::Key::P;
            case SDLK_q:         return hal::Key::Q;
            case SDLK_r:         return hal::Key::R;
            case SDLK_s:         return hal::Key::S;
            case SDLK_t:         return hal::Key::T;
            case SDLK_u:         return hal::Key::U;
            case SDLK_v:         return hal::Key::V;
            case SDLK_w:         return hal::Key::W;
            case SDLK_x:         return hal::Key::X;
            case SDLK_y:         return hal::Key::Y;
            case SDLK_z:         return hal::Key::Z;
            default:             return hal::Key::Undefined; // Fallback
        }
    }
    std::queue<hal::KeyEvent> charQueue;
}


// Render the framebuffer to SDL window
void hal::present(const uint32_t* framebuffer){
        // TODO make this use faster update method. SDL_UpdateTexture is apparently slow for updates every frame
        SDL_UpdateTexture(texture, nullptr, framebuffer, SDLwidth * sizeof(uint32_t)); 
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);

        // update deltatime
        uint64_t currentTime = SDL_GetTicks64();
        deltaTime = (currentTime - lastFrameTime) / 1000.0f; // convert to ms
        lastFrameTime = currentTime;
}

void hal:: updateInput() {
    SDL_Event e;
    while (SDL_PollEvent(&e)){
        if (e.type == SDL_QUIT) quit = true;

        if (e.type == SDL_KEYDOWN) {
            hal::Key key = sdlKeyToHalKey(e.key.keysym.sym);
            keyState[key] = true;
        }
        if (e.type == SDL_KEYUP) {
            hal::Key key = sdlKeyToHalKey(e.key.keysym.sym);
            keyState[key] = false;
        }

        // For event based text input (text editors etc.)
        if (e.type == SDL_TEXTINPUT) {
            hal::KeyEvent ke;
            ke.ch = e.text.text[0];
            ke.key = sdlKeyToHalKey(e.key.keysym.sym);
            charQueue.push(ke);
        }
    }
}

bool hal::pollCharEvent(KeyEvent& out){
    if (charQueue.empty()) return false;
    out = charQueue.front();
    charQueue.pop();
    return true;
}

bool hal::isKeyPressed(hal::Key key){
    auto it = keyState.find(key);
    return it != keyState.end() && it->second;
}

float hal::getDeltaTime() {return deltaTime;}
bool hal::quitRequested() {return quit;}

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
    lastFrameTime = SDL_GetTicks64();

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