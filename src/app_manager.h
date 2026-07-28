#pragma once
#include <vector>
#include "hal/hal.h"

class App;

class AppManager {
private:
    std::vector<App*> apps;           // All active apps
    App* focusedApp = nullptr;          // Which one gets input?

public:
    ~AppManager();
    void openApp(App* app, bool grabFocus = true);
    void handleInput();
    void update(float deltaTime);
    void render();
    void setFocus(App* focusApp);
};