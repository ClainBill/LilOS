// THIS IS EXAMPLE CODE FROM GEMINI

// The idea is to create a superclass for apps so anything (like the terminal screen) will create a subclass from this.
// maybe rename to Window or whatever

#pragma once
# include "hal/hal.h"
#include <string>

class App {
public:
    std::string appName;

    // A virtual destructor is required for safe memory cleanup of inherited classes
    virtual ~App() = default; 

    // Every app must implement these two functions
    virtual void update(float deltaTime) = 0;
    virtual void render() = 0;
    
    virtual void handleInput() = 0;

    // Maybe a void for delete?

    // You could also add things like:
    // virtual void OnEnter() = 0;
    // virtual void OnExit() = 0;
};