#pragma once

#include <cstdint>

// Use forward declarations instead of header files as we only pass a pointer
struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;

extern const int WIDTH;
extern const int HEIGHT;
extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern SDL_Texture* texture;
extern uint32_t* framebuffer;